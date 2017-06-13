//
// Created by andreanistico on 17/02/17.
//

#include <iostream>
#include "Automatic/include/Lander/Lander.h"
#include "common/common.h"
#include "lcm/lcm-cpp.hpp"
#include "common/CallbackHandler.hpp"
#include "poll.h"



using namespace common;

int main(int argc, char** argv){

    lcm::LCM handler, handler2;

    if (!handler.good() && !handler2.good())
        return 1;

    CallbackHandler call;

    lcm::Subscription *sub   = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
    lcm::Subscription *sub2  = handler2.subscribe("platform/pose", &CallbackHandler::positionSetpointCallback, &call);

    sub ->setQueueCapacity(1);
    sub2->setQueueCapacity(1);

    struct pollfd fds[1];

    Lander lander;

    //lander.updateSignals();

    lander.handleMachine();

    fds[0].fd = handler2.getFileno(); // Square pose
    fds[0].events = POLLIN;

    std::cout << "waiting..." << std::endl;
    char a;
    std::cin >> a;
    std::cout << "...starting" << std::endl;

    int* actState(0);
    while(0==handler.handle()) {

        lander.setState(call._vision_pos);

        int ret = poll(fds, 1, 0);


        if (fds[0].revents & POLLIN) {

            handler2.handle();
            //Platform pose passed from the position setpoint channel, just a lazy workaround
            lander.setPlatformState(call._position_sp);

        }

        lander.updateSignals();
        lander.handleMachine();

        *actState = lander.getActualMachineState();
        std::cout << *actState << std::endl;
    }

    return 0;

}

