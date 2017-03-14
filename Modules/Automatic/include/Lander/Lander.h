//
// Created by andreanistico on 06/02/17.
//

#ifndef MOCAP2MAV_LANDER_H
#define MOCAP2MAV_LANDER_H

#include "StateMachine/include/Machine.h"
#include "common/MavState.h"
#include "StatesClasses.hpp"
#include "common/common.h"

#define DEBUG
#define PI              3.141592653589
#define Kland           1
#define THRE            0.15
#define DRATE_MIN       0.1
#define DRATE_MAX       0.4
#define VMAX            1.5
#define TMAX            2
#define TMIN            0.5
#define PLATFORM_OFFSET 0.1

class Lander {

public:
    Lander();

    void setPlatformState(const MavState platformState);
    void initStateMachine();
    void setState(MavState pose);
    void handleMachine();
    void updateSignals();
    int getActualMachineState();
    MavState getState();
    MavState getCommand();
    void run();

private:

    LandMachine  _machine;
    MavState _state;
    MavState _setPoint;
    MavState _platformState;

    //Create states
    InitState       _initS;
    HoldState       _holdS;
    AsceState       _asceS;
    DescState       _descS;
    RToLandState    _rtolS;
    CompState       _compS;
    LandState       _landS;

    int _actualState;
    int _prevState;

    //Time helpers
    double _dt;
    uint64_t _actualTime;
    uint64_t _prevTime;

    //Signals
    double _horizontaErr;
    double _verticalErr;
    double _tauHold;
    double _tauLost;
    double _tauErr;
    int    _NHold;
    int    _NLost;
    int    _NComp;

    //Errors
    Eigen::Vector3d _err_int;
    Eigen::Vector3d _err;
    Eigen::Vector3d _err_prev;
    Eigen::Vector3d _err_diff;

    //Logic and signals helpers
    void updateIntegrals();
    void resetIntegrals();
    void managetime();

    //Clamp z SP
    void clampZSP();

    //Reset position setpoint to actual position
    void resetSetPoint();

    //Initialize stuff here
    void init();
    //Tracking logic defined here
    void hold(bool increaseK = false);
    //Go up in case we miss, reset integrals.
    void asce();
    //Get closer to the target
    void desc();
    //Compensate altitude oscillation before landing
    void comp();
    //Aggressive land
    void land();

};

#endif //MOCAP2MAV_LANDER_H
