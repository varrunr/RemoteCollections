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

#ifndef AMPACHESERVICEQUERYMAKER_H
#define AMPACHESERVICEQUERYMAKER_H

#include "DynamicServiceQueryMaker.h"

#include "RemoteCollection.h"
#include "RemoteCollectionService.h"

#include <kio/jobclasses.h>

namespace ThreadWeaver
{
    class Job;
}

namespace Collections {

class RemoteCollectionQueryMaker : public DynamicServiceQueryMaker
{
    Q_OBJECT

public:
    RemoteCollectionQueryMaker( RemoteCollection * collection );
    ~RemoteCollectionQueryMaker();

    virtual QueryMaker* reset();
    virtual void run();
    virtual void abortQuery();

    virtual QueryMaker* setQueryType( QueryType type );

    using DynamicServiceQueryMaker::addMatch;
    virtual QueryMaker* addMatch ( const Meta::ArtistPtr &artist );
    virtual QueryMaker* addMatch ( const Meta::AlbumPtr &album );

    virtual QueryMaker* setReturnResultAsDataPtrs ( bool resultAsDataPtrs );

    virtual QueryMaker* addFilter( qint64 value, const QString &filter, bool matchBegin = false, bool matchEnd = false );
    virtual QueryMaker* addNumberFilter( qint64 value, qint64 filter, QueryMaker::NumberComparison compare );

    virtual int validFilterMask();
    virtual QueryMaker* limitMaxResultSize( int size );

    //Methods "borrowed" from MemoryQueryMaker
    void handleResult();
    void handleResult( const Meta::TrackList &tracks );
    void handleResult( const Meta::ArtistList &artists );
    void handleResult( const Meta::AlbumList &albums );

    void fetchArtists();
    void fetchAlbums();
    void fetchTracks();

protected:
    RemoteCollection * m_collection;
    KIO::StoredTransferJob * m_storedTransferJob;

    struct Private;
    Private * const d;

    QString m_server;
    QString m_sessionId;
    QString m_parentAlbumId;
    QString m_parentArtistId;
    QString m_parentService;
    
    uint m_dateFilter;
    QString m_artistFilter;
    QString m_lastArtistFilter;

public slots:

private:
    // Disable copy constructor and assignment
    RemoteCollectionQueryMaker( const RemoteCollectionQueryMaker& );
    RemoteCollectionQueryMaker& operator= ( const RemoteCollectionQueryMaker& );

    template<class PointerType, class ListType>
    void emitProperResult(const ListType& list);
};

} //namespace Collections

#endif
