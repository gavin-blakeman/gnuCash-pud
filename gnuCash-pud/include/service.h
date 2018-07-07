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

#ifndef SERVICE_H
#define SERVICE_H

  // Miscellaneous include files

#include "boost/filesystem.hpp"
#include "qtservice/src/QtServiceBase"

#include "../include/statemachine.h"

namespace gnuCash_pud
{
  namespace service
  {
    class CService : public QObject, public QtService<QCoreApplication>
    {
      Q_OBJECT

    private:
      CStateMachine *stateMachine;
      boost::filesystem::path commodityListName_;

    protected:
      void start();
      void stop();
      void pause() {}
      void resume();

    public:
      CService(int argc, char **argv, boost::filesystem::path const &commodityListName);
      virtual ~CService();
    };

  } // namespace service

}   // namespace gnuCash_pud

#endif // SERVICE_H
