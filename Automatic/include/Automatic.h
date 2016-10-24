#ifndef AUTOMATIC_H
#define AUTOMATIC_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include "MavState.h"
#include <iostream>

class Automatic
{
   
public:

    Automatic();
    void land(const float dt,const float speed,const float offset,const float land_gain);
    void takeOff();
    void move();
    void rotate();
    void setState(MavState pose);
    void setTask(exec::task task);
    exec::task getTask();
    void calculatePositionInterm(const double alpha,const exec::task target, const MavState state, MavState &comm);
    void calculateYawInterm(const float heading,const double yawTarget, double &yawComm);
    MavState getState();
    MavState _comm;
    exec::task _actualTask;

private:
   MavState _state;

};

#endif // AUTOMATIC_H
