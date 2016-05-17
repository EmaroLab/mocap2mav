#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "Security.h"
#include "common/CallbackHandler.hpp"
#include "poll.h"

int main(int argc, char** argv)
{
    lcm::LCM handler, handler2;

    if (!handler.good())
        return 1;


    CallbackHandler call;
    Security secur;

    lcm::Subscription *sub  = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
    lcm::Subscription *sub2 = handler2.subscribe("local_position_sp", &CallbackHandler::positionSetpointCallback, &call);

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
            std::cout<<"x: "<<secur.getSetPoint().getX()<<" y: "<<secur.getSetPoint().getY()<<" z:cd "<<secur.getSetPoint().getZ()<<std::endl;
        }
        if(secur.IsOutside())
            secur.CalculateNewSP();

        geometry::pose command = call.mavState2LcmPose(secur.getSetPoint());
        std::cout<<"command "<<"x: "<<command.position[0]<<" y: "<<command.position[1]<<" z "<<command.position[2]<<std::endl;
        handler.publish("local_position_spOK", &command);
    }


}
