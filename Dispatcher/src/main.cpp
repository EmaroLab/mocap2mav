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
    int count = 0;

    bool _estimate_ready = false;
    bool _position_sp_ready = false;

    void callback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){

        if (_vision_pos.position[0] == msg->position[0] && _vision_pos.position[1] == msg->position[1] && _vision_pos.position[2] == msg->position[2]){
            count++;
            if(count >= 5)
                _vision_pos.isValid = 0;
        }
        else{
            count = 0;
            _vision_pos.isValid = 1;
        }

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

        if (call._vision_pos.isValid == 0)
        std::cout<<"The rigid body has gone out from mocap"<<std::endl;

    }

    return 0;

}
