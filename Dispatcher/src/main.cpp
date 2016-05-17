//
// Created by andrea on 05/04/16.
//

#include <lcm/lcm-cpp.hpp>
#include "PositionDispatcher.h"
#include <QApplication>
#include <poll.h>
#include "CallbackHandler.hpp"


bool useSecurity;

int main(int argc, char** argv){

    lcm::LCM handler, handler2;

    if (!handler.good())
        return 1;

    QApplication a(argc, argv);

    PositionDispatcher p;

    CallbackHandler call;

    lcm::Subscription *sub  = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
    lcm::Subscription *sub2;

    useSecurity = true;

    if (!useSecurity) {

        sub2 = handler2.subscribe("local_position_sp", &CallbackHandler::positionSetpointCallback,
                                  &call);
    } else{

        sub2 = handler2.subscribe("local_position_spOK", &CallbackHandler::positionSetpointCallback, &call);

    }

    sub->setQueueCapacity(1);
    sub2->setQueueCapacity(1);

    struct pollfd fds[1];

    fds[0].fd = handler2.getFileno();
    fds[0].events = POLLIN;

    while(0==handler.handle()){

        int ret = poll(fds,1,0);

        if (fds[0].revents & POLLIN){

            call._position_sp_ready = true;
            handler2.handle();

        }

        p.sendPosition(call._vision_pos.timestamp, call._vision_pos, call._position_sp, call._estimate_ready, call._position_sp_ready);

        if(call._estimate_ready)    call._estimate_ready = false;

    }

    return 0;

}
