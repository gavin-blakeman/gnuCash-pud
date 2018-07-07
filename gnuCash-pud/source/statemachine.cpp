//*********************************************************************************************************************************
//
// PROJECT:							gnuCash-pud (gnuCash-Price Upload Daemon)
// FILE:								StateMachine
// SUBSYSTEM:						StateMachine
// LANGUAGE:						C++
// TARGET OS:						UNIX/LINUX/WINDOWS/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						gnuCash_pud:settings
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
// OVERVIEW:            Implements the service
//
// HISTORY:             2018-06-28/GGB - Development of gnuCash-pud
//
//*********************************************************************************************************************************

#include "../include/statemachine.h"

  // Standard C++ libraries

#include <chrono>
#include <ctime>
#include <thread>

  // Miscellaneous libraries

#include "include/database.h"
#include <GCL>

namespace gnuCash_pud
{
  /// @brief Constructor for the state machine class.
  /// @param[in] np: Parent object.
  /// @param[in]
  /// @version 2018-06-28/GGB - Function created.

  CStateMachine::CStateMachine(QObject *np) : parent(np), pollTimer(nullptr)
  {
    //tcpSocket = new tcp::CTCPSocket(parent, siteID, instrumentID);

    std::this_thread::sleep_for(std::chrono::seconds(60));

    pollTimer = new QTimer();
    connect(pollTimer, SIGNAL(timeout()), this, SLOT(pollModeTimer()));
    pollTimer->setInterval(std::chrono::milliseconds(std::chrono::minutes(60).count()).count());

    //VWL::database.connectToDatabase();
  }

  /// @brief Destructor - Frees dynamically allocated objects
  /// @throws None
  /// @version 2018-06-28/GGB - Function created.

  CStateMachine::~CStateMachine()
  {
    if (pollTimer)
    {
      delete pollTimer;
      pollTimer = nullptr;
    };

    if (tcpSocket)
    {
      delete tcpSocket;
      tcpSocket = nullptr;
    };
  }

  /// @brief Slot for the poll mode timer.
  /// @throws None.
  /// @version 2018-06-28/GGB - Function created.

  void CStateMachine::pollModeTimer()
  {
    static bool timeUpdated = false;
    uint16_t dateValue, timeValue;
    double t1, t2;

      // Check if it is

    TRACEENTER;
    try
    {
      DEBUGMESSAGE("Connecting to database");
      //database.openDatabase();
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    catch(...)
    {
      GCL::logger::defaultLogger().logMessage(GCL::logger::error, "Unable to connect to database.");
    };


    //GCL::logger::defaultLogger().logMessage(GCL::logger::debug, "Polling Weather System Device.");

    //tcpSocket->readArchive();

    std::time_t time = std::time(&time);
    struct tm *currentTime = std::localtime(&time);

    if ( (currentTime->tm_hour == 0) && (currentTime->tm_min < 10) && (!timeUpdated))
    {
      timeUpdated = tcpSocket->setTime();
    }
    else if ( (currentTime->tm_hour == 23) && (timeUpdated) )
    {
      timeUpdated = false;
    }
    else
    {
      VWL::database.lastWeatherRecord(siteID, instrumentID, dateValue, timeValue);
      dateValue = currentTime->tm_hour * 60 + currentTime->tm_min;                 // Time in minutes after start of day
      timeValue = (timeValue / 100) * 60 + (timeValue % 100);                                  // Convert time to minutes.

      t1 = std::abs(720 - dateValue);
      t2 = std::abs(720 - timeValue);

      if (std::abs(t1 - t2) > 5)        // If time difference greater than 5 minutes.
      {
        timeUpdated = tcpSocket->setTime();
      }
    }

    VWL::database.closeDatabase();

    TRACEEXIT;
  }

  /// @brief Function to start the poll mode.
  /// @throws None.
  /// @version 2018-06-28/GGB - Function created.

  void CStateMachine::start()
  {
    pollTimer->start();
  }

  /// @brief Function to stop the poll mode.
  /// @throws None.
  /// @version 2018-06-28/GGB - Function created.

  void CStateMachine::stop()
  {
    pollTimer->stop();
  }

} // namespace OCWS
