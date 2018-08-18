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

#include "../include/processCSV.h"

  // Standard C++ library header files.

#include <stdexcept>

  // Miscellaneous library include files.

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <GCL>

/// @brief Processes a CSV file that is supplied as data in a string.
/// @param[in] fileData: Stream containing the file data to be read.
/// @param[out] commodityValues: storage class containing the values that can be written to the database.
/// @throws std::runtime_error
/// @version 2018-08-12/GGB - Function created.

void processCSV(std::stringstream &fileData, DCommodityValues &commodityValues)
{
  std::string szLine;
  std::size_t indexStart= 0, indexEnd = 0;
  std::string szClose;
  std::string szDate;
  std::string szTemp;
  std::uint64_t lineCount = 1;

  DEBUGMESSAGE("Starting processing .csv file...");

    // Read the first line. This line should contain a list of column values seperated by ",".

  std::getline(fileData, szLine);
  indexEnd = szLine.find_first_of(", \t", indexStart);
  szTemp = szLine.substr(indexStart, indexEnd - indexStart);
  indexStart = indexEnd + 1;

  boost::algorithm::to_lower(szTemp);

  if (szTemp != "timestamp")
  {
      // Unknown file format.

    ERRORMESSAGE("Unknown file format.");
    throw std::runtime_error("Unknown file format.");
  };

    /// @todo Add more smarts to determine columns automatically.

  while (!fileData.eof())
  {
    indexStart = indexEnd = 0;

    std::getline(fileData, szLine);
    lineCount++;

    if (!szLine.empty())
    {

        // Decompose the line into the relevant values.
        // Line is in the format "date, open, high, low, close, volume"
        // We only need date and close.

        // Date/timestamp

      indexEnd = szLine.find_first_of(", \t", indexStart);
      szDate = szLine.substr(indexStart, indexEnd - indexStart);
      indexStart = indexEnd + 1;

        // open

      indexEnd = szLine.find(",", indexStart);
      indexStart = indexEnd + 1;

        // high

      indexEnd = szLine.find(",", indexStart);
      indexStart = indexEnd + 1;

        // low

      indexEnd = szLine.find(",", indexStart);
      indexStart = indexEnd + 1;

        // close

      indexEnd = szLine.find(",", indexStart);
      szClose = szLine.substr(indexStart, indexEnd - indexStart);

        // Now parse the strings and create the commodity entry.

      try
      {
        SCommodityValue commodityValue;

        boost::trim(szDate);
        boost::trim(szClose);

          // Date should be in form yyyy-mm-dd.

        indexStart = 0;
        indexEnd = szDate.find("-", indexStart);
        szTemp = szDate.substr(indexStart, indexEnd - indexStart);
        commodityValue.year = boost::lexical_cast<std::uint16_t>(szTemp);

        indexStart = indexEnd + 1;
        indexEnd = szDate.find("-", indexStart);
        szTemp = szDate.substr(indexStart, indexEnd - indexStart);
        commodityValue.month = boost::lexical_cast<std::uint16_t>(szTemp);

        indexStart = indexEnd + 1;
        indexEnd = szDate.size();
        szTemp = szDate.substr(indexStart, indexEnd - indexStart);
        commodityValue.day = boost::lexical_cast<std::uint16_t>(szTemp);

        commodityValue.type = "last";
        commodityValue.source = "Finance::Quote";
        commodityValue.value = boost::lexical_cast<double>(szClose);

        commodityValues.push_back(commodityValue);
      }
      catch(...)
      {
        WARNINGMESSAGE(boost::str(boost::format("Error in line %li. Continuing...") % lineCount));
      }
    };
  };    // while statement.

  DEBUGMESSAGE("Completed processing .csv file.");
}
