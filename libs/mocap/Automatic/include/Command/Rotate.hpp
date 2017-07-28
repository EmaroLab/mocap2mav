//
// Created by andreanistico on 13/06/17.
//

#ifndef MOCAP2MAV_ROTATE_HPP
#define MOCAP2MAV_ROTATE_HPP

#include "Command.hpp"
#include "common/conversions.h"
class Rotate : protected Command{

private:

    void rotate(){

        _comm->setType(MavState::type::POSITION);
        double angleValid = _actualTask->params[0];
        double yawSP = _actualTask->yaw;
        double yawComm;
        if(angleValid==0){

            double x_target = _actualTask->x;
            double y_target = _actualTask->y;

            yawSP = atan2(y_target - _state->getY(),x_target - _state->getX());

        }

        //calculateYawInterm(_state.getYawFromQuat(),yawSP,yawComm);

        yawComm = yawSP;


        _comm->setYaw(yawComm);

        Eigen::Quaterniond q_interm = yawToQuaternion(yawComm);

        _comm->setOrientation(q_interm);


    }

    void calculateYawInterm(float heading, double yawTarget, double &yawComm){

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


public:
    Rotate(MavState *_state, MavState *_comm,exec::task *_actualTask) : Command(_state, _comm, _actualTask) {}

    void execute() override {
        rotate();
    }

};

#endif //MOCAP2MAV_ROTATE_HPP
