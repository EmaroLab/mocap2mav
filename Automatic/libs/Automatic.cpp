#include "Automatic.h"
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

MavState Automatic::getState()
{
    return _state;
}
exec::task Automatic::getTask()
{
    return _actualTask;
}

void Automatic::setState(MavState rState)
{
   _state = rState;
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


void Automatic::rotate() {

    double angleValid = _actualTask.params[0];
    double yawSP = _actualTask.yaw;
    double yawComm;
    if(angleValid==0){

        double x_target = _actualTask.x;
        double y_target = _actualTask.y;

        yawSP = atan2(y_target - _state.getY(),x_target - _state.getX());

    }

    calculateYawInterm(_state.getYawFromQuat(),yawSP,yawComm);
    _comm.setYaw((float)yawComm);

}

void Automatic::calculateYawInterm(float heading, double yawTarget, double &yawComm){

    double yawSp_h = yawTarget - heading;

    if (fabs(yawSp_h) <= PI/10) yawComm = yawTarget;
    else if(fabs(yawSp_h) > PI - PI/18){
        //Increase yaw

        yawComm = heading + PI / 10 ;
        if (yawComm > PI){
            yawComm = yawComm - 2*PI;
        }
    }


    else{
        if (yawSp_h > 0){
            //Increase yaw

            yawComm = heading + PI / 10 ;
            if (yawComm > PI){
                yawComm = yawComm - 2*PI;
            }
        }
        else{
            //decrease yaw

            yawComm = heading - PI / 10 ;
            if (yawComm < -PI){
                yawComm = yawComm + 2*PI;
            }
        }
    }
}

void Automatic::land(float dt, double vz) {

  /*  //landing procedure

    MavState comm = g::setPoint;
    position error, sP;
    float descending_rate = 0;

    float offset = nodeList[actualNode].a.params[1];
    float z = comm.z();

    bool descend_valid = false;
    if(fabs(vz) < 0.01 && (robot_state.z - offset) >= - 0.10){


        if(++land_count == land_wait * r_auto) {

            land_count = 0;
            output_land << g::state.x()<<" "<<g::state.y()<<";\n";
            executioner::land::landed = true;

        }

    }
    else{

        //Descending task

        error.x = p.x - robot_state.x;
        error.y = p.y - robot_state.y;

        if (robot_state.z - offset >= - 0.15 ){

            if(fabs(error.x) < 0.03 && fabs(error.y) < 0.03) {

                descend_valid = true;

            }
            else speed = 0;

            sP.x = error.x * land_gain * 0.8 + p.x;
            sP.y = error.y * land_gain * 0.8 + p.y;

            comm.setX(sP.x);
            comm.setY(sP.y);

            z += speed*dt;
        }
        else{

            //Centering task


            sP.x = error.x * land_gain + p.x;
            sP.y = error.y * land_gain + p.y;

            //wait to recenter

            if(fabs(error.x) < 0.08 && fabs(error.y) < 0.08){ z = g::state.z() + 0.3; descend_valid = true;}

            else if(fabs(error.x) < 0.05 && fabs(error.y) < 0.05){ z = g::state.z() + 0.5; descend_valid = true;}

            //z += descending_rate * dt;
            comm.setX(sP.x);
            comm.setY(sP.y);

        }

        land_count = 0;
        executioner::land::landed = false;

    }

    if(descend_valid) comm.setZ(z);
    autoCommand.push_back(comm);
    publish();*/


}

void Automatic::takeOff() {
        
    double height = _actualTask.params[0];

    _comm.setZ((float)height);

}

void Automatic::move()
{
    double alpha = _actualTask.params[0];

    calculatePositionInterm(alpha,_actualTask,_state,_comm);
}


void Automatic::calculatePositionInterm(const double alpha, const exec::task target, const MavState state, MavState &comm)
{

    double positionError[3] = {target.x - state.getX() ,target.y - state.getY() , target.z - state.getZ()};

    double dist = sqrt(pow(positionError[0],2) + pow(positionError[1],2) + pow(positionError[2],2));

    //Publish
    if(fabs(dist) <= alpha)
    {
        comm.setX((float)target.x);
        comm.setY((float)target.y);
        comm.setZ((float)target.z);
    }

    else if(fabs(dist) > alpha)
    {

        double incrementVect[3];
        //Normalize
        positionError[0] = positionError[0] / dist;
        positionError[1] = positionError[1] / dist;
        positionError[2] = positionError[2] / dist;

        //Calculate relative motion to actual position
        incrementVect[0] = positionError[0] * alpha;
        incrementVect[1] = positionError[1] * alpha;
        incrementVect[2] = positionError[2] * alpha;

        comm.setX(state.getX() + (float)incrementVect[0]);
        comm.setY(state.getY() + (float)incrementVect[1]);
        comm.setZ(state.getZ() + (float)incrementVect[2]);
    }
}
