#ifndef AUTOMATIC_H
#define AUTOMATIC_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"

class Automatic
{
   
public:
     Automatic();
    void land(float dt, double vz);
    void takeOff();
    void move(exec::task target,geometry::pose pose);
    void rotate();
    void setState(geometry::pose pose);
    void setTask(exec::task task);
    geometry::pose getState( return _state);
    exec::task getTask( return _actualTask);
   
    //void trajectory(double omega, double rad, double c[2], float t, int secs, float look, position robot_state);
    //void land_plat(MavState platform, MavState robot_state,float  = 1);

private:
   geometry::pose _state;
   exec::task _actualTask;
   




};

#endif // AUTOMATIC_H
