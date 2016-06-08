#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "Executioner.h"
#include "qdebug.h"
#include "common/CallbackHandler.hpp"



int main(int argc, char** argv){

    lcm::LCM handler;

    if (!handler.good())
        return 1;
    CallbackHandler call;
    Executioner e;

    handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);

    while(0==handler.handle()){


            //Run state machine
            e.run();
            //Publish next task
            if(e.readyToPublish()) {

                qDebug() << "publishing task";
                handler.publish("actual_task", &e._actualTask);


            }


    }

}
