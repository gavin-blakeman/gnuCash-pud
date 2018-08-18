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
// HISTORY:             2018-07-05/GGB - Development of gnuCash-pud
//
//*********************************************************************************************************************************

#ifndef SETTINGS_H
#define SETTINGS_H

#include "include/qt.h"

QString const FILENAME                                  ("/usr/local/gnuCash-pud/gnuCash-pud.conf");

QString const ORG_NAME                                  ("GPLv2");
QString const APPL_NAME                                 ("gnuCash-pud");

QString const QDRV_MYSQL                                ("QMYSQL");

  // Definitions for the Database section

QString const DATABASE                                  ("Database/Database");
QString const MYSQL_DRIVERNAME                          ("Database/MySQL/DriverName");
QString const MYSQL_HOSTADDRESS                         ("Database/MySQL/HostAddress");
QString const MYSQL_DATABASENAME                        ("Database/MySQL/DatabaseName");
QString const MYSQL_PORT                                ("Database/MySQL/Port");
QString const MYSQL_USERNAME                            ("Database/MySQL/UserName");
QString const MYSQL_PASSWORD                            ("Database/MySQL/Password");

QString const COMMODITYLIST_FILENAME                    ("CommodityList/FileName");

QString const APIKEY                                    ("APIKey");

QSettings extern settings;

void createDefaultSettings();

#endif // SETTINGS_H
