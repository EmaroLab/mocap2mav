#ifndef AUTOMATIC_H
#define AUTOMATIC_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include "common/MavState.h"
#include "common/common.h"
#include "include/Lander/Lander.h"
#include <iostream>

class Automatic
{
   
public:

    Automatic();
    void land2(MavState platPose,double kp, double ki, double kd);
    void land1(float x_target, float y_target, float h);
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
    Lander _lander;


private:
   MavState _state;

};

#endif // AUTOMATIC_H
