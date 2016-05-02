#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "Automatic.h"
#include "CallbackHandler.hpp"
#include "poll.h"
#include "QDebug"

int main(int argc, char** argv){

	lcm::LCM handler, handler2, handler3;

	if (!handler.good())
		return 1;




	CallbackHandler call;
	Automatic autom;

	lcm::Subscription *sub  = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
	lcm::Subscription *sub2 = handler2.subscribe("local_position_sp", &CallbackHandler::positionSetpointCallback, &call);
	lcm::Subscription *sub3= handler3.subscribe("actual_task",&CallbackHandler::actualTaskCallback, &call);

	sub->setQueueCapacity(1);
	sub2->setQueueCapacity(1);
	sub3->setQueueCapacity(5);

	struct pollfd fds[2];

	fds[0].fd = handler2.getFileno(); //Position setpoint
	fds[0].events = POLLIN;

	fds[1].fd = handler3.getFileno(); // Actual task
	fds[1].events = POLLIN;

	while(0==handler.handle()){


		autom.setState(call._vision_pos);

		int ret = poll(fds,2,0);

		if (fds[0].revents & POLLIN){


			handler2.handle();



			qDebug() << "New setpoint arrived";

		}

		if(fds[1].revents & POLLIN){

			autom.setTask(call._task);
			qDebug() << "New task arrived";


		}

		if (autom.getTask().action == "m"){

			autom.move();

		}
		if (autom.getTask().action == "t"){

			//TODO: perform init
			autom.takeOff();

		}
		if (autom.getTask().action == "l"){

			//TODO: perform init
			autom.land(0.03, autom.getState().getVz()); //TODO: fix hard coding for dt

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

