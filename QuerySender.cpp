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

#include "QuerySender.h"
#include<QUrl>
#include "core/support/Debug.h"
#include<QNetworkRequest>
#include<QNetworkReply>
#include<QDomDocument>
#include "network/NetworkAccessManagerProxy.h"
#include<QStringList>


QuerySender::QuerySender()   
{
    initNetworkParams();
    startNetworkManager();
}

void
QuerySender::initNetworkParams()
{
    //Hard coded temporarily. To be taken from global config later
    m_host = "localhost";
    m_protocol = "http";
    m_port = "21212";

}

void
QuerySender::startNetworkManager()
{
    DEBUG_BLOCK
    m_manager = new QNetworkAccessManager();

}

void QuerySender::sendRequest(QString xmlQuery)
{
    DEBUG_BLOCK
    m_url = (m_protocol + "://" + m_host + ":" + m_port) ;
    debug() << "Request Url:" << m_url;

    //Create Request
    QNetworkRequest request( QUrl(m_protocol + "://" + m_host + ":" + m_port) );
    request.setRawHeader("User-Agent", "Amarok Client");
    request.setRawHeader("Content-type","text/xml");

    //Send Request
    connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkReply *reply = m_manager->post(request,xmlQuery.toAscii());

}

void
QuerySender::replyFinished(QNetworkReply* reply)
{
    DEBUG_BLOCK

    QString xmlBody = QString(reply->readAll());
    emit artistParsingDone(parseArtist(xmlBody));

}

QStringList
QuerySender::parseArtist(const QString &xml)
{
    DEBUG_BLOCK
    debug() << xml;

        debug() << "artists";
        QDomDocument doc( "response" );
        doc.setContent( xml );

        QDomNode root = doc.documentElement();
        QDomNodeList artistNodeList = doc.elementsByTagName("artist");

        int nodeCount = artistNodeList.count();
        QDomNode* artistNode;
        QStringList artistNameList;

        if(!artistNodeList.isEmpty())
        {
            for(int i=0;i<nodeCount;i++)
            {
                artistNode = &(artistNodeList.at(i));
                QString artistName = (artistNode->toElement()).text();
                artistNameList << artistName ;
                debug() << artistName << "\n";
            }
        }
        artistNameList.removeDuplicates();
        return artistNameList;
}

