#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "Executioner.h"
#include <math.h>


class callbackHandler
{
public:
    ~callbackHandler() {}

    geometry::pose _vision_pos;





    void callback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg){


        for (int i = 0; i < 3; ++i) {
            _vision_pos.position[i] = msg->position[i];
        }


        _vision_pos.yaw = atan2(2*(msg->orientation[0]*msg->orientation[3]+msg->orientation[1]*msg->orientation[2]), 1-2*(msg->orientation[2]*msg->orientation[2] + msg->orientation[3]*msg->orientation[3]));

    }
};


int main(int argc, char** argv){

    lcm::LCM handler;

    if (!handler.good())
        return 1;
    callbackHandler call;
    Executioner e;

    handler.subscribe("vision_position_estimate", &callbackHandler::callback, &call);
    while(0==handler.handle()){

        if(e._can_run) {
            //Run state machine
            e.run(call._vision_pos);
            //Publish next task
            if(e._newTask) {
                handler.publish("actual_task", &e._actualTask);
                //e._newTask = false;
            }
        }

    }

}
