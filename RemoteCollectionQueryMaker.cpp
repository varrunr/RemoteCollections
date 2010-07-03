/****************************************************************************************
 * Copyright (c) 2007 - 2010 Nikolaj Hald Nielsen <nhn@kde.org>                         *
 * Copyright (c) 2007 Adam Pigg <adam@piggz.co.uk>                                      *
 * Copyright (c) 2007 Casey Link <unnamedrambler@gmail.com>                             *
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

#include "RemoteCollectionQueryMaker.h"

#include "core/support/Amarok.h"
#include "core/support/Debug.h"

using namespace Collections;

struct RemoteCollectionQueryMaker::Private
{
    enum QueryType { NONE, TRACK, ARTIST, ALBUM, COMPOSER, YEAR, GENRE, CUSTOM };
    QueryType type;
    int maxsize;
    bool returnDataPtrs;
};

RemoteCollectionQueryMaker::RemoteCollectionQueryMaker( RemoteCollection * collection )
    : DynamicServiceQueryMaker()
    , m_collection( collection )
    , m_storedTransferJob( 0 )
    , d( new Private )
    , m_dateFilter( -1 )
{
    DEBUG_BLOCK
    m_collection = collection;
    reset();
}

RemoteCollectionQueryMaker::~RemoteCollectionQueryMaker()
{
    delete d;
}

QueryMaker *
RemoteCollectionQueryMaker::reset()
{
    d->type = Private::NONE;
    d->maxsize = 0;
    d->returnDataPtrs = false;
    m_parentArtistId.clear();
    m_parentAlbumId.clear();
    m_artistFilter.clear();
    m_dateFilter = 0;
    //m_lastArtistFilter = QString(); this one really should survive a reset....

    return this;
}

QueryMaker*
RemoteCollectionQueryMaker::setReturnResultAsDataPtrs( bool resultAsDataPtrs )
{
    d->returnDataPtrs = resultAsDataPtrs;
    return this;
}

void
RemoteCollectionQueryMaker::run()
{
    DEBUG_BLOCK

    //TODO: create xml stuff here!!
}

void
RemoteCollectionQueryMaker::abortQuery()
{
}

QueryMaker *
RemoteCollectionQueryMaker::setQueryType( QueryType type )
{
    DEBUG_BLOCK
    switch( type ) {

    case QueryMaker::Artist:
        d->type = Private::ARTIST;
        return this;

    case QueryMaker::Album:
        d->type = Private::ALBUM;
        return this;

    case QueryMaker::Track:
        d->type = Private::TRACK;
        return this;

    case QueryMaker::Genre:
    case QueryMaker::Composer:
    case QueryMaker::Year:
    case QueryMaker::Custom:
    case QueryMaker::Label:
    case QueryMaker::None:
        //TODO: Implement.
        return this;
    }

    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::ArtistPtr & artist )
{
    DEBUG_BLOCK
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::AlbumPtr & album )
{
    DEBUG_BLOCK
    return this;
}

template<class PointerType, class ListType>
void RemoteCollectionQueryMaker::emitProperResult( const ListType& list )
{
    if ( d->returnDataPtrs ) {
        Meta::DataList data;
        foreach( PointerType p, list )
            data << Meta::DataPtr::staticCast( p );

        emit newResultReady( m_collection->collectionId(), data );
    }
    else
        emit newResultReady( m_collection->collectionId(), list );
}

void RemoteCollectionQueryMaker::handleResult()
{
    DEBUG_BLOCK
}

void RemoteCollectionQueryMaker::handleResult( const Meta::ArtistList & artists )
{
    DEBUG_BLOCK

    emitProperResult<Meta::ArtistPtr, Meta::ArtistList>( artists );
}

void
RemoteCollectionQueryMaker::handleResult( const Meta::AlbumList &albums )
{
    DEBUG_BLOCK

    emitProperResult<Meta::AlbumPtr, Meta::AlbumList>( albums );
}

void
RemoteCollectionQueryMaker::handleResult( const Meta::TrackList & tracks )
{
    DEBUG_BLOCK

    emitProperResult<Meta::TrackPtr, Meta::TrackList>( tracks );
}



QueryMaker *
RemoteCollectionQueryMaker::addFilter( qint64 value, const QString & filter, bool matchBegin, bool matchEnd )
{
    DEBUG_BLOCK
    Q_UNUSED( matchBegin )
    Q_UNUSED( matchEnd )

    //debug() << "value: " << value;
    //for now, only accept artist filters
    if ( value == Meta::valArtist )
    {
        //debug() << "Filter: " << filter;
        m_artistFilter = filter;
    }
    return this;
}

QueryMaker*
RemoteCollectionQueryMaker::addNumberFilter( qint64 value, qint64 filter, QueryMaker::NumberComparison compare )
{
    DEBUG_BLOCK

    if( value == Meta::valCreateDate && compare == QueryMaker::GreaterThan )
    {
        debug() << "asking to filter based on added date";
        m_dateFilter = filter;
        debug() << "setting dateFilter to:" << m_dateFilter;
    }
    return this;
}


int
RemoteCollectionQueryMaker::validFilterMask()
{
    //we only supprt artist filters for now...
    return ArtistFilter;
}

QueryMaker *
RemoteCollectionQueryMaker::limitMaxResultSize( int size )
{
    d->maxsize = size;
    return this;
}

#include "RemoteCollectionQueryMaker.moc"

