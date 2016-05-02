#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "Executioner.h"
#include <math.h>
#include "CallbackHandler.hpp"



int main(int argc, char** argv){

    lcm::LCM handler;

    if (!handler.good())
        return 1;
    CallbackHandler call;
    Executioner e;

    handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
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
