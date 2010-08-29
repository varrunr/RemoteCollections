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

#ifndef QUERYHANDLER_H
#define QUERYHANDLER_H

#include "core/meta/Meta.h"
#include "core-impl/collections/support/XmlQueryReader.h"
#include<QObject>

class QueryHandler : public QObject
{
    Q_OBJECT

public:

    /**
        Creates an instance of QueryHandler initialized with an XML query
    */

    QueryHandler(const QString&);


signals:

    void convertedToXml(const QString&);
    void invalidQuery();

public slots:
    /**
        starts the Query handler
    */
    void start();
    /**
       Slots to receive results of the Query Maker
    */
    void slotResultReady(QString collectionId, Meta::ArtistList);
    void slotResultReady(QString collectionId, Meta::AlbumList);
    void slotResultReady(QString collectionId, Meta::TrackList);

    void queryDoneAction();
    void abortQuery();

private:

    Collections::QueryMaker* m_qm;
    QString m_xmlQuery;
    QString m_xmlResponse;


    void getXmlResponse();
};
#endif
