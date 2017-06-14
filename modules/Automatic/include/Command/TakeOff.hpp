//
// Created by andreanistico on 13/06/17.
//

#ifndef MOCAP2MAV_TAKEOFF_HPP
#define MOCAP2MAV_TAKEOFF_HPP

#include "Command.hpp"
#include "common/conversions.h"
class TakeOff : protected Command{

private:

    double _xin;
    double _yin;
    double _yawin;

    void takeoff() {
        //Save initial state if we have a new task
        if (_newTask) {
            _xin = _state->getX();
            _yin = _state->getY();
            _yawin = _state->getYawFromQuat();
            _newTask = false;
        }

        //Take off procedure
        _comm->setType(MavState::type::POSITION);
        double height = _actualTask->params[0];

        _comm->setX((float)_xin);
        _comm->setY((float)_yin);
        _comm->setZ((float)height);

        Eigen::Quaterniond q = getQuatFromYaw(_yawin);

        _comm->setOrientation((float)q.w(),(float)q.x(),(float)q.y(),(float)q.z());
    }


public:
    TakeOff(MavState *_state, MavState *_comm,exec::task *_actualTask) :
            Command(_state, _comm, _actualTask){}

    void execute() override {
        takeoff();
    }

};
#endif //MOCAP2MAV_TAKEOFF_HPP
