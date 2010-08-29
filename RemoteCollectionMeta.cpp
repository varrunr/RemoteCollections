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

#include "RemoteCollectionMeta.h"
#include "core/support/Debug.h"


using namespace Meta;

//// RemoteAlbum ////

RemoteAlbum::RemoteAlbum( const QString &name )
    : ServiceAlbumWithCover( name )
{}

RemoteAlbum::RemoteAlbum(const QStringList & resultRow)
    : ServiceAlbumWithCover( resultRow )
{}

RemoteAlbum::~ RemoteAlbum()
{}

void RemoteAlbum::setCoverUrl( const QString &coverURL )
{
    m_coverURL = coverURL;
}

QString RemoteAlbum::coverUrl( ) const
{
    return m_coverURL;
}

QList< QAction * > Meta::RemoteTrack::currentTrackActions()
{
    QList< QAction * > actions;
    return actions;
}

