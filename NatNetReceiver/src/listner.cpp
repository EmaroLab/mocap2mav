//
// Created by andrea on 15/03/16.
//

#include <lcm/lcm-cpp.hpp>
#include "NatNetReceiver.h"
#include <QApplication>
#include "lcm_messages/geometry/pose.hpp"
#include <iostream>
#include "common/common.h"

void setPositionMsg(NatNetReceiver &nat, geometry::pose &msg);

bool isPoseValid(geometry::pose &state, const geometry::pose prev_state);

int count = 0;

int main(int argc, char** argv){

    lcm::LCM handler;

    if (!handler.good())
        return 1;

    QApplication a(argc, argv);

    NatNetReceiver nat;

    geometry::pose pose_msg;
    geometry::pose prev_pose_msg;

    while(true) {

        if(nat._isReady){

            setPositionMsg(nat, pose_msg);

            // Quick estimate for velocities

            int64_t dt = pose_msg.timestamp - prev_pose_msg.timestamp;

            pose_msg.velocity[0] = (pose_msg.position[0] - prev_pose_msg.position[0])/((double)dt * MILLI2SECS);
            pose_msg.velocity[1] = (pose_msg.position[1] - prev_pose_msg.position[1])/((double)dt * MILLI2SECS);
            pose_msg.velocity[2] = (pose_msg.position[2] - prev_pose_msg.position[2])/((double)dt * MILLI2SECS);

            // Check for position validity

            if(!isPoseValid(pose_msg, prev_pose_msg)) std::cout << "Untracked object" << std::endl;
            else std::cout << "Object acquired!!" << std::endl;

            //Publish pose estimate
            handler.publish("vision_position_estimate",&pose_msg);

            // Reset
            nat._isReady = false;
            prev_pose_msg.position[0] = pose_msg.position[0];
            prev_pose_msg.position[1] = pose_msg.position[1];
            prev_pose_msg.position[2] = pose_msg.position[2];
            prev_pose_msg.timestamp   = pose_msg.timestamp;

        }

        a.processEvents();
    }

    return 0;

}

void setPositionMsg(NatNetReceiver &nat, geometry::pose &msg){

    msg.position[0] = nat.get_position()[0]; //x
    msg.position[1] = nat.get_position()[1]; //y
    msg.position[2] = nat.get_position()[2]; //z

    msg.orientation[0] = nat.get_orientation()[0]; //w
    msg.orientation[1] = nat.get_orientation()[1]; //x
    msg.orientation[2] = nat.get_orientation()[2]; //y
    msg.orientation[3] = nat.get_orientation()[3]; //z

    msg.timestamp = getTimeMilliSecond();

}

bool isPoseValid(geometry::pose &state, const geometry::pose prev_state){

    if (prev_state.position[0] == state.position[0] && prev_state.position[1] == state.position[1] && prev_state.position[2] == state.position[2]){
        count++;
        if(count >= 5)
            state.isValid = 0;
            return false;
    }
    else{
        count = 0;
        state.isValid = 1;
        return true;
    }

}