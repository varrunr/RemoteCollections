/****************************************************************************************
 * Copyright (c) 2007 - 2010 Nikolaj Hald Nielsen <nhn@kde.org>                         *
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

#ifndef AMPACHESERVICECOLLECTION_H
#define AMPACHESERVICECOLLECTION_H

#include <ServiceCollection.h>

namespace Collections {

/**
A collection that dynamically fetches data from a remote location as needed

	@author
*/
class RemoteCollection : public ServiceCollection
{
    Q_OBJECT

public:
    RemoteCollection( ServiceBase * service );

    virtual ~RemoteCollection();

    virtual QueryMaker* queryMaker();

    virtual QString collectionId() const;
    virtual QString prettyName() const;

    virtual Meta::TrackPtr trackForUrl( const KUrl &url ) {return Meta::TrackPtr();}
    virtual bool possiblyContainsTrack( const KUrl &url ) const;

private:
    /*void parseAlbum( const QString &xml );
    void parseArtist( const QString &xml );*/

    QString m_server;
    QString m_sessionId;

};

} //namespace Collections

#endif
