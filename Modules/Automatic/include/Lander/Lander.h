//
// Created by andreanistico on 06/02/17.
//

#ifndef MOCAP2MAV_LANDER_H
#define MOCAP2MAV_LANDER_H

#include "StateMachine/include/Machine.h"
#include "common/MavState.h"
#include "StatesClasses.hpp"

//Define states



class Lander {

private:
    //LandMachine  _machine;
    MavState _state;
    MavState _setPoint;



public:
    Lander();
    void test();

    void setState(MavState pose);
    MavState getState();
    MavState getCommand();

    void init();
    void hold();
    void desc();
    void asce();

};


#endif //MOCAP2MAV_LANDER_H
