#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "Automatic.h"
#include "CallbackHandler.hpp"
#include "poll.h"
#include "QDebug"
#include "common/common.h"

int main(int argc, char** argv){

	lcm::LCM handler, handler2, handler3;

	if (!handler.good() && !handler2.good() && !handler3.good())
		return 1;

	CallbackHandler call;
	Automatic autom;

	lcm::Subscription *sub  = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
	lcm::Subscription *sub2 = handler2.subscribe("local_position_sp", &CallbackHandler::visionEstimateCallback, &call);
	lcm::Subscription *sub3= handler3.subscribe("actual_task",&CallbackHandler::actualTaskCallback, &call);

	sub->setQueueCapacity(1);
	sub2->setQueueCapacity(1);
	sub3->setQueueCapacity(1);

	struct pollfd fds[1];

	fds[0].fd = handler3.getFileno(); // Actual task
	fds[0].events = POLLIN;

	bool newTask;

	uint64_t t = 0;
	uint64_t t_prev = 0;

	while(0==handler.handle()){


		t = getTimeMilliSecond();
		float dt = t_prev != 0 ? (t - t_prev) * (float)MILLI2SECS : 0.0f;
		t_prev = t;

		autom.setState(call._vision_pos);
        std::cout << autom.getState()._x << " " << autom.getState()._y << " " << autom.getState()._z << std::endl;
		int ret = poll(fds,1,0);


		if(fds[0].revents & POLLIN){

			handler3.handle();
			autom.setTask(call._task);
			std::cout<<  "New task arrived with action: " << autom._actualTask.action;
			newTask = true;

		}
		else{
			newTask = false;
		}

		if (autom.getTask().action == "m"){

			autom.move();

		}
		if (autom.getTask().action == "t"){

			//Save initial state if we have a new task
			if (newTask){
				autom._actualTask.x = autom.getState().getX();
				autom._actualTask.y = autom.getState().getY();
			}

			autom.takeOff();

		}
		if (autom.getTask().action == "l"){

			//Land on last set point command, is it good?
			if (newTask){
				autom._actualTask.x = autom._comm.getX();
				autom._actualTask.y = autom._comm.getY();
			}

			autom.land(dt,8,(float)autom._actualTask.params[0],0.8);

		}
		if (autom.getTask().action == "r"){

			autom.rotate();

		}

		geometry::pose command = call.mavState2LcmPose(autom._comm);

        std::cout << "COMMAND " <<command.position[0] << " " << command.position[1] << " " << command.position[2] << std::endl;
		handler.publish("local_position_sp", &command);



	}

	return 0;

}

