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

#include "../include/database.h"

  // Standard C++ library header files.

#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

  // Miscellaneous libraries

#include "boost/format.hpp"

/// @brief Constructor for the database class.
/// @throws None.
/// @version 2018-06-23/GGB - Function created.

CGnuCashDatabase::CGnuCashDatabase() : CDatabase("GNUCASH"), sqlQuery()
{

}

/// @brief Gets the date of the last commodity value in the database.
/// @param[in] commodityUUID: The UUID of the commodity.
/// @throws
/// @version 2018-11-08/GGB - Function created.

void CGnuCashDatabase::getCommodityLastDate(std::string const &commodityUUID, std::tm &dateTime)
{
  std::string szDate;

  sqlWriter.resetQuery();
  sqlWriter.select({"date"}).from({"prices"}).
      where({GCL::sqlwriter::parameterTriple("guid", "=", commodityUUID)}).
      orderBy({std::make_pair("date", GCL::sqlwriter::CSQLWriter::DESC)});
  if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
  {
    sqlQuery->first();
    if (sqlQuery->isValid())
    {
      szDate = sqlQuery->value(0).toString().toStdString();

        // Convert string to a date value.

      std::istringstream ss(szDate);
      ss >> std::get_time(&dateTime, "%Y-%m-%d");
      if (ss.fail())
      {
        ERRORMESSAGE("Unable to parse date: " + szDate);
        throw std::runtime_error("Unable to parse date");
      };
    }
    else
    {
      processErrorInformation();
      ERRORMESSAGE("Cannot find commodity with GUID: " + commodityUUID);
      throw std::runtime_error("Cannot find commodity");
    };
  }
  else
  {
    processErrorInformation();
    throw std::runtime_error("query error");
  };
}

/// @brief Gets the commodity UUID from the database.
/// @param[in] commodityName: The name (mnemonic) to find the UUID of.
/// @param[out] commodityUUID: The UUID of the commodity.
/// @throws std::runtime_error
/// @version 2018-08-11/GGB - Function created.

void CGnuCashDatabase::getCommodityUUID(std::string const &commodityName, std::string &commodityUUID)
{
  sqlWriter.resetQuery();
  sqlWriter.select({"guid"}).from({"commodities"}).
      where({GCL::sqlwriter::parameterTriple("mnemonic", "=", commodityName)});
  if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
  {
    sqlQuery->first();
    if (sqlQuery->isValid())
    {
      commodityUUID = sqlQuery->value(0).toString().toStdString();
    }
    else
    {
      processErrorInformation();
      ERRORMESSAGE("Cannot find Share: " + commodityName);
      throw std::runtime_error("Cannot find share");
    };
  }
  else
  {
    processErrorInformation();
    throw std::runtime_error("query error");
  };
}

/// @brief Gets the UUID of the currency
/// @param[in] currencyName: The name (mnemonic) of the currency.
/// @param[out] currencyUUID: The UUID of the currency.
/// @throws std::runtime_error
/// @version 2018-08-11/GGB - Function created.

void CGnuCashDatabase::getCurrencyUUID(std::string const &currencyName, std::string &currencyUUID)
{
    // Now get the currency GUID

  sqlWriter.resetQuery();
  sqlWriter.select({"guid"}).from({"commodities"}).
      where({GCL::sqlwriter::parameterTriple("mnemonic", "=", currencyName),
             GCL::sqlwriter::parameterTriple("namespace", "=", "'CURRENCY'")});
  if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
  {
    sqlQuery->first();
    if (sqlQuery->isValid())
    {
      currencyUUID = sqlQuery->value(0).toString().toStdString();
    }
    else
    {
      processErrorInformation();
      ERRORMESSAGE("Cannot find Curreny: " + currencyName);
      throw std::runtime_error("Cannot find currency");
    };
  }
  else
  {
    processErrorInformation();
    throw std::runtime_error("query error");
  };
}

/// @brief Processes and displays the error information.
/// @throws None.
/// @version 2017-08-13/GGB - FUnction created.

void CGnuCashDatabase::processErrorInformation() const
{
  QSqlError error = sqlQuery->lastError();

  ERRORMESSAGE("Error while executing query: " + sqlWriter.string());
  ERRORMESSAGE("Error returned by Driver: " + error.nativeErrorCode().toStdString());
  ERRORMESSAGE("Text returned by driver: " + error.driverText().toStdString());
  ERRORMESSAGE("Text returned by database: " + error.databaseText().toStdString());
}

/// @brief Writes currency records to the database.
/// @param[in] commodityValues: The commodity values to write.
/// @param[in] commodityName: The name of the commodity to write.
/// @param[in] currencyName: The name of the currency to use.
/// @returns
/// @throws std::runtime_error
/// @version 2018-06-23/GGB - Function created.

