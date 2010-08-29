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

#ifndef REMOTE_COLLECTION_META_H
#define REMOTE_COLLECTION_META_H

#include "ServiceBase.h"
#include "ServiceMetaBase.h"
#include "ServiceAlbumCoverDownloader.h"

#include <KStandardDirs>

#include <QDateTime>
#include <QList>
#include <QString>
#include <QStringList>


namespace Meta
{

class RemoteTrack  : public ServiceTrack
{

public:
    explicit RemoteTrack( const QString& title, ServiceBase * service = 0 )
        : ServiceTrack( title )
        , m_service( service )
    {}

    virtual QString sourceName() { return "Remote"; }
    virtual QString sourceDescription() { return "The Remote music collections project"; }
    virtual QPixmap emblem()  { return QPixmap( KStandardDirs::locate( "data", "amarok/images/emblem-ampache.png" ) );  }
    virtual QString scalableEmblem()  { return  KStandardDirs::locate( "data", "amarok/images/emblem-ampache-scalable.svgz" );  }

    virtual QList< QAction *> currentTrackActions();

private:
    ServiceBase * m_service;
};


class RemoteAlbum  : public ServiceAlbumWithCover
{
private:
    QString m_coverURL;

public:
    RemoteAlbum( const QString &name );
    RemoteAlbum( const QStringList &resultRow );

    ~RemoteAlbum();

    virtual QString downloadPrefix() const { return "Remote"; }

    virtual void setCoverUrl( const QString &coverURL );
    virtual QString coverUrl() const;

    bool operator==( const Meta::Album &other ) const
    {
        return name() == other.name();
    }

};

class RemoteArtist : public ServiceArtist
{
    private:
        QString m_coverURL;

    public:
        RemoteArtist( const QString &name, ServiceBase * service )
            : ServiceArtist( name )
            , m_service( service )
        {}

        virtual bool isBookmarkable() const { return true; }
        virtual QString collectionName() const { return m_service->name(); }
        virtual bool simpleFiltering() const { return true; }

        bool operator==( const Meta::Artist &other ) const
        {
            return name() == other.name();
        }

    private:
        ServiceBase * m_service;
};

}

#endif  // End include guard
