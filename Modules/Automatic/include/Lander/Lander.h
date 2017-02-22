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

public:
    Lander();
    void setPlatformState(const MavState platformState);
    void initStateMachine();
    void setState(MavState pose);
    void updateSignals();
    void handleMachine();
    int getActualMachineState();
    MavState getState();
    MavState getCommand();
    void resetSetPoint();

private:

    LandMachine  _machine;
    MavState _state;
    MavState _setPoint;
    MavState _platformState;

    //Create states
    InitState _initS;
    HoldState _holdS;
    AsceState _asceS;
    DescState _descS;

    double _horizontaErr;
    double _tauHold;
    double _tauLost;
    double _tauErr;
    int _NHold;
    int _NLost;

    //Tracking logic defined here
    void hold();
    //Initialize stuff here
    void init();

};


#endif //MOCAP2MAV_LANDER_H
