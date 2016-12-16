#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "Automatic.h"
#include "common/CallbackHandler.hpp"
#include "poll.h"
#include "common/common.h"

using namespace common;

int main(int argc, char** argv){

	lcm::LCM handler, handler2, handler3;

	if (!handler.good() && !handler2.good() && !handler3.good())
		return 1;

	CallbackHandler call;
	Automatic autom;

	lcm::Subscription *sub   = handler.subscribe("vision_position_estimate", &CallbackHandler::visionEstimateCallback, &call);
	lcm::Subscription *sub2  = handler2.subscribe("square/pose", &CallbackHandler::positionSetpointCallback, &call);
	lcm::Subscription *sub3  = handler3.subscribe("actual_task", &CallbackHandler::actualTaskCallback, &call);

	sub ->setQueueCapacity(1);
	sub2->setQueueCapacity(1);
	sub3->setQueueCapacity(1);

	struct pollfd fds[2];

	fds[0].fd = handler3.getFileno(); // Actual task
	fds[0].events = POLLIN;

	fds[1].fd = handler2.getFileno(); // Square pose
	fds[1].events = POLLIN;

	bool newTask;
    bool waiting = true;
	uint64_t t = 0;
	uint64_t t_prev = 0;
	MavState platform;
	while(0==handler.handle()){

		t = time::getTimeMilliSecond();
		float dt = t_prev != 0 ? (t - t_prev) * (float)MILLI2SECS : 0.0f;
		t_prev = t;

		autom.setState(call._vision_pos);
        //std::cout << autom.getState()._x << " " << autom.getState()._y << " " << autom.getState()._z << std::endl;
		int ret = poll(fds,2,0);


		if(fds[0].revents & POLLIN){

			handler3.handle();
			autom.setTask(call._task);

			std::cout<<  "New task arrived with action: " << printAction(autom._actualTask.action) << std::endl;

			newTask = true;
            waiting = false;

		}
		else{
			newTask = false;
		}

		if(fds[1].revents & POLLIN){

            //std::cout << platform.getX() << " " << platform.getY() << " " << platform.getZ() << std::endl;
			handler2.handle();
			platform = call._position_sp;

		}

		//Choose action

		if (autom.getTask().action == actions::IDLE) std::cout << "Idle, is the list empty?" <<

		if (autom.getTask().action == actions::MOVE){

			autom.move();

		}
		if (autom.getTask().action == actions::TAKE_OFF){

			//Save initial state if we have a new task
			if (newTask){
				autom._actualTask.x = autom.getState().getX();
				autom._actualTask.y = autom.getState().getY();
				autom._actualTask.yaw = autom.getState().getYawFromQuat();
			}

			autom.takeOff();

		}
		if (autom.getTask().action == actions::LAND){

			//Land on last set point command, is it good?
			if (newTask){
				autom._actualTask.x = autom._comm.getX();
				autom._actualTask.y = autom._comm.getY();
			}

            if(autom._actualTask.params[1] == 1) autom.land2(platform);
            else autom.land1((float)autom._actualTask.x,(float)autom._actualTask.y,(float)autom._actualTask.params[0]);

		}
		if (autom.getTask().action == actions::ROTATE){

			autom.rotate();

		}

        if(!waiting) {
            geometry::pose command = call.mavState2LcmPose(autom._comm);

            //std::cout << "COMMAND " <<command.position[0] << " " << command.position[1] << " " << command.position[2] << std::endl;
            handler.publish("local_position_sp", &command);
        }
	}

	return 0;

}

