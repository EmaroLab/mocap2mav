#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "Security.h"
#include "CallbackHandler.hpp"
#include "poll.h"
#include "QDebug"
#include "common/common.h"

int main(int argc, char** argv)
{
    lcm::LCM handler, handler2;

    if (!handler.good())
        return 1;


    CallbackHandler call;
    Security secur;

    lcm::Subscription *sub  = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
    lcm::Subscription *sub2 = handler2.subscribe("local_position_sp", &CallbackHandler::visionEstimateCallback, &call);

    sub->setQueueCapacity(1);
    sub2->setQueueCapacity(1);
    struct pollfd fds[1];
    fds[0].events = POLLIN;
    fds[0].fd = handler2.getFileno();

    while(0==handler.handle())
    {

            secur.setState(call._vision_pos);

            int ret = poll(fds,1,0);


            if(fds[0].revents & POLLIN)
            {
                handler2.handle();
                secur.setSetPoint(call._position_sp);
            }
            secur.CalculateNewSP();

            geometry::pose command = call.mavState2LcmPose(secur._comm);
            handler.publish("local_position_sp", &command);
     }


}