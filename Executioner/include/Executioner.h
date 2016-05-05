#ifndef EXECUTIONER_H
#define EXECUTIONER_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include <string>
#include "MavState.h"


class Executioner
{

public:

	Executioner();
	void run(MavState state);
	bool CheckActions(std::string a,MavState state);

	exec::task _actualTask;
	bool _can_run;
	bool _publish_task;
	int _actualNode;
    bool _newTask;
};
#endif
		
