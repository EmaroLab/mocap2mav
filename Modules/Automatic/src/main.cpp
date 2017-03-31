#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "Automatic.h"
#include "common/CallbackHandler.hpp"
#include "poll.h"


using namespace common;

int main(int argc, char** argv){

	lcm::LCM handler, handler2, handler3;

	if (!handler.good() && !handler2.good() && !handler3.good())
		return 1;

	CallbackHandler call;
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
        lander.setState(call._vision_pos);

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

			handler2.handle();
			platform = call._position_sp;
            lander.setPlatformState(platform);

		}

		//Choose action

		if (autom.getTask().action == actions::IDLE) std::cout << "Idle, is the list empty?" << std::endl;

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

            if(autom._actualTask.params[0] == 1){
                //autom.land2(platform,autom._actualTask.params[1],autom._actualTask.params[2],autom._actualTask.params[3]);

                lander.run();
                autom._comm = lander.getCommand();

            }
            else autom.land1((float)autom._actualTask.x,(float)autom._actualTask.y,(float)autom._actualTask.z);

		}
		if (autom.getTask().action == actions::ROTATE){

			autom.rotate();

		}

        if(!waiting) {
            geometry::pose command = call.mavState2LcmPose(autom._comm);
            geometry::pose platRobPos;

            platRobPos.position[0] = platform.getX();
            platRobPos.position[1] = platform.getY();
            platRobPos.position[2] = platform.getZ();

            platRobPos.velocity[0] = autom.getState().getX();
            platRobPos.velocity[1] = autom.getState().getY();
            platRobPos.velocity[2] = autom.getState().getZ();

            handler.publish("local_position_sp", &command);

			//For gazebo visualization with the marker plugin
			handler.publish("Marker/pose_cmd", &command);

            handler3.publish("platRob", &platRobPos);
        }
	}

	return 0;

}

