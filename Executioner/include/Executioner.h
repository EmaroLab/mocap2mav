#ifndef EXECUTIONER_H
#define EXECUTIONER_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include <string>


class Executioner
{

public:

  Executioner();
        void run(geometry::pose state);
        bool CheckActions(std::string a,geometry::pose state);
        exec::task _actualTask;
	bool _can_run;
        exec::task _node;
	int _actualNode;
};
#endif
		
