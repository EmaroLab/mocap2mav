//
// Created by andreanistico on 14/06/17.
//

#ifndef MOCAP2MAV_IDLE_HPP
#define MOCAP2MAV_IDLE_HPP


#include "Command.hpp"
#include "common/conversions.h"

class Idle : public Command{

private:

    double _xin;
    double _yin;
    double _zin;
    double _yawin;

    void idle() {
        //Save initial state if we have a new task
        if (_newTask) {
            _xin = _state->getX();
            _yin = _state->getY();
            _zin = _state->getZ();
            _yawin = _state->getYawFromQuat();
            _newTask = false;
        }


        _comm->setType(MavState::type::POSITION);
        _comm->setX((float)_xin);
        _comm->setY((float)_yin);
        _comm->setZ((float)_zin);

        Eigen::Quaterniond q = getQuatFromYaw(_yawin);

        _comm->setOrientation((float)q.w(),(float)q.x(),(float)q.y(),(float)q.z());
    }

public:

    Idle(MavState *_state, MavState *_comm,exec::task *_actualTask) :
    Command(_state, _comm, _actualTask){}

    void execute() override {
        idle();
    }

};


#endif //MOCAP2MAV_IDLE_HPP
