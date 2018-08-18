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

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

  // Standard C++ library headers

#include <memory>

  // Miscellaneous includes.

#include "include/qt.h"


class CDownloadManager : public QObject
{
  Q_OBJECT

private:
  QNetworkAccessManager manager;
  QNetworkReply *networkReply;
  QByteArray downloadedData_;

protected:

public:
  CDownloadManager();

  void downloadData(std::string const &);
  std::string downLoadedData();

signals:
  void fileDownloadFinished();

public slots:
  void downloadFinished(QNetworkReply *reply);
};


#endif // DOWNLOADMANAGER_H
