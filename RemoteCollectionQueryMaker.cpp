/****************************************************************************************
 * Copyright (c) 2007 - 2010 Nikolaj Hald Nielsen <nhn@kde.org>                         *
 * Copyright (c) 2007 Adam Pigg <adam@piggz.co.uk>                                      *
 * Copyright (c) 2007 Casey Link <unnamedrambler@gmail.com>                             *
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

#include "RemoteCollectionQueryMaker.h"

#include "core/support/Amarok.h"
#include "core/support/Debug.h"
#include<KUrl>
#include "HttpService.h"
#include<QDomDocument>
#include "QueryHandler.h"
#include "RemoteCollectionMeta.h"
#include "core-impl/collections/support/MemoryMatcher.h"

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

    qSend = new QuerySender();

    connect(this, SIGNAL(queryFormed(QString)), qSend, SLOT(sendRequest(QString)),
            Qt::DirectConnection);
    connect(qSend,SIGNAL(artistParsingDone(const QStringList&)),this,SLOT(updateArtists(const QStringList&)),
            Qt::QueuedConnection);

    //Create the QueryWriter
    XmlQm = new XmlQueryWriter(CollectionManager::instance()->primaryCollection()->queryMaker(), QDomDocument());
    reset();
}

RemoteCollectionQueryMaker::~RemoteCollectionQueryMaker()
{
    DEBUG_BLOCK
    delete d;
}

QueryMaker *
RemoteCollectionQueryMaker::reset()
{
    DEBUG_BLOCK
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
    DEBUG_BLOCK
    d->returnDataPtrs = resultAsDataPtrs;
    XmlQm->setReturnResultAsDataPtrs(resultAsDataPtrs );
    return this;
}

QueryMaker*
RemoteCollectionQueryMaker::addReturnValue( qint64 value )
{
    DEBUG_BLOCK
    XmlQm->addReturnValue(value);
    return this;
}

QueryMaker*
RemoteCollectionQueryMaker::addReturnFunction( ReturnFunction function, qint64 value )
{
    DEBUG_BLOCK
    XmlQm->addReturnFunction(function,value );
    return this;
}

QueryMaker*
RemoteCollectionQueryMaker::orderBy( qint64 value, bool descending)
{
    DEBUG_BLOCK
    XmlQm->orderBy(value,descending);
    return this;
}

QueryMaker*
RemoteCollectionQueryMaker::orderByRandom()
{
    DEBUG_BLOCK
    XmlQm->orderByRandom();
    return this;
}

void
RemoteCollectionQueryMaker::run()
{
    DEBUG_BLOCK
    //Create Query XML
    QString xmlQuery = XmlQm->getXml();
    debug() << "XML QUERY FORMED m:"<<xmlQuery << "\n";

    emit queryFormed(xmlQuery);

}
void
RemoteCollectionQueryMaker::updateAlbums(const QStringList &albumNameList)
{
    Q_UNUSED(albumNameList);
    //Parse Album stuff here
}

void
RemoteCollectionQueryMaker::updateTracks(const QStringList &trackNameList)
{
    Q_UNUSED(trackNameList);
    //parse Track Stuff here
}

void
RemoteCollectionQueryMaker::updateArtists(const QStringList &artistNameList)
{
    DEBUG_BLOCK
    Meta::ArtistList artists;
    Meta::ServiceArtist * artist;
    QStringList::const_iterator artistIterator;
    for (artistIterator = artistNameList.constBegin(); artistIterator != artistNameList.constEnd()
                                            ; ++artistIterator)
    {
        QString artistName = (*artistIterator).toLocal8Bit().constData();
        artist = new Meta::RemoteArtist( artistName, m_collection->service() );

        Meta::ArtistPtr artistPtr( artist );
        artists.push_back(artistPtr);

        m_collection->acquireWriteLock();
        m_collection->addArtist( artistPtr );
        m_collection->releaseLock();
    }
    handleResult(artists);
    emit queryDone();
}

void
RemoteCollectionQueryMaker::abortQuery()
{

}

QueryMaker *
RemoteCollectionQueryMaker::setQueryType( QueryType type )
{
    DEBUG_BLOCK

    XmlQm->setQueryType(type);
    debug() << "type:"<<type;
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

QueryMaker*
RemoteCollectionQueryMaker::includeCollection( const QString &collectionId )
{
    DEBUG_BLOCK
    XmlQm->includeCollection(collectionId);
    return this;
}

QueryMaker*
RemoteCollectionQueryMaker::excludeCollection( const QString &collectionId )
{
    DEBUG_BLOCK
    XmlQm->excludeCollection(collectionId);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::ArtistPtr & artist )
{
    DEBUG_BLOCK
    debug() << "Add Match Artist";
    XmlQm->addMatch(artist);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::AlbumPtr & album )
{
    DEBUG_BLOCK
    debug() << "Add Match Track";
    XmlQm->addMatch(album);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::TrackPtr & track )
{
    DEBUG_BLOCK
    debug() << "Add Match Track";
    XmlQm->addMatch(track);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::ComposerPtr & composer )
{
    DEBUG_BLOCK
    XmlQm->addMatch(composer);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::GenrePtr & genre )
{
    DEBUG_BLOCK
    XmlQm->addMatch(genre);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::YearPtr & year )
{
    DEBUG_BLOCK
    XmlQm->addMatch(year);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::DataPtr & data )
{
    DEBUG_BLOCK
    debug() << "Add Match data";
    XmlQm->addMatch(data);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addMatch( const Meta::LabelPtr & label )
{
    DEBUG_BLOCK
    XmlQm->addMatch(label);
    return this;
}

template<class PointerType, class ListType>
void RemoteCollectionQueryMaker::emitProperResult( const ListType& list )
{
    DEBUG_BLOCK
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

QueryMaker* RemoteCollectionQueryMaker::setAlbumQueryMode( AlbumQueryMode mode )
{
    DEBUG_BLOCK
    XmlQm->setAlbumQueryMode(mode);
    return this;
}

QueryMaker *
RemoteCollectionQueryMaker::addFilter( qint64 value, const QString & filter, bool matchBegin, bool matchEnd )
{
    DEBUG_BLOCK
    Q_UNUSED( matchBegin )
    Q_UNUSED( matchEnd )
    XmlQm->addFilter(value, filter, matchBegin, matchEnd);
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
    XmlQm->addNumberFilter(value, filter, compare );
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
    DEBUG_BLOCK
    //we only supprt artist filters for now...
    return ArtistFilter;
}

QueryMaker *
RemoteCollectionQueryMaker::limitMaxResultSize( int size )
{
    DEBUG_BLOCK
    XmlQm->limitMaxResultSize(size);
    d->maxsize = size;
    return this;
}

#include "RemoteCollectionQueryMaker.moc"

