//
// Created by andreanistico on 13/06/17.
//

#ifndef MOCAP2MAV_COMMAND_HPP
#define MOCAP2MAV_COMMAND_HPP

#include "common/MavState.h"
#include "lcm_messages/exec/task.hpp"
class Command {

protected:
    MavState* _state;
    MavState* _comm;
    exec::task* const _actualTask;
    bool _newTask;


public:
    Command(){}
    Command(MavState *_state, MavState *_comm,exec::task *_actualTask) : _state(_state), _comm(_comm),
                                                                         _actualTask(_actualTask), _newTask(true) {}

    virtual ~Command() {}

    virtual void execute() = 0;
    void init(MavState* s, MavState* c){
        this->_state = s;
        this->_comm = c;
    }

};




#endif //MOCAP2MAV_COMMAND_HPP
