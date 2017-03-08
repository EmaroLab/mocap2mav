//
// Created by andreanistico on 06/02/17.
//

#include <iostream>
#include "include/Lander/StatesClasses.hpp"
#include "Lander/Lander.h"
#include "parameters.h"

Lander::Lander()
        : _horizontaErr((double)0)    , _tauHold((double)0), _tauLost((double)0), _tauErr((double)0), _NHold(0),
          _NLost(0), _initS(&_machine), _holdS(&_machine)  , _asceS(&_machine)  , _descS(&_machine),_compS(&_machine),
          _err(0,0), _err_int(0,0), _err_diff(0,0), _dt(0), _prevTime(0), _actualTime(0)
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
    _holdS._nextComState = &_compS;
    _asceS._nextState    = &_holdS;
    _descS._nextState    = &_holdS;
    _compS._nextState    = &_asceS;

    _machine.setStatePtr(&_initS);

    _tauHold = 0.5 * params_automatic::platformLenght;
    _tauLost = params_automatic::platformLenght;

    //Print actual state
    std::cout << "Actual state: " << _machine.getActualNodeId() << std::endl;

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
        state == AbstractLandState::states::DESC ||
        state == AbstractLandState::states::COMP){

        //Increment N if needed
        if (_horizontaErr < _tauHold) {
            _NLost = 0;
            _NHold++;
        }
        else if (_horizontaErr > _tauLost) {
            _NHold = 0;
            _NComp = 0;
            _NLost++;
        }
/*
        if(state == AbstractLandState::states::HOLD) {

            if ((fabs(_state.getZ() - (-params_automatic::zMin)) < 0.1) && _horizontaErr < _tauHold) {
                _NComp++;
            } else{
                _NComp = 0;
            }
        }
*/
        if(state == AbstractLandState::states::COMP)  {

            //Check whether we are on place to land
            if (_horizontaErr < _tauHold) {
                _NComp++;
            } else{
                _NComp = 0;
            }

        }

    }

#ifdef DEBUG
    std::cout << "HERRO: " << _horizontaErr<< std::endl;
    std::cout << "NHOLD: " << _NHold<< std::endl;
    std::cout << "NLOST: " << _NLost<< std::endl;

    std::cout << "INTEX: " << _err_int[0] << std::endl;
    std::cout << "INTEY: " << _err_int[1] << std::endl;
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
            clampZSP();
            hold();
            break;
        case (AbstractLandState::states::DESC):

            desc();
            clampZSP();
            break;
        case (AbstractLandState::states::ASCE):

            asce();
            clampZSP();
            break;
        case (AbstractLandState::states::COMP):

            comp();
            break;

        default:
            hold();
            break;

    }

}

void Lander::updateIntegrals() {

    _err_int[0] +=  _dt * _err[0];
    _err_int[1] +=  _dt * _err[1];

    double tempx,tempy;

    tempx = common::clamp(_err_int[0],params_automatic::minIntValue,params_automatic::maxIntValue);
    tempy = common::clamp(_err_int[1],params_automatic::minIntValue,params_automatic::maxIntValue);

    _err_int[0] =  tempx;
    _err_int[1] =  tempy;

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
    _setPoint.setZ(_setPoint.getZ() + 0.1);

}

void Lander::desc() {

    //Add step because z is negative up
    _setPoint.setZ(_setPoint.getZ() - 0.1);
}

void Lander::comp() {

    hold();

    MavState tempState = _state;
    //Calculate desired vertical velocity in order to compensate oscillations
    double desc = 0.07;
    double z_target_v = _platformState.getVz() - desc;

    double err_v = z_target_v - tempState.getVz();

    double corr = 1;

    if(_platformState.getVz() > 0) corr = 1.1;

    z_target_v += corr * params_automatic::KPComp * (err_v);

    //Now we need to transform this velocity in a position setpoint since in Firmware:
    // VelSP = Kp * PosError then PosSP = ( VelSP / Kp ) + RobotPos

    _setPoint.setZ((z_target_v) + tempState.getZ());

    //_setPoint.setZ(-_platformState.getZ() - 0.6);
}

void Lander::clampZSP() {

    double temp;
    temp = common::clamp(fabs(_setPoint.getZ()),params_automatic::zMin,params_automatic::zMax);

    _setPoint.setZ(-temp);
}








