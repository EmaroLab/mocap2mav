//
// Created by andreanistico on 06/02/17.
//

#include <iostream>
#include "include/Lander/StatesClasses.hpp"
#include "Lander/Lander.h"
#include "parameters.h"

Lander::Lander()
        : _horizontaErr((double)0)    , _tauHold((double)0), _tauLost((double)0), _tauErr((double)0), _NHold(0),
          _NLost(0), _initS(&_machine), _holdS(&_machine)  , _asceS(&_machine)  , _descS(&_machine) ,_err(0,0),
          _err_int(0,0), _err_diff(0,0), _dt(0), _prevTime(0), _actualTime(0)
{

    initStateMachine();

}

void Lander::setState(MavState pose) {
    _state = pose;
}

MavState Lander::getState() {
    return _state;
}

MavState Lander::getCommand() {
    return _setPoint;
}

void Lander::initStateMachine() {

    //Link signals
    _machine._horizontaErr =  &_horizontaErr;
    _machine._tauErr       =  &_tauErr;
    _machine._tauHold      =  &_tauHold;
    _machine._tauLost      =  &_tauLost;
    _machine._NLost        =  &_NLost;
    _machine._NHold        =  &_NHold;
    _machine._NComp        =  &_NComp;
    _machine._state        =  &_state;
    _machine._setPoint     =  &_setPoint;

    //Link states
    _initS._nextState    = &_holdS;
    _holdS._nextAscState = &_asceS;
    _holdS._nextDesState = &_descS;
    _asceS._nextState    = &_holdS;
    _descS._nextState    = &_holdS;

    _machine.setStatePtr(&_initS);

    _tauHold = 0.5 * params_automatic::platformLenght;
    _tauLost = params_automatic::platformLenght;

#ifdef DEBUG
    //Print actual state
    std::cout << "Actual state: " << _machine.getActualNodeId() << std::endl;
#endif
}

void Lander::setPlatformState(const MavState platformState) {
    _platformState = platformState;
}

void Lander::updateSignals() {

    int state = _machine.getActualNodeId();
    //Compute horizontal error
    double xTemp = _state.getX();
    double yTemp = _state.getY();
    double xPlatTemp = _platformState.getX();
    double yPlatTemp = _platformState.getY();

    double dx = xPlatTemp - xTemp;
    double dy = yPlatTemp - yTemp;

    _err[0] = dx;
    _err[1] = dy;

    _horizontaErr = _err.norm();

    if( state == AbstractLandState::states::HOLD ||
        state == AbstractLandState::states::ASCE ||
        state == AbstractLandState::states::DESC){

        //Increment N if needed
        if (_horizontaErr < _tauHold) {
            _NLost = 0;
            _NHold++;
        }
        else if (_horizontaErr > _tauLost) {
            _NHold = 0;
            _NLost++;
        }
    }

#ifdef DEBUG
    std::cout << "HERRO: " << _horizontaErr<< std::endl;
    std::cout << "NHOLD: " << _NHold<< std::endl;
    std::cout << "NLOST: " << _NLost<< std::endl;
#endif

}

void Lander::handleMachine() {
    _machine.handle();
}

int Lander::getActualMachineState() {
    return _machine.getActualNodeId();
}

void Lander::resetSetPoint() {

    _setPoint = _state;
    _setPoint.setType(MavState::type::POSITION);

}

void Lander::hold() {

    //This function is purely tracking, nothing more

    /*
     * The tracking is performed by POSITION control, issuing the position setpoint
     * in order to achieve the desired velocity calculated by:
     *
     * Vdes = K * ep + Vplat
     * or
     * Psp = Pplat + K * Vplat
     *
     * where:
     *
     * Vdes = desired velocity, K = proportional gain, ep = position error, Vplat = paltform velocity
     */

    //Cache values
    MavState platPos = _platformState;
    MavState state   = _state;

    Eigen::Vector2d tempVel(platPos.getVx(),platPos.getVy());
    Eigen::Vector2d tempSetPoint(platPos.getX(),platPos.getY());

    updateIntegrals();
    //PosSP = PlatPos + K * Vplat
    tempSetPoint += params_automatic::KpHold * tempVel + params_automatic::KiHold * _err_int;

    //Fill right fields
    _setPoint.setPosition(tempSetPoint(0),tempSetPoint(1),_setPoint.getZ());
    _setPoint.setType(MavState::POSITION);

}

void Lander::init() {

    //Set point to my position
    resetSetPoint();

    //TODO: improve height logic(we assume that we are safely flying)
    //Go to max tracking height
    _setPoint.setZ(-params_automatic::zMax);

}

void Lander::run() {

    managetime();
    int state = getActualMachineState();
    static bool initDone = false;
    switch (state){

        case (AbstractLandState::states::INIT):
            if(!initDone){
                init();
                initDone = true;
            }
            break;

        case (AbstractLandState::states::HOLD):
            initDone = false;
            hold();
            break;
        case (AbstractLandState::states::DESC):

            desc();
            break;
        case (AbstractLandState::states::ASCE):

            asce();
            break;

        default:
            hold();
            break;

    }

}

void Lander::updateIntegrals() {

    _err_int[0] +=  _dt * _err[0];
    _err_int[1] +=  _dt * _err[1];

}

void Lander::managetime() {

    _actualTime = common::time::getTimeMilliSecond();
    double dt = _prevTime != 0 ? (_actualTime - _prevTime) * (double)MILLI2SECS : 0.0;
    _prevTime = _actualTime;
    _dt = dt;

}

void Lander::resetIntegrals() {

    _err_int[0] = 0;
    _err_int[1] = 0;

}

void Lander::asce() {

    resetIntegrals();
    _setPoint.setZ(_setPoint.getZ() - 0.1);

}

void Lander::desc() {

    //Add step because z is negative up
    _setPoint.setZ(_setPoint.getZ() + 0.1);
}

void Lander::comp() {

}








