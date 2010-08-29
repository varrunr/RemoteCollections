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

#ifndef QUERYSENDER_H
#define QUERYSENDER_H

#include<QNetworkAccessManager>
#include<QObject>

class QuerySender : public QObject
{
    Q_OBJECT;

public:

    QuerySender();

public slots:

    /**
     *   Send a POST request to the remote server
     *   @param XML query to send
     */
    void sendRequest(QString);

    void replyFinished(QNetworkReply*);

signals:

    void replyReceived();
    void artistParsingDone(const QStringList&);
    void albumParsingDone(const QStringList&);
    void trackParsingDone(const QStringList&);

private:

    void initNetworkParams();
    void startNetworkManager();

    /**
     *   Parse resulting XML response from remote server into
     *   a list of artists/albums/tracks
     */
    QStringList parseArtist(const QString&);
    QStringList parseAlbum(const QString&);
    QStringList parseTrack(const QString&);


    QNetworkAccessManager* m_manager;
    QString m_host;
    QString m_protocol;
    QString m_port;
    QString m_url;

};
#endif
