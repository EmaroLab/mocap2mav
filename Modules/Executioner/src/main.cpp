#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "Executioner.h"
#include "common/CallbackHandler.hpp"
#include "Parser.h"
#include <poll.h>


int main(int argc, char** argv){



    Parser p;
    Executioner e;

    p.loadFile("lists/list.txt");
    std::cout << "Parsing ....." << std::endl;

    if(p.parse()){
        std::cout << ".......Parsed" << std::endl;
        e.setList(p.getTaskListParsed());
    }else{

        std::cout << "Unable to parse the file" << std::endl;

    }
    // Call init everytime you have a new list.
    e.init();
    lcm::LCM handler,handler2;

    if (!handler.good())
        return 1;
    CallbackHandler call;


    handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
    handler2.subscribe("state", &CallbackHandler::stateCallback, &call);

    struct pollfd fds[1];

    fds[0].fd = handler2.getFileno(); // Actual task
    fds[0].events = POLLIN;

    while(0==handler.handle()){

        //Fetch land messages from mavros
        int ret = poll(fds,1,0);

        if(fds[0].revents & POLLIN){
            handler2.handle();
            e.setStatus(call._armed,call._landed);

        }


        //AndOr

        //Run state machine
        e.setState(call._vision_pos);
        e.run();
        //Publish next task
        if(e.readyToPublish()) {

            std::cout << "publishing task" << std::endl;
            handler.publish("actual_task", &e._actualTask);

        }

    }

}
