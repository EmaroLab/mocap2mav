#include <QtSerialPort/QtSerialPort>
#include <iostream>
#include "PositionDispatcher.h"


#define DISPATCH_INTERVAL 100 //ms



PositionDispatcher::PositionDispatcher(QObject *parent) :
    QObject(parent),
    _serial(this)
{
    _serial.setPortName("/dev/ttyUSB0");

    // Open serial port

    if(!_serial.open(QSerialPort::ReadWrite)) {

        qCritical() << "Error opening serial port";
        emit finished();
        return;
    }

    _serial.setBaudRate(QSerialPort::Baud57600);
    _serial.setFlowControl(QSerialPort::NoFlowControl);

    // Start dispatch time counter
    _dispatchTime.start();

    qDebug() << "Connected to MavLink Radio...";
}

PositionDispatcher::~PositionDispatcher()
{
    _serial.close();
    qDebug() << "Serial Port closed";
}


void PositionDispatcher::sendPosition(int64_t ts, geometry::pose vision, geometry::pose sp, bool visionValid, bool spValid)
{

    mavlink_message_t msg1;
    mavlink_message_t msg2;

    mavlink_msg_vision_position_estimate_pack(
            1,
            MAV_COMP_ID_ALL, &msg1,
            (uint64_t) ts * 1000,
            (float)vision.position[0],
            (float)vision.position[1],
            (float)vision.position[2],
            0, //rad
            0, //rad
            0); //rad

    mavlink_msg_vicon_position_estimate_pack(
            1,
            MAV_COMP_ID_ALL, &msg2,
            (uint64_t) ts * 1000,
            (float)sp.position[0],
            (float)sp.position[1],
            (float)sp.position[2],
            0, //rad
            0, //rad
            0); //rad

    if(_dispatchTime.elapsed() >= DISPATCH_INTERVAL){

       // if(visionValid) {
            _sendMavlinkMessage(&msg1);
            std::cout << "Vision estimate: " << vision.position[0] << " " << vision.position[1] <<
            " " << vision.position[2] << std::endl;
        //}

        if(spValid){
            _sendMavlinkMessage(&msg2);
            std::cout << "Position SP: " << sp.position[0] << " " <<sp.position[1] <<
            " " << sp.position[2] << std::endl;
        }

        _dispatchTime.restart();

    }

}

void PositionDispatcher::_sendMavlinkMessage(mavlink_message_t *msg)
{
    // Initialize the required buffers
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    // Copy the message to the send buffer
    uint16_t len = mavlink_msg_to_send_buffer(buf, msg);

    // Send the message
    QByteArray sendBuff((const char*)buf, len);
    int written = _serial.write(sendBuff);

    //Wait for data to be sent
    if(!_serial.waitForBytesWritten(500)) {
        qCritical() << "Serial unable to send data";
        emit finished();
    }

}
