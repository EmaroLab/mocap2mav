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
    //_serial.setReadBufferSize(MAVLINK_MAX_PACKET_LEN);

    // Start dispatch time counter
    _dispatchTime.start();
    qDebug() << "Connected to MavLink Radio...";
}

PositionDispatcher::~PositionDispatcher()
{
    _serial.close();
    qDebug() << "Serial Port closed";
}


void PositionDispatcher::sendPosition(int64_t ts, MavState vision, MavState sp, bool visionValid, bool spValid)
{

    mavlink_message_t msg1;
    mavlink_message_t msg2;

    float roll,pitch,yaw;
    vision.orientation(&roll,&pitch,&yaw);

    mavlink_msg_vision_position_estimate_pack(
            1,
            MAV_COMP_ID_ALL, &msg1,
            (uint64_t) ts * 1000,
            vision.getX(),
            vision.getY(),
            vision.getZ(),
            roll, //rad
            pitch, //rad
            yaw); //rad

   mavlink_msg_set_position_target_local_ned_pack(
           1,
           MAV_COMP_ID_ALL,
           &msg2,
           (uint64_t) ts * 1000,
           1,
           MAV_COMP_ID_ALL,
           MAV_FRAME_LOCAL_NED,
           0b0000001111111000,
           sp.getX(),   //x        [m]
           sp.getY(),   //y
           sp.getZ(),   //z        Be careful, z is directed downwards!!
           0,           //vx       [m/s]
           0,           //vy
           0,           //vz
           0,           //ax       [m/s^2]
           0,           //ay
           0,           //az
           0,           //yaw      [rad]
           0);          //yaw_rate [rad/s]



    if(_dispatchTime.elapsed() >= DISPATCH_INTERVAL){

       // if(visionValid) {
            _sendMavlinkMessage(&msg1);
            std::cout << "Vision estimate: " << vision.getX() << " " << vision.getY() <<
            " " << vision.getZ() <<" \\ " <<roll <<" " <<pitch <<" " <<" " <<yaw << std::endl;
        //}

        if(spValid){
            _sendMavlinkMessage(&msg2);
            std::cout << "Position SP: " << sp.getX() << " " <<sp.getY() <<
            " " << sp.getZ() << std::endl;

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
