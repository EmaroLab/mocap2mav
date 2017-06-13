//
// Created by andreanistico on 13/06/17.
//

#ifndef MOCAP2MAV_MOVE_HPP
#define MOCAP2MAV_MOVE_HPP

#include "Command.hpp"
class Move : protected Command{

private:

    void move()
    {
        _comm->setType(MavState::type::POSITION);
        double alpha = _actualTask->params[0];
        *(this->_comm) = calculatePositionInterm(alpha,*_actualTask,*_state);
    }

    MavState calculatePositionInterm(const double alpha, const exec::task target, const MavState state)
    {

        double positionError[3] = {target.x - state.getX() ,target.y - state.getY() , target.z - state.getZ()};
        MavState comm;
        double dist = sqrt(pow(positionError[0],2) + pow(positionError[1],2) + pow(positionError[2],2));

        //Publish
        if(fabs(dist) <= alpha)
        {
            comm.setX((float)target.x);
            comm.setY((float)target.y);
            comm.setZ((float)target.z);
        }

        else if(fabs(dist) > alpha)
        {

            double incrementVect[3];
            //Normalize
            positionError[0] = positionError[0] / dist;
            positionError[1] = positionError[1] / dist;
            positionError[2] = positionError[2] / dist;

            //Calculate relative motion to actual position
            incrementVect[0] = positionError[0] * alpha;
            incrementVect[1] = positionError[1] * alpha;
            incrementVect[2] = positionError[2] * alpha;

            comm.setX(state.getX() + (float)incrementVect[0]);
            comm.setY(state.getY() + (float)incrementVect[1]);
            comm.setZ(state.getZ() + (float)incrementVect[2]);
        }
        return comm;

    }

public:
    Move(MavState *_state, MavState *_comm,exec::task *_actualTask) : Command(_state, _comm, _actualTask) {}

    void execute() override {
        move();
    }

};
#endif //MOCAP2MAV_MOVE_HPP
