//*********************************************************************************************************************************
//
// PROJECT:							gnuCash-pud (gnuCash-Price Upload Daemon)
// FILE:								downloadManager
// SUBSYSTEM:						Manages downloads from the Alpha Vantage API
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

#include "../include/downloadManager.h"

  // Standard C++ library header files.

//#include <chrono>
//#include <ctime>
//#include <thread>

  // gnuCash-pud header files

//#include "include/database.h"
#include "../include/settings.h"

  // Miscellaneous library header files.

#include "boost/format.hpp"


/// @brief Constructor for the class.
/// @throws None.
/// @version 2018-08-11/GGB - Function created.

CDownloadManager::CDownloadManager()
{
  connect(&manager, SIGNAL(finished()), this, SLOT(downloadFinished()));
}

/// @brief Start the download of data from the specified URL.
/// @param[in] url: The URL to request for download.
/// @throws None.
/// @note 1. https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=MSFT&apikey=demo&datatype=csv
/// @version 2018-08-12/GGB - Function created.

void CDownloadManager::downloadData(std::string const &commodityName)
{
  QString urlString = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY";

  urlString += "&symbol=" + QString::fromStdString(commodityName);
  urlString += "&apikey=" + settings.value(APIKEY).toString();
  urlString += "&datatype=csv";

  QUrl url(urlString);

  INFOMESSAGE("Downloading from URL: " + urlString.toStdString());

  networkReply = manager.get(QNetworkRequest(url));

//#if QT_CONFIG(ssl)
//    connect(reply, SIGNAL(sslErrors(QList<QSslError>)), SLOT(sslErrors(QList<QSslError>)));
//#endif
}

/// @brief Returns the downloaded data as a std::string
/// @returns The downloaded data as a std string.
/// @version 2018-08-12/GGB - Function created.

std::string CDownloadManager::downLoadedData()
{
  return downloadedData_.toStdString();
}

/// @brief Function called when the download is finished.
/// @param[in] reply:
/// @throws None.
/// @version 2018-08-11/GGB - Function created.

void CDownloadManager::downloadFinished(QNetworkReply *reply)
{
  QUrl url = reply->url();
  if (reply->error())
  {
    ERRORMESSAGE(boost::str(boost::format("Download of %s failed: %s") % url.toEncoded().constData() %
                            reply->errorString().toStdString()));
  }
  else
  {
    //if (isHttpRedirect(reply))
    //{
//      fputs("Request was redirected.\n", stderr);
  //  }
//    else
//    {
      downloadedData_ = reply->readAll();
      DEBUGMESSAGE(boost::str(boost::format("Download of %s succeeded.") % url.toEncoded().constData()));
//    }
  };

  reply->deleteLater();
  reply = nullptr;

  emit fileDownloadFinished();
}

