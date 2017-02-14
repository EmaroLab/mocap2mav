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
    LandMachine  _machine;
    MavState _state;
    MavState _setPoint;

    double _horizontaErr;
    double _tauHold;
    double _tauLost;
    double _tauErr;
    int _NHold;
    int _NLost;

public:

    Lander();
    void test();

    void setState(MavState pose);
    MavState getState();
    MavState getCommand();



};


#endif //MOCAP2MAV_LANDER_H
