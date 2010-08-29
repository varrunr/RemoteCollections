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

#include "QueryHandler.h"
#include "core/support/Debug.h"
#include<QTimer>
#include<QString>
#include<QDomDocument>

QueryHandler::QueryHandler(const QString &xmlQuery)
    :m_xmlQuery(xmlQuery)
    ,m_xmlResponse("No Response")
{
    DEBUG_BLOCK
}

void
QueryHandler::start()
{
    DEBUG_BLOCK
    getXmlResponse();
}

void
QueryHandler::getXmlResponse()
{
    DEBUG_BLOCK

    if( m_xmlQuery == "" )
    {
        debug() << "Please pass a query";
        emit invalidQuery();
        return;
    }

    debug() << m_xmlQuery;
    m_qm = XmlQueryReader::getQueryMaker(m_xmlQuery, XmlQueryReader::ParseReturnValues);

    if( !m_qm )
    {
       debug() << "Invalid XML query: \n" << m_xmlQuery;
       emit invalidQuery();
       return;
    }

    m_qm->setAutoDelete( true );
    m_qm->run();

    connect( m_qm, SIGNAL( newResultReady( QString, Meta::ArtistList ) ),
            this, SLOT( slotResultReady( QString, Meta::ArtistList ) ),
            Qt::DirectConnection);

    connect( m_qm, SIGNAL( newResultReady( QString, Meta::AlbumList ) ),
            this, SLOT( slotResultReady( QString, Meta::AlbumList ) ),
            Qt::DirectConnection);

    connect( m_qm, SIGNAL( newResultReady( QString, Meta::TrackList ) ),
            this, SLOT( slotResultReady( QString, Meta::TrackList ) )
            ,Qt::DirectConnection);

    connect( m_qm, SIGNAL( queryDone() ),this, SLOT( queryDoneAction())
            ,Qt::DirectConnection);

    //abort query after 15 seconds in case the query does not return
    //QTimer::singleShot( 15000, this, SLOT( abortQuery() ) );

}

void
QueryHandler::slotResultReady(QString collectionId, Meta::ArtistList artists)
{
    DEBUG_BLOCK
    Q_UNUSED(collectionId);
    if(!artists.isEmpty())
    {
        QDomDocument doc("response");

        QDomElement root = doc.createElement("result");
        root.setAttribute("type","artists");
        doc.appendChild(root);

        Q_UNUSED(collectionId);

            int id = 0;

            foreach(const Meta::ArtistPtr &artist, artists )
            {
                QString artistName = artist.data()->prettyName();

                id++;

                QDomElement artistNode = doc.createElement("artist");
                artistNode.setAttribute( "id",(id) );

                QDomElement tag = doc.createElement( "name");
                artistNode.appendChild( tag );

                root.appendChild(artistNode);

                QDomText t = doc.createTextNode(artistName);
                tag.appendChild(t);

            }

        m_xmlResponse = doc.toString();
        debug() << m_xmlResponse;
        emit convertedToXml(m_xmlResponse);
        deleteLater();
        disconnect(sender(),0,this,SLOT(slotResultReady(QString,Meta::ArtistList)));
    }

}

void
QueryHandler::slotResultReady( QString collectionId, Meta::AlbumList albums)
{
    DEBUG_BLOCK
    if(!albums.isEmpty())
    {
        QDomDocument doc("response");
        QDomElement root = doc.createElement("result");
        root.setAttribute("type","albums");
        doc.appendChild(root);

        Q_UNUSED(collectionId);
        if(!albums.isEmpty())
        {
            int id = 0;

            foreach(const Meta::AlbumPtr &album, albums )
            {
                QString albumName = album.data()->prettyName();

                id++;

                QDomElement albumNode = doc.createElement("album");
                albumNode.setAttribute( "id",(id) );

                QDomElement tag = doc.createElement( "name");
                albumNode.appendChild( tag );

                root.appendChild(albumNode);

                QDomText t = doc.createTextNode(albumName);
                tag.appendChild(t);
            }
        }

        m_xmlResponse = doc.toString();
        debug() << m_xmlResponse;
        emit convertedToXml(m_xmlResponse);
        deleteLater();
        disconnect( sender(),0,this,SLOT(slotResultReady(QString,Meta::AlbumList)));
    }

}
void
QueryHandler::slotResultReady( QString collectionId, Meta::TrackList tracks )
{
    DEBUG_BLOCK
    if(!tracks.isEmpty())
    {
        QDomDocument doc("response");
        QDomElement root = doc.createElement("result");
        root.setAttribute("type","tracks");
        doc.appendChild(root);

        Q_UNUSED(collectionId);
        if(!tracks.isEmpty())
        {
            int id = 0;

            foreach(const Meta::TrackPtr &track, tracks )
            {
                QString trackName = track.data()->prettyName();

                id++;

                QDomElement trackNode = doc.createElement("track");
                trackNode.setAttribute( "id",(id) );

                QDomElement tag = doc.createElement( "name");
                trackNode.appendChild( tag );

                root.appendChild(trackNode);

                QDomText t = doc.createTextNode(trackName);
                tag.appendChild(t);
            }
        }

        m_xmlResponse = doc.toString();
        debug() << m_xmlResponse;
        emit convertedToXml(m_xmlResponse);
        deleteLater();
    }
}

void
QueryHandler::queryDoneAction()
{
    DEBUG_BLOCK
    deleteLater();
}

void
QueryHandler::abortQuery()
{
    DEBUG_BLOCK
    deleteLater();
}
