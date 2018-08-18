//*********************************************************************************************************************************
//
// PROJECT:							gnuCash-lib (gnuCash-Library)
// FILE:								processCSV
// SUBSYSTEM:						Processing CSV files in various formats
// LANGUAGE:						C++
// TARGET OS:						UNIX/LINUX/WINDOWS/MAC
// LIBRARY DEPENDANCE:	Qt
// NAMESPACE:						WSd
// AUTHOR:							Gavin Blakeman (GGB)
// LICENSE:             GPLv2
//
//                      Copyright 2018 Gavin Blakeman.
//                      This file is part of the gnuCash-lib (gnuCash-Library)
//
//                      gnuCash-lib is free software: you can redistribute it and/or modify it under the terms of the GNU General
//                      Public License as published by the Free Software Foundation, either version 2 of the License, or (at your
//                      option) any later version.
//
//                      gnuCash-lib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
//                      implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
//                      for more details.
//
//                      You should have received a copy of the GNU General Public License along with gnuCash-pud.  If not,
//                      see <http://www.gnu.org/licenses/>.
//
// OVERVIEW:            Implements the main(...) function
//
// HISTORY:             2018-06-28/GGB - Development of gnuCash-lib
//
//*********************************************************************************************************************************

#ifndef PROCESSCSV_H
#define PROCESSCSV_H

  // Standard C++ library

#include <sstream>
#include <string>

  // gnuCash-lib header files.

#include "database.h"

void processCSV(std::stringstream &, DCommodityValues &);

#endif // PROCESSCSV_H

