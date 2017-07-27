//
// Created by andreanisti on 15/11/16.
//

#include "common/MavState.h"

MavState::MavState()
        : timestamp(0), _x(0.0), _y(0.0), _z(0.0), _vx(0.0), _vy(0.0), _vz(0.0), _yaw(0.0),_type(type::POSITION) {

    _orientation.setIdentity();
}

void MavState::setPosition(double x, double y, double z) {
    _x = x;
    _y = y;
    _z = z;
}

void MavState::setX(double x) {
    _x = x;
}

void MavState::setY(double y) {
    _y = y;
}

void MavState::setZ(double z) {
    _z = z;
}

double MavState::getX() const{
    return _x;
}

double MavState::getY() const{
    return _y;
}

double MavState::getZ() const{
    return _z;
}

void MavState::setV(double vx, double vy, double vz) {

    _vx = vx;
    _vy = vy;
    _vz = vz;

}

double MavState::getVx() const{
    return _vx;
}

double MavState::getVy() const{
    return _vy;
}

double MavState::getVz() const{
    return _vz;
}

void MavState::setYaw(double yaw) {
    _yaw = yaw;
}

double MavState::getYaw() const{
    return _yaw;
}

void MavState::setOrientation(Eigen::Quaterniond quat) {
    _orientation = quat;
}

Eigen::Quaterniond MavState::getOrientation() const{

    return _orientation;

}

void MavState::getPosition(double &x, double &y, double &z) const{
    x = _x;
    y = _y;
    z = _z;
}

void MavState::getOrientationRPY(double &roll, double &pitch, double &yaw) const{

    roll  = std::atan2(2*(_orientation.w()*_orientation.x()+_orientation.y()*_orientation.z()), 1-2*(_orientation.x()*_orientation.x()+_orientation.y()*_orientation.y()));
    pitch = std::asin (2*(_orientation.w()*_orientation.y()-_orientation.z()*_orientation.x()));
    yaw   = std::atan2(2*(_orientation.w()*_orientation.z()+_orientation.x()*_orientation.y()), 1-2*(_orientation.y()*_orientation.y()+_orientation.z()*_orientation.z()));

}

void MavState::setOrientation(double qw, double qx, double qy, double qz) {

    _orientation.w() = qw;
    _orientation.x() = qx;
    _orientation.y() = qy;
    _orientation.z() = qz;

}

double MavState::getYawFromQuat() const{
    return std::atan2(2*(_orientation.w()*_orientation.z()+_orientation.x()*_orientation.y()), 1-2*(_orientation.y()*_orientation.y()+_orientation.z()*_orientation.z()));
}

void MavState::setType(MavState::type t) {

    _type = t;

}

MavState::type MavState::getType() const{
    return _type;
}

void MavState::setVx(double v) {

    _vx = v;

}

void MavState::setVy(double v) {

    _vy = v;

}

void MavState::setVz(double v) {

    _vz = v;

}








