//*********************************************************************************************************************************
//
// PROJECT:							gnuCash-pud (gnuCash-Price Upload Daemon)
// FILE:								Service
// SUBSYSTEM:						Service
// LANGUAGE:						C++
// TARGET OS:						UNIX/LINUX/WINDOWS/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						gnuCash_pud
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
//                      This file is part of the gnuCash-pud (gnuCash-Price Upload Daemon)
//
//                      gnuCash-pud is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
//                      option) any later version.
//
//                      gnuCash-pud is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with gnuCash-pud.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Implements the main(...) function
//
// HISTORY:             2018-06-28/GGB - Development of gnuCash-pud
//
//*********************************************************************************************************************************

#include "../include/service.h"

  // Standard C++ library header files

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

  // Miscellaneous library header files.

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/chrono.hpp>
#include "boost/filesystem.hpp"
#include "boost/format.hpp"
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <GCL>
#include <QCL>

  // gnuCash-pud header files

#include "include/database.h"
#include "../include/settings.h"

QSettings settings(ORG_NAME, APPL_NAME);

namespace gnuCash_pud
{
    // Software version information

  std::uint16_t const MAJORVERSION	= 2018;           // Major version (year)
  std::uint16_t const MINORVERSION	= 9;              // Minor version (month)
  std::uint16_t const BUILDNUMBER = 0x008E;
  std::string const BUILDDATE(__DATE__);

  /// @brief Function to return the release string.
  /// @returns The release string.
  /// @throws std::bad_alloc
  /// @version 2018-06-28/GGB - Function created.

  std::string getReleaseString()
  {
    return boost::str(boost::format("Version: %u.%02u.%04X") % MAJORVERSION % MINORVERSION % BUILDNUMBER);
  }

  /// @version Function to return the build date.
  /// @returns The build date.
  /// @throws std::bad_alloc
  /// @version 2018-06-28/GGB - Function created.

  std::string getReleaseDate()
  {
    return BUILDDATE;
  }

  /// @brief Constructor for the service.
  /// @param[in] argc: The argc value from the command line call.
  /// @param[in] argv: The argv value from the command line call.
  /// @throws std::bad_alloc
  /// @note 1. Ensure there are no usage of the defaultLogger() in this constructor.
  /// @version 2018-06-28/GGB - Function created.

  CService::CService(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "gnuCash-pud"), stateMachine(this)
  {
    application()->setOrganizationName(ORG_NAME);
    application()->setApplicationName(APPL_NAME);

    setServiceDescription("gnuCash - Price Upload Daemon");
  }

  /// @brief Destructor for the class.
  /// @throws None.
  /// @version 2018-06-28/GGB - Function created.

  CService::~CService()
  {
  }

  /// @brief Function to resume the daemon
  /// @throws
  /// @version 2018-06-28/GGB - Function created.

  void CService::resume()
  {
    INFOMESSAGE("Daemon restarted.");
    stateMachine.start();
  }

  /// @brief This function is called when the service starts. The function is only called once.
  /// @throws
  /// @version 2018-06-28/GGB - Function created.

