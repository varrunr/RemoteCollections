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

#include "RemoteCollectionService.h"
#include "RemoteCollection.h"
#include "core/support/Amarok.h"
#include "core/support/Components.h"
#include "core/interfaces/Logger.h"
#include "browsers/SingleCollectionTreeItemModel.h"
#include "core-impl/collections/support/CollectionManager.h"
#include <config-amarok.h>
#include "core/support/Debug.h"

#include <KStandardDirs>

#include"HttpService.h"
#include <QDomDocument>

AMAROK_EXPORT_PLUGIN( RemoteCollectionServiceFactory )

void RemoteCollectionServiceFactory::init()
{
    DEBUG_BLOCK
    //Starting the HTTP service at port 21212
    HttpService *RemoteService;
    RemoteService = new HttpService(quint16(21212));

    m_initialized = true;
    ServiceBase* service = new RemoteCollectionService( this, "RemoteCollection" );
    connect( service, SIGNAL( ready() ), this, SLOT( slotServiceReady() ) );
    emit newService( service );

}

QString
RemoteCollectionServiceFactory::name()
{
    DEBUG_BLOCK
    return "RemoteCollection";
}

KPluginInfo
RemoteCollectionServiceFactory::info()
{
    DEBUG_BLOCK
    KPluginInfo pluginInfo( "amarok_service_remotecollection.desktop", "services" );
    pluginInfo.setConfig( config() );
    return pluginInfo;
}

KConfigGroup
RemoteCollectionServiceFactory::config()
{
    DEBUG_BLOCK
    return Amarok::config( "Service_RemoteCollection" );
}

bool
RemoteCollectionServiceFactory::possiblyContainsTrack( const KUrl & url ) const
{
    DEBUG_BLOCK
    return false;
}


RemoteCollectionService::RemoteCollectionService( RemoteCollectionServiceFactory* parent, const QString & name )
    : ServiceBase( name,  parent )
    , m_authenticated( false )
    , m_server ( QString() )
    , m_sessionId ( QString() )
    , m_infoParser( 0 )
    , m_collection( 0 )
{

    DEBUG_BLOCK
    setShortDescription( i18n( "Service to access remote music collections" ) );
    setIcon( KIcon( "view-services-remote-amarok" ) );
    setLongDescription( i18n( "Service to access remote music collections" ) );
    setImagePath( KStandardDirs::locate( "data", "amarok/images/hover-info-remotecollections.png" ) );

}

RemoteCollectionService::~RemoteCollectionService()
{
    DEBUG_BLOCK
    CollectionManager::instance()->removeUnmanagedCollection( m_collection );
    delete m_collection;
}

void
RemoteCollectionService::polish()
{
    DEBUG_BLOCK
    m_bottomPanel->hide();
    setInfoParser( m_infoParser );

    QList<int> levels;
    levels << CategoryId::Artist;
    setModel( new SingleCollectionTreeItemModel( new Collections::RemoteCollection( this ), levels ) );
}

#include "RemoteCollectionService.moc"

