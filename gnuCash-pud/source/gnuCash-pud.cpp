﻿//*********************************************************************************************************************************
//
// PROJECT:							gnuCash-pud (gnuCash-Price Upload Daemon)
// FILE:								main application
// SUBSYSTEM:						main(...)
// LANGUAGE:						C++
// TARGET OS:						UNIX/LINUX/WINDOWS/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						WS
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

  // Standard C++ library header files

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

  // Miscellaneous library header files.

#include <boost/algorithm/string.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/chrono.hpp>
#include "boost/filesystem.hpp"
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/program_options.hpp>
#include <GCL>
#include <QCL>

  // gnuCash-pud header files.

#include "include/service.h"

/// @brief Main function for the service.
//
/// @version 2018-06-28/GGB - Function created.

int main(int argc, char *argv[])
{
  int returnValue = -1;

  std::string ipaddr;
  int port;
  int pollInterval;
  std::string dbDriver;
  std::string dbIP;
  int dbPort;
  std::string dbName;
  std::string dbUser;
  std::string dbPassword;
  boost::filesystem::path commodityFilePath;

  boost::program_options::options_description cmdLine("Allowed Options");
  cmdLine.add_options()
      ("help,h", "produce help message")
      //("ipaddr", boost::program_options::value<std::string>(&ipaddr), "IP address of Weather Station")
      //("port", boost::program_options::value<int>(&port)->default_value(22222), "port to use with the Weather Station <22222>")
      ("pollinterval", boost::program_options::value<int>(&pollInterval)->default_value(5), "interval to poll the Weather Station <5>")
      ("dbdriver", boost::program_options::value<std::string>(&dbDriver)->default_value("MYSQL"), "database type <MYSQL>")
      ("dbip", boost::program_options::value<std::string>(&dbIP)->default_value("localhost"), "database host address <localhost>")
      ("dbport", boost::program_options::value<int>(&dbPort)->default_value(3306), "database port <3306>")
      ("dbname", boost::program_options::value<std::string>(&dbName)->default_value("WEATHER"), "database name <WEATHER>")
      ("dbuser", boost::program_options::value<std::string>(&dbUser)->default_value("WEATHER"), "database username <WEATHER>")
      ("dbpassword", boost::program_options::value<std::string>(&dbPassword)->default_value("WEATHER"), "database password <WEATHER>")
     // ("siteid", boost::program_options::value<unsigned long>(&siteID)->default_value(53), "site ID value")
     // ("instrumentid", boost::program_options::value<unsigned long>(&instrumentID)->default_value(1), "instrument ID value")
      ("writesettings", "write the settings to conf file then exit.")
      ("install,i", "Install the service.")
      ("uninstall,u", "Uninstall the service.")
      ("exec,e", "Execute as standalone application.")
      ("terminate,t", "Stop the service.")
      ("pause,p", "Pause the service.")
      ("resume,r", "Resume a paused service.")
      ("version,v", "Display version and status information.")
      ("debug", "Display debug information.")
      ("trace", "Capture Trace Information.")
      ;

  boost::program_options::variables_map vm;
  boost::program_options::store(boost::program_options::command_line_parser(argc, argv).
                                options(cmdLine)
                                .run(), vm);
  boost::program_options::notify(vm);

  if (vm.count("help"))
  {
    std::cout << cmdLine << std::endl;
    GCL::logger::defaultLogger().shutDown();
    return 0;
  };

  QSettings settings(VWL::settings::FILENAME, QSettings::NativeFormat);

  settings.setValue(VWL::settings::WS_POLLINTERVAL, QVariant(pollInterval));

  boost::to_upper(dbDriver);
  settings.setValue(DATABASE, QVariant(QString::fromStdString(dbDriver)));

  if (dbDriver == "MYSQL")
  {
    settings.setValue(MYSQL_DRIVERNAME, QVariant(VWL::QDRV_MYSQL));
    settings.setValue(MYSQL_HOSTADDRESS, QVariant(QString::fromStdString(dbIP)));
    settings.setValue(MYSQL_PORT, QVariant(dbPort));
    settings.setValue(MYSQL_DATABASENAME, QVariant(QString::fromStdString(dbName)));
    settings.setValue(MYSQL_USERNAME, QVariant(QString::fromStdString(dbUser)));
    settings.setValue(MYSQL_PASSWORD, QVariant(QString::fromStdString(dbPassword)));
  }
  else
  {
    ERRORMESSAGE("gnuCash can only use mySQL.");
  }

  if (vm.count("writeSettings"))
  {
    std::cout << "Settings written exiting" << std::endl;
    GCL::logger::defaultLogger().shutDown();
    return 0;
  };

      // Create the logger.

  GCL::logger::PLoggerSink fileLogger(new GCL::logger::CFileSink());

  if (vm.count("debug") && (vm.count("trace")))
  {
    fileLogger->setLogLevel(GCL::logger::CSeverity{true, true, true, true, true, true, true});
  }
  else if (vm.count("debug"))
  {
    fileLogger->setLogLevel(GCL::logger::CSeverity{false, false, true, true, true, true, true});
  }
  else
  {
    fileLogger->setLogLevel(GCL::logger::CSeverity{false, false, true, true, true, true, true});
  };

  std::dynamic_pointer_cast<GCL::logger::CFileSink>(fileLogger)->setLogFileName("/home/gavin", "gnuCash-pud", ".log");
  std::dynamic_pointer_cast<GCL::logger::CFileSink>(fileLogger)->openLogFile();

  GCL::logger::defaultLogger().addSink(fileLogger);
  GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "File Logger Created.");

  try
  {
    GCL::logger::defaultLogger().logMessage(GCL::logger::notice, "Service starting.");

    DEBUGMESSAGE("Creating Service");
    gnuCash_pud::service::CWSService service(argc, argv, commodityFilePath);

    DEBUGMESSAGE("Executing Service");
    returnValue = service.exec();

    GCL::logger::defaultLogger().logMessage(GCL::logger::notice, "Service Terminated. Return Value: " + std::to_string(returnValue));

    GCL::logger::defaultLogger().shutDown();
    return returnValue;

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

      returnValue = ( error.errorCode() );
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
    };

  GCL::logger::defaultLogger().shutDown();
  return returnValue;
}

