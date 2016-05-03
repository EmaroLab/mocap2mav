#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "Automatic.h"
#include "CallbackHandler.hpp"
#include "poll.h"
#include "QDebug"
#include "common/common.h"

int main(int argc, char** argv){

	lcm::LCM handler, handler2, handler3;

	if (!handler.good())
		return 1;


	CallbackHandler call;
	Automatic autom;

	lcm::Subscription *sub  = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
	lcm::Subscription *sub2 = handler2.subscribe("local_position_sp", &CallbackHandler::visionEstimateCallback, &call);
	lcm::Subscription *sub3= handler3.subscribe("actual_task",&CallbackHandler::actualTaskCallback, &call);

	sub->setQueueCapacity(1);
	sub2->setQueueCapacity(1);
	sub3->setQueueCapacity(5);

	struct pollfd fds[2];

	fds[0].fd = handler2.getFileno(); //Position setpoint
	fds[0].events = POLLIN;

	fds[1].fd = handler3.getFileno(); // Actual task
	fds[1].events = POLLIN;

	bool newTask = true;

	uint64_t t = 0;
	uint64_t t_prev = 0;

	while(0==handler.handle()){


		t = getTimeMilliSecond();
		float dt = t_prev != 0 ? (t - t_prev) * (float)MILLI2SECS : 0.0f;
		t_prev = t;

		autom.setState(call._vision_pos);

		int ret = poll(fds,2,0);

		if (fds[0].revents & POLLIN){


			handler2.handle();
			qDebug() << call._position_sp._x;
			qDebug() << "New setpoint arrived";

			//Perform init for task which need it

		}

		if(fds[1].revents & POLLIN){

			autom.setTask(call._task);
			qDebug() << "New task arrived";
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
			autom.land(dt, autom.getState().getVz());

		}
		if (autom.getTask().action == "r"){

			//TODO: perform init
			autom.rotate();

		}

		geometry::pose command = call.mavState2LcmPose(autom._comm);

		handler.publish("local_position_sp", &command);


	}

	return 0;

}

