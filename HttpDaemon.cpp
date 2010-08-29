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

#include "HttpDaemon.h"
#include<QTcpServer>
#include<QTcpSocket>
#include<QStringList>
#include<QRegExp>
#include<QTextStream>
#include<QDateTime>
#include "core/support/Debug.h"
#include<QObject>

HttpDaemon::HttpDaemon(quint16 port, QObject* parent)
:QTcpServer(parent), m_disabled(false)
{
    DEBUG_BLOCK
    listen(QHostAddress::LocalHost, port);
}

void
HttpDaemon::pause()
{
    m_disabled = true;
}

void
HttpDaemon::resume()
{
    m_disabled = false;
}

void
HttpDaemon::incomingConnection(int socket)
{
    DEBUG_BLOCK
    if (m_disabled)
    return;

    m_socket = new QTcpSocket(this);
    m_socket->setSocketDescriptor(socket);
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(discardClient()));
    debug() << "New Connection";

}

void
HttpDaemon::sendResponse(const QString &response)
{
    DEBUG_BLOCK

    if(m_socket->isValid())
        debug() << "valid";
    else debug() << "invalid";

    //Send Response Back to Client
    QTextStream os(m_socket);
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 Ok\r\n"
          "Content-Type: text/xml; charset=\"utf-8\"\r\n"
          "\r\n"
       << response;

    //Close socket
    m_socket->close();

    if (m_socket->state() == QTcpSocket::UnconnectedState)
    {
        delete m_socket;
        debug() << "Connection closed";
    }

}

QString
HttpDaemon::retrieveXmlData(const QString &postData)
{
    // "\r\n\r\n" is used to separate header and body
    return (postData.split("\r\n\r\n"))[1];
}

void
HttpDaemon::closeConnection()
{
    DEBUG_BLOCK
    if (m_socket->state() == QTcpSocket::UnconnectedState)
    {
        delete m_socket;
        debug() << "Connection closed";
    }
}


void
HttpDaemon::readClient()
{
    DEBUG_BLOCK
    if (m_disabled)
    return;

    // This slot is called when the client sent data to the server
    QString xmlQuery;
    requestMethod method = NONE;
    if (m_socket->canReadLine())
    {
        QStringList tokens = QString(m_socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));

        if(tokens[0] == "POST")
        {
            method = POST;
            debug() << "RECIEVED REQUEST \n";

            if(m_socket->canReadLine())
            {
                xmlQuery = retrieveXmlData(QString(m_socket->readAll()));

            }
        }
        else if(tokens[0] == "GET")
        {
            method = GET;
        }
        else method = NONE;
    }

    if(method != POST)
    {
        debug() << "Invalid request Type";
        return;
    }

     m_handler = new QueryHandler(xmlQuery);

    //Start Query Handler once request is recieved
    connect(this, SIGNAL(startQueryConversion()), m_handler, SLOT(start()),Qt::DirectConnection);

    //Close Connection in case of an invalid Query
    connect(m_handler, SIGNAL(invalidQuery()),this, SLOT(closeConnection()));

    //Once the Reponse has been encoded into XML recieve result in slotSendResponse()
    connect(m_handler, SIGNAL(convertedToXml(const QString&)),this, SLOT(sendResponse(const QString&))
            ,Qt::QueuedConnection);

    emit startQueryConversion();
}

void
HttpDaemon::discardClient()
{
    DEBUG_BLOCK
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

    debug() << "Connection closed";
}
