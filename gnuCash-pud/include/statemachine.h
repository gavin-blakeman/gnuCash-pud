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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

  // Standard C++ library files

#include <cstdint>

  // Miscellaneous header files

#include "boost/filesystem.hpp"
#include "include/qt.h"

  // gnuCash-pud header files.

#include "tcp.h"

namespace gnuCash_pud
{

  class CStateMachine : public QObject
  {
    Q_OBJECT

  public:
    boost::filesystem::path commodityListName;

  private:
    QObject *parent;
    //EPollState pollModeState;
    tcp::CTCPSocket *tcpSocket = nullptr;
    QTimer *pollTimer;

  protected:
  public:
    CStateMachine(QObject *);
    virtual ~CStateMachine();

    void start();
    void stop();

  public slots:
    void pollModeTimer();
  };


} // namespace OCWS


#endif // STATEMACHINE_H
