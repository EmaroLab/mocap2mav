#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "Executioner.h"
#include "qdebug.h"
#include "common/CallbackHandler.hpp"
#include "AndOr/aograph.h"


int main(int argc, char** argv){

    lcm::LCM handler;

    if (!handler.good())
        return 1;
    CallbackHandler call;
    Executioner e;

    handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);


    AOgraph graph("DEFAULT");
    while(0==handler.handle()){

        if(e._can_run) {
            //Run state machine
            e.run();
            //Publish next task
            if(e._publish_task) {

                qDebug() << "publishing task";
                handler.publish("actual_task", &e._actualTask);
                e._publish_task = false;

            }
        }

    }

}
