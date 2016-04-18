//
// Created by andrea on 05/04/16.
//

#include <lcm/lcm-cpp.hpp>
#include "PositionDispatcher.h"
#include <QApplication>
#include <iostream>
#include "QDebug"


class callbackHandler
{
public:
    ~callbackHandler() {}

    geometry::pose _vision_pos;
    geometry::pose _position_sp;

    bool _estimate_ready = false;
    bool _position_sp_ready = false;

    void callback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        for (int i = 0; i < 3; ++i) {
            _vision_pos.position[i] = msg->position[i];
        }

        for (int j = 0; j < 4; ++j) {
            _vision_pos.orientation[j] = msg->orientation[j];
        }
        _estimate_ready = true;

    }

    void callback2(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        for (int i = 0; i < 3; ++i) {
            _position_sp.position[i] = msg->position[i];
        }

        for (int j = 0; j < 4; ++j) {
            _position_sp.orientation[j] = msg->orientation[j];
        }
        _position_sp.yaw = msg->yaw;
        _position_sp_ready = true;

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
    handler.subscribe("local_position_sp", &callbackHandler::callback2, &call); // Do we need to bufferize sp messages?

    while(0==handler.handle()){

        p.sendPosition(call._vision_pos.timestamp, call._vision_pos, call._position_sp, call._estimate_ready, call._position_sp_ready);

        if(call._estimate_ready)    call._estimate_ready = false;
        //if(call._position_sp_ready) call._position_sp_ready = false;

        //TODO: check for lost rigid bodies

    }

    return 0;

}
