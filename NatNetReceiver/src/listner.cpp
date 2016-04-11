//
// Created by andrea on 15/03/16.
//



#include <lcm/lcm-cpp.hpp>
#include "NatNetReceiver.h"
#include <QApplication>
#include <iostream>
#include "lcm_messages/geometry/pose.hpp"

void publishPosition(NatNetReceiver &nat, lcm::LCM &handler, geometry::pose &msg);


int main(int argc, char** argv){

    lcm::LCM handler;

    if (!handler.good())
        return 1;


    QApplication a(argc, argv);

    NatNetReceiver nat;

    geometry::pose pose_msg;

    while(true) {

        if(nat._isReady){
            publishPosition(nat, handler, pose_msg);
            nat._isReady = false;
        }


        a.processEvents();
    }

    return 0;

}

void publishPosition(NatNetReceiver &nat, lcm::LCM &handler, geometry::pose &msg){

    msg.position[0] = nat.get_position()[0]; //x
    msg.position[1] = nat.get_position()[1]; //y
    msg.position[2] = nat.get_position()[2]; //z

    msg.orientation[0] = nat.get_orientation()[0]; //w
    msg.orientation[1] = nat.get_orientation()[1]; //x
    msg.orientation[2] = nat.get_orientation()[2]; //y
    msg.orientation[3] = nat.get_orientation()[3]; //z


    handler.publish("vision_position_estimate", &msg);



}
