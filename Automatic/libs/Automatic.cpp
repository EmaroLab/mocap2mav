#include "Automatic.h"
#include <string>
#include <math.h>
#define PI 3.141592653589

int land_count = 0;int rot_count = 0;int circle_count = 0; int land_plat_count = 0;

float error_int = 0;
float error_int_x = 0;
float error_int_y = 0;
bool  try_land = false;
bool start_try_land = false;
bool landing = false;

Automatic::Automatic()
{

}

geometry::pose Automatic::getState()
{ return _state;}
exec::task Automatic::getTask()
{return _actualTask;}

void Automatic::setState(geometry::pose rState)
{
    _state.position[0]=rState.position[0];
    _state.position[1]=rState.position[1];
    _state.position[2]=rState.position[2];
    _state.yaw=rState.yaw;
    _state.orientation[0]=rState.orientation[0];
    _state.orientation[1]=rState.orientation[1];
    _state.orientation[2]=rState.orientation[2];
    _state.orientation[3]=rState.orientation[3];
}

void Automatic::setTask(exec::task rTask)
{
    _actualTask.x=rTask.x;
    _actualTask.y=rTask.y;
    _actualTask.z=rTask.z;
    _actualTask.yaw=rTask.yaw;
    _actualTask.action=rTask.action;
    _actualTask.params[0]=rTask.params[0];
    _actualTask.params[1]=rTask.params[1];
    _actualTask.params[2]=rTask.params[2];
    _actualTask.params[3]=rTask.params[3];
}

void calculatePositionInterm(double alpha,exec::task target,geometry::pose state, geometry::pose comm);

void Automatic::move(exec::task target,geometry::pose state)
{
    double alpha = getTask().params[0];
    target.x=getTask().x;
    target.y=getTask().y;
    target.z=getTask().z;
    state.position[0]=getState().position[0];
    state.position[1]=getState().position[1];
    state.position[2]=getState().position[2];
    geometry::pose comm;
    comm.position[0]=target.x;
    comm.position[1]=target.y;
    comm.position[2]=target.z;
    calculatePositionInterm(alpha,target,state,comm);
}


void calculatePositionInterm(double alpha,exec::task target,geometry::pose state, geometry::pose comm)
{

    double positionError[3] = {target.x - state.position[0] ,target.y - state.position[1] , target.z - state.position[2]};
    double incrementVect[3];

    double dist = sqrt(pow(positionError[0],2) + pow(positionError[1],2) + pow(positionError[2],2));

    //Publish
    if(fabs(dist) <= alpha)
    {
        comm.position[0]=target.x;
        comm.position[1]=target.y;
        comm.position[2]=target.z;
    }

    else if(fabs(dist) > alpha)
    {
        //Normalize
        positionError[0] = positionError[0] / dist;
        positionError[1] = positionError[1] / dist;
        positionError[2] = positionError[2] / dist;

        //Calculate relative motion to actual position
        incrementVect[0] = positionError[0] * alpha;
        incrementVect[1] = positionError[1] * alpha;
        incrementVect[2] = positionError[2] * alpha;

        comm.position[0]=(state.position[0] + incrementVect[0]);
        comm.position[1]=(state.position[1] + incrementVect[1]);
        comm.position[2]=(state.position[2] + incrementVect[2]);
    }
}
