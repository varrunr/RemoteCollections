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

#include "RemoteCollection.h"

#include "RemoteCollectionQueryMaker.h"
#include "core/support/Debug.h"
#include <KLocale>
#include <threadweaver/ThreadWeaver.h>

using namespace Collections;

RemoteCollection::RemoteCollection( ServiceBase * service )
    : ServiceCollection( service, "RemoteCollection", "RemoteCollection" )
{
}

RemoteCollection::~RemoteCollection()
{
}

QueryMaker *
RemoteCollection::queryMaker()
{
    return new RemoteCollectionQueryMaker( this );
}

QString
RemoteCollection::collectionId() const
{
    return "Dummy Remote Collection";
}

QString
RemoteCollection::prettyName() const
{
    return i18n( "Dummy Remote Collection" );
}

bool
RemoteCollection::possiblyContainsTrack(const KUrl & url) const
{
    return false;
}


