/*******************************************************************************
 * NatNet v. 2.7 Motion Capture Receiver for Qt
 * Based on PacketClient by Optitrack  
 * 
 * Copyright (C) 2014-2015 Tommaso Falchi Delitala <volalto86@gmail.com>
 * 
 * This file may be used under the terms of the GNU General Public License 
 * versions 2.0 as published by the Free Software Foundation.
 ******************************************************************************/

#ifndef NATNETRECEIVER_H
#define NATNETRECEIVER_H

#include "MavState.h"
#include "global.h"
#include <QObject>
#include <QtNetwork>
#include <QByteArray>


//#include <conversions.h>


class NatNetReceiver : public QObject
{
    Q_OBJECT

public:
    explicit NatNetReceiver(QObject *parent = 0,
                            QString serverAddress = QString("239.255.42.99"),
                            quint16 dataPort = 1511);
    ~NatNetReceiver();

signals:
    void dataUpdate();

private slots:
    void readData();

private:
    QUdpSocket _cmd_sock;
    QUdpSocket _data_sock;
    QHostAddress _serverAddr;

    QByteArray _buffer;
    void _decodeData();

};

#endif // NATNETRECEIVER_H
