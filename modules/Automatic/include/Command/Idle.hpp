//
// Created by andreanistico on 14/06/17.
//

#ifndef MOCAP2MAV_IDLE_HPP
#define MOCAP2MAV_IDLE_HPP


#include "Command.hpp"

class Idle : protected Command{

private:
public:
    Idle(MavState *_state, MavState *_comm, exec::task *_actualTask) : Command(_state, _comm, _actualTask) {}

private:
    void execute() override {

    }

};



#endif //MOCAP2MAV_IDLE_HPP
