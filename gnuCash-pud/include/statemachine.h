﻿//*********************************************************************************************************************************
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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

  // Standard C++ library files

#include <cstdint>

  // Miscellaneous header files

#include "boost/filesystem.hpp"
#include "include/qt.h"

  // gnuCash-pud header files.

#include "include/database.h"
#include "downloadManager.h"

namespace gnuCash_pud
{

  struct SConfiguration
  {
    std::string databaseDriver;
    std::string databaseIPAddress;
    std::uint16_t databasePort;
    std::string databaseSchema;
    std::string databaseUser;
    std::string databasePassword;
    boost::filesystem::path shareNamesPath;
    boost::filesystem::path shareNamesFile;
    std::string apiKey;
  };

  class CStateMachine : public QObject
  {
    Q_OBJECT

  private:
    QObject *parent;
    SConfiguration configurationData_;
    CGnuCashDatabase gnuCashDatabase;
    GCL::alarm::CAlarmWeekly alarmWeekly_;
    CDownloadManager downloadManager;
    boost::filesystem::path inputFileName;
    boost::filesystem::ifstream inputFileStream;

    void alarmCallback(std::uint64_t, void *);
    void processCommodity(std::string const &, std::string const &);

  protected:
  public:
    CStateMachine(QObject *);
    virtual ~CStateMachine();

    void start();
    void stop();

    SConfiguration &configurationData() { return configurationData_; }

  public slots:
    void webQueryComplete();

  };


} // namespace OCWS


#endif // STATEMACHINE_H
