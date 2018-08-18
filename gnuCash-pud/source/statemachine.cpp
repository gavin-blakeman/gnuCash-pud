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

  // gnuCash-pud header files

#include "../include/settings.h"
#include "include/processCSV.h"

  // Standard C++ libraries

#include <chrono>
#include <ctime>
#include <thread>

  // Miscellaneous libraries

#include <boost/algorithm/string.hpp>
#include <GCL>

namespace gnuCash_pud
{
  /// @brief Constructor for the state machine class.
  /// @param[in] np: Parent object.
  /// @param[in]
  /// @version 2018-06-28/GGB - Function created.

  CStateMachine::CStateMachine(QObject *np) : parent(np),
    alarmWeekly_(std::bind(&CStateMachine::alarmCallback, this, std::placeholders::_1, std::placeholders::_2), nullptr)
  {
    std::this_thread::sleep_for(std::chrono::seconds(60));
  }

  /// @brief Destructor - Frees dynamically allocated objects
  /// @throws None
  /// @version 2018-06-28/GGB - Function created.

  CStateMachine::~CStateMachine()
  {
  }

  /// @brief      The callback function to handle the state when the alarm is enabled. (IE, the desired day of week and time is met)
  /// @details    The function handles the opening of the list of the commodity names, reading the values from the website and
  ///             writing to the database.
  /// @param[in]  alarmHandle: The handle of the alarm that tripped the callback. (Only using one alarm so not used)
  /// @param[in]  callbackData: Data associated with the alarm. This will always be nullptr in this case.
  /// @throws
  /// @version    2018-08-11/GGB - Function created.

  void CStateMachine::alarmCallback(std::uint64_t /*alarmHandle */, void */*callbackData*/)
  {
    INFOMESSAGE("Beginning processing alarm...");

    gnuCashDatabase.createConnection(QString::fromStdString(configurationData_.databaseDriver),
                                     QString::fromStdString(configurationData_.databaseIPAddress),
                                     configurationData_.databasePort,
                                     QString::fromStdString(configurationData_.databaseSchema),
                                     QString::fromStdString(configurationData_.databaseUser),
                                     QString::fromStdString(configurationData_.databasePassword));

    inputFileStream.open(inputFileName);

      /// @todo Should be a check that the filestream was opened correctly.

      // Iterate over the entire stream and process each commodity in turn.
      // Format is one commodity per line.

    while (!inputFileStream.eof())
    {
      std::string szLine;
      std::size_t indexStart = 0, indexEnd = 0;
      std::string szCommodity;
      std::string szCurrency;

      std::getline(inputFileStream, szLine);

      indexEnd = szLine.find_first_of(", \t", indexStart);
      szCommodity = szLine.substr(indexStart, indexEnd - indexStart);
      boost::trim(szCommodity);

      indexStart = indexEnd + 1;
      indexEnd = szLine.find_first_of(", \t", indexStart);
      szCurrency = szLine.substr(indexStart, indexEnd - indexStart);
      boost::trim(szCurrency);

      processCommodity(szCommodity, szCurrency);
    }
    inputFileStream.close();

    gnuCashDatabase.closeConnection();

    INFOMESSAGE("Completed processing alarm.");
  }

  /// @brief Process a commodity.
  /// @details  Read from the database to find the last date available for the commodity. Read from the Alpha Vantage website, the
  ///           range of dates and data required. Write the data to the database.
  /// @param[in] commodityName: The name of the commodity to get.
  /// @throws
  /// @version 2018-08-11/GGB - Function created.

  void CStateMachine::processCommodity(std::string const &commodityName, std::string const &currencyName)
  {
    QEventLoop localEventLoop;
    DCommodityValues commodityValues;

    INFOMESSAGE("Processing stock: " + commodityName + "...");

      // Request data from the web service.

    connect(&downloadManager, SIGNAL(fileDownloadFinished()), &localEventLoop, SLOT(localEventLoop::quit));

    downloadManager.downloadData(commodityName);
    localEventLoop.exec();

    std::stringstream fileData(downloadManager.downLoadedData());

    processCSV(fileData, commodityValues);
    gnuCashDatabase.writeCurrencyValues(commodityValues, commodityName, currencyName);

    INFOMESSAGE("Completed processing stock: " + commodityName + ".");
  }

  /// @brief Function to start the poll mode.
  /// @throws None.
  /// @version 2018-06-28/GGB - Function created.

  void CStateMachine::start()
  {

  }

  /// @brief Function to stop the poll mode.
  /// @throws None.
  /// @version 2018-06-28/GGB - Function created.

  void CStateMachine::stop()
  {
    //pollTimer->stop();
  }

  /// @brief Function called when a webQuery is complete.
  /// @throws None.
  /// @version 2018-08-12/GGB - Function created.

  void CStateMachine::webQueryComplete()
  {

  }

} // namespace OCWS
