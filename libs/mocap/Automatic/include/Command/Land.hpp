//
// Created by andreanistico on 13/06/17.
//

#ifndef MOCAP2MAV_LAND_HPP
#define MOCAP2MAV_LAND_HPP

#include "Command.hpp"
#include "common/conversions.h"
#include "../Lander/Lander.h"

class Land : public Command{

private:

    double _xin;
    double _yin;
    double _yawin;
    int    _plat;
    MavState* _platformPose;
    Lander _lander;

    double calculateDescendRate(double dz,double drate_max,double drate_min, double tmax, double tmin){

        //Calculate the descend rate profile (linear) through y = mx + q
        //where x is vertical distance between robot and platform
        if(dz > tmax)      return drate_max;
        else if(dz < tmin) return drate_min;
        else{

            double m = (drate_max - drate_min) / (tmax - tmin);
            double q = drate_min - m * tmin;
            return m * dz + q;

        }
    }

    void simpleLanding(float x_target, float y_target, float h) {

        //Calculate difference

        double dx = - _state->getX() + x_target;
        double dy = - _state->getY() + y_target;
        double dz = - _state->getZ() + h;

        // Be sure that we are on top of the target

        double x_target_v = Kland * (dx);
        double y_target_v = Kland * (dy);

        _comm->setVx(x_target_v);
        _comm->setVy(y_target_v);
        _comm->setType(MavState::VELOCITY);
        //TODO: add security checks on vz
        if(fabs(dx) <= THRE && fabs(dy) <= THRE){
            //Descending is safe, is it?
            double desc = calculateDescendRate(fabs(dz), DRATE_MAX, DRATE_MIN, TMAX, TMIN);
            _comm->setVz(-desc);

            if (fabs(dz) < THRE){
                _comm->setX(x_target);
                _comm->setY(y_target);
                _comm->setZ(h-10);
                _comm->setType(MavState::POSITION);
            }

        }
        else _comm->setVz(0);

    }

    void land(){

        //Save initial state if we have a new task
        if (_newTask) {
            _xin   = _state->getX();
            _yin   = _state->getY();
            _yawin = _state->getYawFromQuat();
            _plat = (int)_actualTask->params[0];
            _newTask = false;
        }

        if (_plat == 0){
            simpleLanding((float)_xin,(float)_yin,0);
        } else {
            _lander.setState(*_state);
            _lander.setPlatformState(*_platformPose);
            _lander.run();
            *_comm = _lander.getCommand();
        }

    }

public:
    Land(MavState *_state, MavState *_comm,exec::task *_actualTask, MavState* _platform) : Command(_state, _comm, _actualTask) , _plat(_plat), _platformPose(_platform){}

    void execute() override {
        land();
    }
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif //MOCAP2MAV_ROTATE_HPP