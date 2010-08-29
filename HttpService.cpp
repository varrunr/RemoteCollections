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

#include "HttpService.h"
#include "core/support/Debug.h"

HttpService::HttpService(const quint16 port)
    :m_port(port)
{
    DEBUG_BLOCK
    start();
}

void
HttpService::start()
{
    DEBUG_BLOCK
    m_daemon = new HttpDaemon(m_port);

     if (!m_daemon->isListening()) {
         debug() << "Failed to bind to port " << m_port;
     }
}

void
HttpService::pause()
{
    m_daemon->pause();
}

void
HttpService::resume()
{
    m_daemon->resume();
}
