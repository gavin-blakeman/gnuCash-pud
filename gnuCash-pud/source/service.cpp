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

  // Miscellaneous header files.

#include "boost/format.hpp"
#include <GCL>
#include <QCL>
#include "include/database.h"

namespace gnuCash_pud
{
  // Software version information

  std::uint16_t const MAJORVERSION	= 2018;           // Major version (year)
  std::uint8_t const MINORVERSION	= 6;              // Minor version (month)
  std::uint16_t const BUILDNUMBER = 0x0000;
  std::string const BUILDDATE(__DATE__);

  /// @brief Returns the copyright string.
  /// @returns The copyright string.
  /// @throws
  /// @version 2018-06-28/GGB - Function created.

  std::string getCopyrightString()
  {
    QFile file(":/text/copyright_licence.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      return std::string("");
    }
    else
    {
      QTextStream data(&file);
      QString copyrightString = data.readAll();
      return copyrightString.toStdString();
    };
  }

  /// @brief Function to return the release string.
  /// @returns The release string.
  /// @throws
  /// @version 2018-06-28/GGB - Function created.

  std::string getReleaseString()
  {
    return boost::str(boost::format("Version: %u-%02u.%04X") % MAJORVERSION % MINORVERSION % BUILDNUMBER);
  }

  /// @version Function to return the build date.
  /// @returns The build date.
  /// @throws
  /// @version 2018-06-28/GGB - Function created.

  std::string getReleaseDate()
  {
    return BUILDDATE;
  }

  namespace service
  {

    /// @brief Constructor for the service.
    /// @param[in] argc: The argc value from the command line call.
    /// @param[in] argv: The argv value from the command line call.
    /// @param[in] commodityListName: The name of the file containing the list of commodities to fetch.
    /// @throws
    /// @version 2018-06-28/GGB - Function created.

    CService::CService(int argc, char **argv, boost::filesystem::path const &commodityListName)
      : QtService<QCoreApplication>(argc, argv, "gnuCash-pud"), stateMachine(nullptr), commodityListName_(commodityListName)
    {
      TRACEENTER;
      application()->setOrganizationName(VWL::settings::ORG_NAME);
      application()->setApplicationName(VWL::settings::APPL_NAME);

      setServiceDescription("gnuCash-pud");

      TRACEEXIT;
    }

    /// @brief Destructor for the class.
    /// @throws None.
    /// @version 2018-06-28/GGB - Function created.

    CService::~CService()
    {
      if (stateMachine)
      {
        delete stateMachine;
        stateMachine = nullptr;
      }
    }

    /// @brief Function to resume the daemon
    /// @throws
    /// @version 2018-06-28/GGB - Function created.

    void CService::resume()
    {
      INFOMESSAGE("Daemon restarted.");
      stateMachine->start();
    }

    /// @brief This is the main part of the service. All the code for the service creation needs to go in here.
    /// @throws
    /// @version 2018-06-28/GGB - Function created.

    void CService::start()
    {
      TRACEENTER;
      std::ostringstream os;

        /* Write some messages for the user. */

      INFOMESSAGE("Application: WSd.");
      INFOMESSAGE("Copyright: Gavin Blakeman 2018.");
      INFOMESSAGE("License: GPLv2.");
      INFOMESSAGE(getReleaseString());
      INFOMESSAGE("Release Date:" + getReleaseDate());

        // Create the state machine

      DEBUGMESSAGE("Creating state machine...");
      stateMachine = new CStateMachine(this, );
      DEBUGMESSAGE("State machine created.");

        // Indicate that the service is starting.

      INFOMESSAGE("Daemon Started.");
      stateMachine->start();

      TRACEEXIT;
    }

    /// @brief Function to stop the daemon
    /// @throws
    /// @version 2018-06-28/GGB - Function created.

    void CWSService::stop()
    {
      INFOMESSAGE("Stop Daemon");

      stateMachine->stop();
    }
  }   // namespace service
}   // namespace gnuCash_pud
