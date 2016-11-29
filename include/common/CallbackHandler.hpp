//
// Created by andrea on 29/04/16.
//

#ifndef MOCAP2MAV_CALLBACKHANDLER_HPP
#define MOCAP2MAV_CALLBACKHANDLER_HPP

#include "MavState.h"
#include "lcm/lcm-cpp.hpp"
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include <iostream>
class CallbackHandler {

public:

    MavState _vision_pos;
    MavState _position_sp;
    exec::task _task;

    bool _estimate_ready;
    bool _position_sp_ready;

    CallbackHandler(){

        _position_sp.setPosition(0,0,0);
        _position_sp_ready = false;
        _position_sp.setOrientation(1,0,0,0);
        _position_sp.setYaw(0);

        _vision_pos.setPosition(0,0,0);
        _estimate_ready = false;
        _vision_pos.setOrientation(1,0,0,0);
        _vision_pos.setYaw(0);

    }
//TODO: use lcm2mavstate
    void visionEstimateCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        _vision_pos.setPosition((double)msg->position[0],(double)msg->position[1],(double)msg->position[2]);

        _vision_pos.setV((double)msg->velocity[0], (double)msg->velocity[1], (double)msg->velocity[2]);

        _vision_pos.setOrientation((double)msg->orientation[0],(double)msg->orientation[1],(double)msg->orientation[2],(double)msg->orientation[3]);

        _estimate_ready = true;

    }

    void positionSetpointCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        _position_sp.setPosition((float)msg->position[0],(float)msg->position[1],(float)msg->position[2]);

        _position_sp.setOrientation((float)msg->orientation[0],(float)msg->orientation[1],(float)msg->orientation[2],(float)msg->orientation[3]);

        _position_sp.setYaw((float)msg->yaw);

    }

    void actualTaskCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const exec::task* msg){

        _task.action = msg->action;
        _task.id = msg->id;

        _task.params[0] = msg->params[0];
        _task.params[1] = msg->params[1];
        _task.params[2] = msg->params[2];
        _task.params[3] = msg->params[3];

        _task.x = msg->x;
        _task.y = msg->y;
        _task.z = msg->z;

        _task.yaw = msg->yaw;


    }

    MavState lcmPose2MavState(const geometry::pose lcmPose){

        MavState temp;
        temp.setPosition((float)lcmPose.position[0],(float)lcmPose.position[1],(float)lcmPose.position[2]);
        temp.setOrientation((float)lcmPose.orientation[0],(float)lcmPose.orientation[1],(float)lcmPose.orientation[2],(float)lcmPose.orientation[3]);
        temp.setYaw(lcmPose.yaw);

        temp.timestamp = lcmPose.timestamp;
        return temp;

    }

    geometry::pose mavState2LcmPose(MavState mavPose){

        geometry::pose temp;

        temp.position[0] = mavPose.getX();
        temp.position[1] = mavPose.getY();
        temp.position[2] = mavPose.getZ();

        temp.yaw = mavPose.getYaw();

        temp.orientation[0] = mavPose.getOrientation().w();
        temp.orientation[1] = mavPose.getOrientation().x();
        temp.orientation[2] = mavPose.getOrientation().y();
        temp.orientation[3] = mavPose.getOrientation().z();

        temp.timestamp = mavPose.timestamp;

        return temp;

    }

};

#endif //MOCAP2MAV_CALLBACKHANDLER_HPP
