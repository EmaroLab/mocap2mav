#ifndef EXECUTIONER_H
#define EXECUTIONER_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include <string>
#include "common/MavState.h"
#include "common/common.h"


class Executioner
{

public:

	Executioner();
	void run();
	bool CheckActions(int a);

	exec::task _actualTask;
	bool _can_run;
	bool _publish_task;
	int _actualNode;
    bool _newTask;

private:
	MavState _state;
};
#endif
		