bool CGnuCashDatabase::writeCurrencyValues(DCommodityValues const &commodityValues, std::string const &commodityName,
                                           std::string const &currencyName)
{
  std::string commodityGUID;
  std::string currencyGUID;
  std::uint64_t commidityFraction;
  size_t recordCount = 0;

    // Database structure
    //  GUID
    //  commodity_guid
    //  currency_guid
    //  date -> 2020-06-07 16:00:00
    //  source -> user:price      user:price_editor   user:split_register   Finance::Quote
    //  type -> transaction     last
    //  value_num
    //  value_denom

    // First get the commodity GUID and fraction.

  sqlWriter.resetQuery();
  sqlWriter.select({"guid", "fraction"}).from({"commodities"}).
      where({GCL::sqlwriter::parameterTriple("mnemonic", "=", commodityName)});

  if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
  {
    sqlQuery->first();
    if (sqlQuery->isValid())
    {
      commodityGUID = sqlQuery->value(0).toString().toStdString();
      commidityFraction = sqlQuery->value(1).toULongLong();
    }
    else
    {
      processErrorInformation();
      ERRORMESSAGE("Cannot find Share: " + commodityName);
      throw std::runtime_error("Cannot find share.");
    };
  }
  else
  {
    processErrorInformation();
    throw std::runtime_error("query error.");
  };

    // Now get the currency GUID

  sqlWriter.resetQuery();
  sqlWriter.select({"guid"}).from({"commodities"}).
      where({GCL::sqlwriter::parameterTriple("mnemonic", "=", currencyName),
             GCL::sqlwriter::parameterTriple("namespace", "=", "'CURRENCY'")});
  if (sqlQuery->exec(QString::fromStdString(sqlWriter.string())))
  {
    sqlQuery->first();
    if (sqlQuery->isValid())
    {
      currencyGUID = sqlQuery->value(0).toString().toStdString();
    }
    else
    {
      processErrorInformation();
      ERRORMESSAGE("Cannot find Curreny: " + currencyName);
      throw std::runtime_error("Cannot find currency");
    };
  }
  else
  {
    processErrorInformation();
    throw std::runtime_error("query error");
  };

  sqlWriter.resetQuery();
  sqlWriter.insertInto("prices",
                       {"guid", "commodity_guid", "currency_guid", "date", "source", "type", "value_num", "value_denom"})
      .values({{GCL::sqlwriter::bindValue(":guid"), commodityGUID, currencyGUID, GCL::sqlwriter::bindValue(":date"),
             "'Finance::Quote'", "'last'", GCL::sqlwriter::bindValue(":numerator"), GCL::sqlwriter::bindValue("denominator")
               }});

  sqlQuery->clear();
  sqlQuery->prepare(QString::fromStdString(sqlWriter.string()));

  for (auto cv : commodityValues)
  {
    std::uint64_t numerator;
    std::uint64_t denominator;

      // Check for duplicates

    GCL::sqlwriter::CSQLWriter sqlWriter2;
    QSqlQuery sqlQuery2(*dBase);

    sqlWriter2.resetQuery();
    sqlWriter2.select({"guid"}).from({"prices"}).where({GCL::sqlwriter::parameterTriple("commodity_guid", "=", commodityGUID),
                                                      GCL::sqlwriter::parameterTriple("currency_guid", "=", currencyGUID),
                                                      GCL::sqlwriter::parameterTriple("date", "=", boost::str(boost::format("%04u-%02u-%02u") % cv.year % cv.month % cv.day)),
                                                      GCL::sqlwriter::parameterTriple("source", "=", "'Finance::Quote'"),
                                                     });
    if (sqlQuery2.exec(QString::fromStdString(sqlWriter2.string())))
    {
      sqlQuery2.first();
      if (sqlQuery2.isValid())
      {
        INFOMESSAGE(boost::str(boost::format("Duplicate record found (%li of %li). %04i-%02i-%02i. Ignoring") % ++recordCount
                               % commodityValues.size() % cv.year % cv.month % cv.day));
      }
      else
      {
        denominator = commidityFraction;
        cv.value *= denominator;

          /// Capture an additional 8 decimal points if required.

        if (std::floor(cv.value) != cv.value)
        {
          denominator *= 100;
          cv.value *= 100;

          if (std::floor(cv.value) != cv.value)
          {
            denominator *= 100;
            cv.value *= 100;

            if (std::floor(cv.value) != cv.value)
            {
              denominator *= 100;
              cv.value *= 100;

              if (std::floor(cv.value) != cv.value)
              {
                denominator *= 100;
                cv.value *= 100;
              };
            };
          };
        };

        numerator = std::floor(cv.value);

        sqlQuery->bindValue(":guid", QUuid::createUuid().toString());
        sqlQuery->bindValue(":date", QString::fromStdString(boost::str(boost::format("%04u-%02u-%02u") % cv.year % cv.month % cv.day)));
        sqlQuery->bindValue(":numerator", static_cast<qulonglong>(numerator));
        sqlQuery->bindValue(":denominator", static_cast<qulonglong>(denominator));

        if (!sqlQuery->exec())
        {
          processErrorInformation();
        };

        recordCount++;
      }
    }
    else
    {
      processErrorInformation();
      throw std::runtime_error("query error");
    };

    sqlQuery2.finish();

    if( (recordCount % 100) == 0)
    {
      INFOMESSAGE(boost::str(boost::format("Completed %li of %li records.") % recordCount % commodityValues.size()));
    };
  };      // For loop.

  INFOMESSAGE("Completed writing records to database.");
}
