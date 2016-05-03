#ifndef AUTOMATIC_H
#define AUTOMATIC_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include "MavState.h"

class Automatic
{
   
public:
     Automatic();
    void land(float dt, double vz);
    void takeOff();
    void move();
    void rotate();
    void setState(MavState pose);
    void setTask(exec::task task);
    exec::task getTask();
    void calculatePositionInterm(const double alpha,const exec::task target, const MavState state, MavState &comm);
    void calculateYawInterm(MavState state, double &comm);
    MavState getState();
    MavState _comm;
    //void trajectory(double omega, double rad, double c[2], float t, int secs, float look, position robot_state);
    //void land_plat(MavState platform, MavState robot_state,float  = 1);

private:
   MavState _state;
   exec::task _actualTask;

};

#endif // AUTOMATIC_H
