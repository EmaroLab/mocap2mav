#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "mocap/include/Automatic.h"
#include "common/CallbackHandler.hpp"
#include "poll.h"
//#include "common/Parameters.hpp"

using namespace common;

int main(int argc, char** argv){


	lcm::LCM handler, handler2, handler3;

	if (!handler.good() && !handler2.good() && !handler3.good())
		return 1;

	CallbackHandler call;

	//Load parameters
	//TODO: create param class and feed it to lander


	Automatic autom;
	Lander lander;

	lcm::Subscription *sub   = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
	lcm::Subscription *sub2  = handler2.subscribe("platform/pose", &CallbackHandler::positionSetpointCallback, &call);
	lcm::Subscription *sub3  = handler3.subscribe("actual_task", &CallbackHandler::actualTaskCallback, &call);

	sub ->setQueueCapacity(1);
	sub2->setQueueCapacity(1);
	sub3->setQueueCapacity(1);

	struct pollfd fds[2];

	fds[0].fd = handler3.getFileno(); // Actual task
	fds[0].events = POLLIN;

	fds[1].fd = handler2.getFileno(); // Square pose
	fds[1].events = POLLIN;

    bool waiting = true;

	MavState platform;

	while(0==handler.handle()){

		autom.setState(call._vision_pos);
        lander.setState(call._vision_pos);

		int ret = poll(fds,2,0);

		if(fds[0].revents & POLLIN){

			handler3.handle();
			autom.setTask(call._task);

			std::cout<<  "New task arrived with action: " << printAction(autom._actualTask.action) << std::endl;

            waiting = false;
            autom.handleCommands();
		}

		if(fds[1].revents & POLLIN){

			handler2.handle();
			platform = call._position_sp;
            autom.setPlatformState(platform);

		}

        if(!waiting) {

            //Call commands
            autom.executeCommand();

            //Prepare LCM stuff
            geometry::pose command = call.mavState2LcmPose(autom._comm);
            geometry::pose platRobPos;

            platRobPos.position[0] = platform.getX();
            platRobPos.position[1] = platform.getY();
            platRobPos.position[2] = platform.getZ();

            platRobPos.velocity[0] = platform.getVx();
            platRobPos.velocity[1] = platform.getVy();
            platRobPos.velocity[2] = platform.getVz();

            handler.publish("local_position_sp", &command);

			//For gazebo visualization with the marker plugin
			handler.publish("Marker/pose_cmd", &command);

            handler3.publish("platRob", &platRobPos);
        }
	}

	return 0;

}

