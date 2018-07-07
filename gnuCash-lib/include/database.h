//*********************************************************************************************************************************
//
// PROJECT:							gnuCash-lib (gnuCash-Library)
// FILE:								database
// SUBSYSTEM:						Database Access
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

#ifndef DATABASE
#define DATABASE

  // Standard C++ library definitions.

#include <cstdint>
#include <memory>
#include <vector>

#include "include/qt.h"

#include "include/database.h"

#include <GCL>
#include <QCL>

struct SCommodityValue
{
  std::uint16_t year;       // Full year value (2018)
  std::uint16_t month;      // Month 1- January
  std::uint16_t day;        // Day 1 - 1st day of month.
  std::string source;
  std::string type;
  double value;
};

typedef std::vector<SCommodityValue> DCommodityValues;

class CGnuCashDatabase : public QCL::CDatabase
{
private:
  GCL::sqlwriter::CSQLWriter sqlWriter;
  std::unique_ptr<QSqlQuery> sqlQuery;       ///< Pointer to the query that will be used

  void processErrorInformation() const;

protected:
public:
  CGnuCashDatabase();

  bool createConnection(QString const &, QString const &, std::uint16_t, QString const &, QString const &, QString const &);

  bool writeCurrencyValues(DCommodityValues const &, std::string const &commodityName, std::string const &currencyName);
};

#endif // DATABASEGNUCASH

