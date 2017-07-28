#ifndef AUTOMATIC_H
#define AUTOMATIC_H
#include <iostream>
#include <memory>
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include "common/MavState.h"
#include "common/common.h"
#include "common/conversions.h"
#include "Lander/Lander.h"
#include "Command/TakeOff.hpp"
#include "Command/Land.hpp"
#include "Command/Move.hpp"
#include "Command/Rotate.hpp"
#include "Command/Idle.hpp"

class Automatic
{
   
public:

    Automatic();
    void setState(MavState pose);
    void setPlatformState(MavState pose);
    void setTask(exec::task task);
    exec::task getTask();
    MavState getState();
    MavState _comm;
    exec::task _actualTask;

    //Command pattern implementation
    void handleCommands();
    void executeCommand();

private:
   MavState _state;
   MavState _platformState;
   std::unique_ptr<Command> _actualCommand;

};

#endif // AUTOMATIC_H
