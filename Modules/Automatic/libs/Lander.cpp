//
// Created by andreanistico on 06/02/17.
//

#include <iostream>
#include <include/Lander/StatesClasses.hpp>
#include "Lander/Lander.h"

#define PLATFORM_LENGHT 1.0;
#define TAU_N           100;
#define TAU_HOLD        100;


Lander::Lander()
        : _horizontaErr((double)0), _tauHold((double)0), _tauLost((double)0), _tauErr((double)0), _NHold(0),
          _NLost(0), _initS(&_machine), _holdS(&_machine), _asceS(&_machine),_descS(&_machine){

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

    //Link states
    _initS._nextState    = &_holdS;
    _holdS._nextAscState = &_asceS;
    _holdS._nextDesState = &_descS;
    _asceS._nextState    = &_holdS;
    _descS._nextState    = &_holdS;

    _machine.setStatePtr(&_initS);

    _tauHold = 0.5*PLATFORM_LENGHT;
    _tauLost = PLATFORM_LENGHT;

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

    Eigen::Vector2d err(dx,dy);

    _horizontaErr = err.norm();

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

}

void Lander::handleMachine() {
    _machine.handle();

}







