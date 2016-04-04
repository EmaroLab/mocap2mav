/*******************************************************************************
 * NatNet v. 2.7 Motion Capture Receiver for Qt
 * Based on PacketClient by Optitrack  
 * 
 * Copyright (C) 2014-2015 Tommaso Falchi Delitala <volalto86@gmail.com>
 * 
 * This file may be used under the terms of the GNU General Public License 
 * versions 2.0 as published by the Free Software Foundation
 ******************************************************************************/

#ifndef MAVSTATE_H
#define MAVSTATE_H

#include <cmath>
#include <cstdint>
#include <conversions.h>
#include<math.h>




#define RAD2DEG 180.0/3.1415927


class MavState
{
public:
    //MavState(const MavState&) = default;

    inline void setPosition(float x, float y, float z)
    {
        _x = x;
        _y = y;
        _z = z;
    }

    inline void setX(float x)
    {
        _x = x;
    }

    inline void setY(float y)
    {
        _y = y;
    }

    inline void setZ(float z)
    {
        _z = z;
    }

    inline void setYaw(float yaw)
    {
        _yaw = yaw;
    }

    inline void setOrientation(float qw, float qx, float qy, float qz)
    {
        _qw = qw;
        _qx = qx;
        _qy = qy;
        _qz = qz;
    }

    inline void position(float* x, float* y, float* z)
    {
        *x = _x;
        *y = _y;
        *z = _z;
    }

    inline void orientation(float* qw, float* qx, float* qy, float* qz) const
    {
        *qw = _qw;
        *qx = _qx;
        *qy = _qy;
        *qz = _qz;
    }

    inline void orientation(float* roll, float* pitch, float* yaw) const
    {

        *roll = std::atan2(2*(_qw*_qx+_qy*_qz), 1-2*(_qx*_qx+_qy*_qy));
        *pitch = std::asin(2*(_qw*_qy - _qz*_qx));
        *yaw = std::atan2(2*(_qw*_qz+_qx*_qy), 1-2*(_qy*_qy + _qz*_qz));

    }

    inline void orientation2(float* roll, float* pitch, float* yaw) const
        {
            float quaternion[4] = {_qw ,_qx , _qy , _qz };
            float dcm[3][3];
            quaternion_to_dcm(quaternion , dcm);
            dcm_to_euler( dcm, roll, pitch, yaw);

        }

    inline float x() const
    {
        return _x;
    }

    inline float y() const
    {
        return _y;
    }

    inline float z() const
    {
        return _z;
    }

    inline float yaw() const
    {
        return _yaw;
    }

    inline float getYaw() const
    {
        return std::atan2(2*(_qw*_qz+_qx*_qy), 1-2*(_qy*_qy + _qz*_qz));
    }

    // OPERATORS

    void operator=(MavState &m){

        this->setPosition(m.x(),m.y(),m.z());

        this->setYaw(m.yaw());

        //XXX TODO orientation quaternion

    }


    // MOCAP
    long int timestamp; //"Latency" from motion capture server
    float _x = 0.0;
    float _y = 0.0;
    float _z = 0.0;
    float _yaw = 0.0;
    float _qw = 0.0;
    float _qx = 0.0;
    float _qy = 0.0;
    float _qz = 0.0;

    // Telemetry
    uint16_t rc[8];

    // Control
    uint16_t rc_override[8];
};

inline float rad2deg(float rad)
{
    return rad*RAD2DEG;
}


#endif // MAVSTATE_H

