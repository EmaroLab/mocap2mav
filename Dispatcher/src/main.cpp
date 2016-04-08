//
// Created by andrea on 05/04/16.
//

#include <lcm/lcm-cpp.hpp>

#include <QApplication>
#include <iostream>
#include "lcm_messages/geometry/pose.hpp"


class callbackHandler
{
public:
    ~callbackHandler() {}

    geometry::pose _vision_pos;
    void callback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        for (int i = 0; i < 3; ++i) {
            _vision_pos.position[i] = msg->position[i];
        }

        for (int j = 0; j < 4; ++j) {
            _vision_pos.orientation[j] = msg->orientation[j];
        }

        std::cout << _vision_pos.position[0] << " " << _vision_pos.position[1] << " " << _vision_pos.position[2] << std::endl;

    }

};


int main(int argc, char** argv){

    lcm::LCM handler;

    if (!handler.good())
        return 1;



    QApplication a(argc, argv);


    callbackHandler call;
    handler.subscribe("vision_position_estimate", &callbackHandler::callback, &call);


    while(0==handler.handle());

    return 0;

}