/****************************************************************************************
 * Copyright (c) 2010 Varrun Ramani <varrunr@gmail.com>                                 *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 2 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

#ifndef HTTPDAEMON_H
#define HTTPDAEMON_H

#include<QTcpServer>
#include<QObject>
#include<QHostAddress>
#include "QueryHandler.h"

class HttpDaemon : public QTcpServer
{
     Q_OBJECT

public:

    /**
     *   Creates a new HTTP daemon listening to a port
     *   @param Port number to listen on
     *   @param Assign a parent object
     */
     HttpDaemon(quint16, QObject* parent = 0);

     /**
      *   When a new client connects, the server constructs a QTcpSocket and all
      *   communication with the client is done over this QTcpSocket. QTcpSocket
      *   works asynchronously, this means that all the communication is done
      *   in the two slots readClient() and discardClient().
      */
     void incomingConnection(int socket);

     void pause();

     void resume();

signals:
     void startQueryConversion();

 private slots:
     /**
      *   Reads the socket for extracting POST requests (XML Query)
      */
     void readClient();
     void discardClient();
     void closeConnection();
     /**
      *   Sends a response back to the Client
      */
     void sendResponse(const QString&);

private:
     enum requestMethod {NONE,GET,POST};
     bool m_disabled;
     QueryHandler* m_handler;
     QTcpSocket* m_socket;

     /**
      * Separate the XML body from the header and return body alone
      */
     QString retrieveXmlData(const QString&);

};
#endif
