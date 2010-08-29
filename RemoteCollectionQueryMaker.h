/****************************************************************************************
 * Copyright (c) 2007 - 2010 Nikolaj Hald Nielsen <nhn@kde.org>                         *
 * Copyright (c) 2010 Varrun Ramani <varrunr@gmail.com>                         *
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
#include "core-impl/collections/support/XmlQueryWriter.h"
#include <kio/jobclasses.h>
#include "QuerySender.h"

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
    virtual QueryMaker* setReturnResultAsDataPtrs ( bool resultAsDataPtrs );

    virtual QueryMaker* addReturnValue( qint64 value );
    virtual QueryMaker* addReturnFunction( ReturnFunction function, qint64 value );
    virtual QueryMaker* orderBy( qint64 value, bool descending = false );
    virtual QueryMaker* orderByRandom();

    QueryMaker* includeCollection( const QString &collectionId );
    QueryMaker* excludeCollection( const QString &collectionId );

    using DynamicServiceQueryMaker::addMatch;

    virtual QueryMaker* addMatch( const Meta::TrackPtr &track );
    virtual QueryMaker* addMatch( const Meta::ArtistPtr &artist );
    virtual QueryMaker* addMatch( const Meta::AlbumPtr &album );
    virtual QueryMaker* addMatch( const Meta::ComposerPtr &composer );
    virtual QueryMaker* addMatch( const Meta::GenrePtr &genre );
    virtual QueryMaker* addMatch( const Meta::YearPtr &year );
    virtual QueryMaker* addMatch( const Meta::DataPtr &data );
    virtual QueryMaker* addMatch( const Meta::LabelPtr &label );


    virtual QueryMaker* addFilter( qint64 value, const QString &filter, bool matchBegin = false, bool matchEnd = false );
    virtual QueryMaker* addNumberFilter( qint64 value, qint64 filter, QueryMaker::NumberComparison compare );

    virtual int validFilterMask();
    virtual QueryMaker* limitMaxResultSize( int size );

    virtual QueryMaker* setAlbumQueryMode( AlbumQueryMode mode );


  //  virtual QueryMaker* beginAnd();
  //  virtual QueryMaker* beginOr();
  //  virtual QueryMaker* endAndOr();

    //Methods "borrowed" from MemoryQueryMaker
    void handleResult();
    void handleResult( const Meta::TrackList &tracks );
    void handleResult( const Meta::ArtistList &artists );
    void handleResult( const Meta::AlbumList &albums );

    void fetchArtists();
    void fetchAlbums();
    void fetchTracks();

signals:
    void queryFormed(QString);

public slots:
    /**
        Update the remote collection with results
     */
    void updateArtists(const QStringList&);
    void updateAlbums(const QStringList&);
    void updateTracks(const QStringList&);

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

    XmlQueryWriter* XmlQm;

private:
    QuerySender* qSend;
    // Disable copy constructor and assignment
    RemoteCollectionQueryMaker( const RemoteCollectionQueryMaker& );
    RemoteCollectionQueryMaker& operator= ( const RemoteCollectionQueryMaker& );

    template<class PointerType, class ListType>
    void emitProperResult(const ListType& list);
};

} //namespace Collections

#endif
