#include "Automatic.h"

#define PI 3.141592653589


Eigen::Quaterniond getQuatFromYaw(double yaw){

    Eigen::Quaterniond quaterniond;

    quaterniond.w() = cos(yaw/2);
    quaterniond.x() = 0;
    quaterniond.y() = 0;
    quaterniond.z() = sin(yaw/2);

    return quaterniond;

}

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

    //calculateYawInterm(_state.getYawFromQuat(),yawSP,yawComm);

    yawComm = yawSP;


    _comm.setYaw(yawComm);

    Eigen::Quaterniond q_interm = getQuatFromYaw(yawComm);

    _comm.setOrientation(q_interm);

}

//QUA

void Automatic::calculateYawInterm(float heading, double yawTarget, double &yawComm){

    double yawSp_h = yawTarget - heading;



/*  DISABLE ALGORITHM FOR BUG FIXING
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
    */
}

void Automatic::land(MavState platPose) {

    //Calculate difference

    double dx = _state.getX() - platPose.getX();
    double dy = _state.getY() - platPose.getY();
    double dz = _state.getZ() - platPose.getZ();


    _comm.setType(MavState::type::VELOCITY);
    _comm.setZ(-1);


}

void Automatic::takeOff() {

    double height = _actualTask.params[0];

    _comm.setX((float)_actualTask.x);
    _comm.setY((float)_actualTask.y);
    _comm.setZ((float)height);

    Eigen::Quaterniond q = getQuatFromYaw(_actualTask.yaw);

    _comm.setOrientation((float)q.w(),(float)q.x(),(float)q.y(),(float)q.z());

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
