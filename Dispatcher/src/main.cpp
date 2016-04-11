//
// Created by andrea on 05/04/16.
//

#include <lcm/lcm-cpp.hpp>
#include "PositionDispatcher.h"
#include <QApplication>
#include <iostream>
#include "lcm_messages/geometry/pose.hpp"
#include "QDebug"

class callbackHandler
{
public:
    ~callbackHandler() {}

    geometry::pose _vision_pos;
    geometry::pose _position_sp;

    void callback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        for (int i = 0; i < 3; ++i) {
            _vision_pos.position[i] = msg->position[i];
        }

        for (int j = 0; j < 4; ++j) {
            _vision_pos.orientation[j] = msg->orientation[j];
        }

        //std::cout << _vision_pos.position[0] << " " << _vision_pos.position[1] << " " << _vision_pos.position[2] << std::endl;

    }

    void callback2(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        for (int i = 0; i < 3; ++i) {
            _position_sp.position[i] = msg->position[i];
        }

        for (int j = 0; j < 4; ++j) {
            _position_sp.orientation[j] = msg->orientation[j];
        }

        //std::cout << _vision_pos.position[0] << " " << _vision_pos.position[1] << " " << _vision_pos.position[2] << std::endl;

    }

};


int main(int argc, char** argv){

    lcm::LCM handler;


    if (!handler.good())
        return 1;

    QApplication a(argc, argv);

    PositionDispatcher p;

    callbackHandler call;
    handler.subscribe("vision_position_estimate", &callbackHandler::callback, &call);
    //handler.subscribe("local_position_sp",&callbackHandler::callback2, &call);
    qint64 now =  QDateTime::currentMSecsSinceEpoch();

    while(0==handler.handle()){

        p.sendPosition(call._vision_pos.timestamp,call._vision_pos.position);
        std::cout <<"Vision estimate: "<< call._vision_pos.position[0] << " " << call._vision_pos.position[1] << " " << call._vision_pos.position[2] << std::endl;
        std::cout <<"Position SP: "<< call._position_sp.position[0] << " " << call._position_sp.position[1] << " " << call._position_sp.position[2] << std::endl;

    }

    return 0;

}
