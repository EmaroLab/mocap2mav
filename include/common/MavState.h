//
// Created by andreanisti on 15/11/16.
//

#ifndef MOCAP2MAV_MAVSTATE_H
#define MOCAP2MAV_MAVSTATE_H

#include <cmath>
#include <cstdint>
#include <math.h>
#include <Eigen/Eigen>

class MavState {

public:
    //MavState(const MavState&) = default;
    MavState();
    void setPosition(double x, double y, double z);
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    double getX() const;
    double getY() const;
    double getZ() const;

    void setV(double vx, double vy, double vz);
    double getVx() const;
    double getVy() const;
    double getVz() const;

    void setYaw(double yaw);
    double getYaw()const;
    double getYawFromQuat() const;


    //Use eigen
    void setOrientation(Eigen::Quaterniond quat);
    void setOrientation(double qw, double qx, double qy, double qz);

    void getPosition(double& x, double& y, double& z) const;

    //get quaternion
    Eigen::Quaterniond getOrientation() const;

    //convert from eigen
    void getOrientationRPY(double& roll, double& pitch, double& yaw) const;


    // OPERATORS

    void operator=(MavState &m){

        this->setPosition(m.getX(),m.getY(),m.getZ());

        this->setOrientation(m.getOrientation());

        this->setV(m.getVx(),m.getVy(),m.getVz());

        this->setYaw(m.getYaw());

    }
    long int timestamp;
private:
    // MOCAP
    //"Latency" from motion capture server
    double _x;
    double _y;
    double _z;
    double _vx;
    double _vy;
    double _vz;
    double _yaw;
    Eigen::Quaterniond _orientation;

};


#endif //MOCAP2MAV_MAVSTATE_H