  void CService::start()
  {
    std::ifstream ifs;
    int returnValue = 0;
    std::string dbDriver;
    boost::filesystem::path logfilePath = "/home/gavin/";
    boost::filesystem::path logfileName;

    try
    {
        // Try to open the configuration file.

      ifs.open("/home/gavin/gnuCash-pud.conf");
      if (!ifs)
      {
        CRITICALMESSAGE("Current Path: " + boost::filesystem::current_path().string());
        CRITICALMESSAGE("Could not open configuration file. '/etc/gnuCash/gnuCash-pud.conf'");
        QCoreApplication::exit(1);
      };

      boost::program_options::options_description configFile("Allowed Options");
      configFile.add_options()
          ("database.driver", boost::program_options::value<std::string>(&stateMachine.configurationData().databaseDriver))
          ("database.ipaddress", boost::program_options::value<std::string>(&stateMachine.configurationData().databaseIPAddress))
          ("database.port", boost::program_options::value<std::uint16_t>(&stateMachine.configurationData().databasePort))
          ("database.schema", boost::program_options::value<std::string>(&stateMachine.configurationData().databaseSchema))
          ("database.user", boost::program_options::value<std::string>(&stateMachine.configurationData().databaseUser))
          ("database.password", boost::program_options::value<std::string>(&stateMachine.configurationData().databasePassword))
          ("sharenames.pathname", boost::program_options::value<boost::filesystem::path>(&stateMachine.configurationData().shareNamesPath))
          ("sharenames.filename", boost::program_options::value<boost::filesystem::path>(&stateMachine.configurationData().shareNamesFile))
          ("logging.filename", boost::program_options::value<boost::filesystem::path>(&logfileName))
          ("logging.path", boost::program_options::value<boost::filesystem::path>(&logfilePath))
          ("logging.loglevel.trace", "")
          ("logging.loglevel.debug", "")
          ("logging.loglevel.info", "")
          ("logging.loglevel.notice", "")
          ("alphavantage.apikey", boost::program_options::value<std::string>(&stateMachine.configurationData().apiKey))
          ;

      boost::program_options::variables_map vm;
      boost::program_options::store(boost::program_options::parse_config_file(ifs, configFile), vm);
      boost::program_options::notify(vm);

      ifs.close();		// Close the configuration file.

              // Create the file logger.

      GCL::logger::PLoggerSink fileLogger(new GCL::logger::CFileSink(logfilePath, logfileName));
      std::dynamic_pointer_cast<GCL::logger::CFileSink>(fileLogger)->openLogFile();
      GCL::logger::defaultLogger().addSink(fileLogger);

        // First thing to do is to disable the streamLogger as it is not needed in a daemon. (Produces strange results)

      GCL::logger::defaultLogger().removeDefaultStreamSink();

        /* Write some messages for the user. */

      INFOMESSAGE("Application: gnuCash-PUD.");
      INFOMESSAGE("Copyright: Gavin Blakeman 2018.");
      INFOMESSAGE("License: GPLv2.");
      INFOMESSAGE(getReleaseString());
      INFOMESSAGE("Release Date:" + getReleaseDate());

      fileLogger->trace(vm.count("logging.loglevel.trace"));
      fileLogger->debug(vm.count("logging.loglevel.debug"));
      fileLogger->info(vm.count("logging.loglevel.info"));
      fileLogger->notice(vm.count("logging.loglevel.notice"));

      INFOMESSAGE("Daemon Started.");

      boost::to_upper(dbDriver);

    //      /// @todo Include a check that the file is accessable and exit if not.



    //    /// @todo Add a sense check that all the necessary data is available.
    //    ///				IE, database data, commodity list, apikey etc.


    		// Test that the database is accessable.

    	{
        CGnuCashDatabase testDatabase;
        if (!testDatabase.createConnection(settings.value(MYSQL_DRIVERNAME).toString(),
                                           settings.value(MYSQL_HOSTADDRESS).toString(),
                                           settings.value(MYSQL_PORT).toUInt(),
                                           settings.value(MYSQL_DATABASENAME).toString(),
                                           settings.value(MYSQL_USERNAME).toString(),
                                           settings.value(MYSQL_PASSWORD).toString()))
        {
          CRITICALMESSAGE("Unable to open connection to database. Exiting.");
          GCL::logger::defaultLogger().shutDown();
          QCoreApplication::exit(6);
        }
        else
        {
          testDatabase.closeConnection();
        }
      };

      stateMachine.start();

    }
    catch (GCL::CError &error)
    {
      boost::chrono::system_clock::time_point timeStamp = boost::chrono::system_clock::now();
       std::clog << "[" << timeStamp << "] - ";
      std::clog << "GCL unhandled exception: " << error.errorCode() << " - ";
      std::clog << error.errorMessage() << std::endl;

      timeStamp = boost::chrono::system_clock::now();
      std::clog << "[" << timeStamp << "] - ";
      std::clog << "Application Terminated: Return Value: " << error.errorCode() << std::endl;

      QCoreApplication::exit(error.errorCode());
    }
    catch(std::exception &e)
    {
      std::cerr << e.what() << std::endl;
      QCoreApplication::exit(4);
    }
    catch(std::exception &E)
    {
      CRITICALMESSAGE(E.what());
      QCoreApplication::exit(5);
    }
    catch(...)
    {
      boost::chrono::system_clock::time_point timeStamp = boost::chrono::system_clock::now();
      std::clog << "[" << timeStamp << "] - ";
      std::clog << "Unable to initialise logger. Terminating" << std::endl;
      returnValue = -2;
      timeStamp = boost::chrono::system_clock::now();
      std::clog << "[" << timeStamp << "] - ";
      std::clog << "Application Terminated: Return Value: " << returnValue << std::endl;
      QCoreApplication::exit(3);
    };
  }

  /// @brief Function to stop the daemon
  /// @throws
  /// @version 2018-06-28/GGB - Function created.

  void CService::stop()
  {
    INFOMESSAGE("Stop Daemon");

    stateMachine.stop();
  }

}   // namespace gnuCash_pud
