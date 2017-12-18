//
// Created by andreanistico on 06/02/17.
//

#include <iostream>
#include "StatesClasses.hpp"
#include "Lander.h"
#include "parameters.h"
#include "common/conversions.h"

Lander::Lander()
        : _horizontaErr((double)0)    , _tauHold((double)0), _tauLost((double)0), _tauErr((double)0), _NHold(0),
          _NLost(0),_NComp(0), _initS(&_machine), _holdS(&_machine)  , _asceS(&_machine)  , _descS(&_machine),_compS(&_machine),
          _rtolS(&_machine),_landS(&_machine), _err(0,0,0), _err_int(0,0,0), _err_diff(0,0,0), _dt(0), _prevTime(0), _actualTime(0),
          _actualState(0),_prevState(0), _verticalErr(0), _holdPIDX(params_automatic::KpHold,params_automatic::KiHold,params_automatic::KdHold),
          _holdPIDY(params_automatic::KpHold,params_automatic::KiHold,params_automatic::KdHold)
{


    //Load parameters


    initStateMachine();
    _actualState = _machine.getActualNodeId();
    _err_prev = _err;
    _holdPIDX.setMaxIOutput(params_automatic::maxIntValue);
    _holdPIDY.setMaxIOutput(params_automatic::maxIntValue);

    _holdPIDX.setOutputLimits(params_automatic::maxOutput);

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
    _machine._verticalErr  =  &_verticalErr;
    _machine._tauErr       =  &_tauErr;
    _machine._tauHold      =  &_tauHold;
    _machine._tauLost      =  &_tauLost;
    _machine._NLost        =  &_NLost;
    _machine._NHold        =  &_NHold;
    _machine._NComp        =  &_NComp;
    _machine._state        =  &_state;
    _machine._setPoint     =  &_setPoint;

    //New signals
    _machine._holding      =  &_holding;
    _machine._centered     =  &_centered;
    _machine._lost         =  &_lost;


    //Link states
    _initS._nextState    = &_holdS;

    _holdS._nextAscState = &_asceS;
    _holdS._nextDesState = &_descS;
    _holdS._nextState    = &_rtolS;

    _asceS._nextState    = &_holdS;

    _descS._nextState    = &_holdS;

    _rtolS._nextComState = &_compS;
    _rtolS._nextState    = &_holdS;

    _compS._nextState    = &_asceS;
    _compS._nextLanState = &_landS;

    _landS._nextState    = &_asceS;

    _machine.setStatePtr(&_initS);

    _tauHold = 0.5 * params_automatic::platformLenght;
    _tauLost = params_automatic::platformLenght * 0.7;

    //Print actual state
    std::cout << "Actual state: " << _machine.getActualNodeId() << std::endl;

}

void Lander::setPlatformState(const MavState platformState) {
    _platformState = platformState;
}

void Lander::updateSignals() {

    //Compute horizontal error
    double xTemp = _state.getX();
    double yTemp = _state.getY();
    double zTemp = _state.getZ();
    double xPlatTemp = _platformState.getX();
    double yPlatTemp = _platformState.getY();
    double zPlatTemp = _platformState.getZ();

    double dx = xPlatTemp - xTemp;
    double dy = yPlatTemp - yTemp;
    double dz = zPlatTemp - zTemp + PLATFORM_OFFSET;

    _err[0] = dx;
    _err[1] = dy;
    _err[2] = 0; //Trick, used to calculate horizontal error

    _horizontaErr = _err.norm();
    _err[2]       = dz;
    _verticalErr  = _err[2];

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

    _holding  = (_NHold > params_automatic::NFramesHold);
    _lost     = (_NLost > params_automatic::NFramesLost);
    _centered = _horizontaErr < _tauHold * 0.5;

    if(_actualState == AbstractLandState::states::R2LA || _actualState == AbstractLandState::states::COMP || _actualState == AbstractLandState::states::LAND){

        //Reset NComp
        if(_prevState == AbstractLandState::states::HOLD) _NComp = 0;

        //Check whether we are on place to land
        if (_centered) {
            _NComp++;
        } else{
            _NComp = 0;
            _NHold = 0;
        }
    }
    //Check if we are escaping to ascending
    if(_actualState == AbstractLandState::states::ASCE && (_prevState == AbstractLandState::states::COMP ||
                                                           _prevState == AbstractLandState::states::LAND))
    {
        _NComp = 0;
        _NHold = 0;
    }


    _err_prev = _err;

#ifdef DEBUG

    std::cout << "**********************************" << std::endl;
    std::cout << "STATE: " << _actualState<< std::endl;
    std::cout << "HERRO: " << _horizontaErr<< std::endl;
    std::cout << "HERRV: " << _verticalErr<< std::endl;
    std::cout << "NHOLD: " << _NHold<< std::endl;
    std::cout << "NLOST: " << _NLost<< std::endl;
    std::cout << "NCOMP: " << _NComp<< std::endl;
    std::cout << "INTEX: " << _err_int[0] << std::endl;
    std::cout << "INTEY: " << _err_int[1] << std::endl;
    std::cout << "**********************************" << std::endl;
#endif


}

void Lander::handleMachine() {

    updateSignals();
    _machine.handle();
}

int Lander::getActualMachineState() {
    return _machine.getActualNodeId();
}

void Lander::run() {

    _prevState = _actualState;
    handleMachine();
    _actualState = _machine.getActualNodeId();
    managetime();

    static bool initDone = false;
    switch (_actualState){

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

        case (AbstractLandState::states::R2LA):
            clampZSP();
            hold();
            break;

        case (AbstractLandState::states::COMP):
            hold();
            comp();
            break;
        case (AbstractLandState::states::LAND):
            land();
            break;

        default:
            hold();
            break;

    }


}

void Lander::resetSetPoint() {

    _setPoint = _state;
    _setPoint.setType(MavState::type::POSITION);

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

void Lander::init() {

    //Set point to my position
    resetSetPoint();

    //Go to max tracking height
    _setPoint.setZ(params_automatic::zMax);

}

void Lander::hold() {

    //This function is purely tracking, nothing more

    /*
     * Tracking is performed by POSITION control, issuing the position setpoint
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

    Eigen::Vector2d tempVel(_platformState.getVx(),_platformState.getVy());

    _holdPIDX.setDt(_dt);
    _holdPIDY.setDt(_dt);

    double xTarget = _holdPIDX.getOutput(_state.getX(), _platformState.getX());
    double yTarget = _holdPIDY.getOutput(_state.getY(), _platformState.getY());
    Eigen::Vector2d targetVect(xTarget + _platformState.getX(),yTarget + _platformState.getY());


    updateIntegrals();
    //PosSP = PlatPos + K * Vplat
    targetVect += params_automatic::KpHoldV * tempVel;

    //Fill right fields
    _setPoint.setPosition(targetVect(0),targetVect(1),_setPoint.getZ());
    _setPoint.setType(MavState::POSITION);

}

void Lander::asce() {

    _holdPIDX.reset();
    _holdPIDY.reset();
    _setPoint.setZ(_setPoint.getZ() + 0.1);

}

void Lander::desc() {


    _setPoint.setZ(_setPoint.getZ() - 0.1);
}

void Lander::comp() {

    //Calculate desired vertical velocity in order to compensate oscillations
    double dz = - _state.getZ() + _platformState.getZ() + PLATFORM_OFFSET;
    double desc = common::interpolate(fabs(dz), DRATE_MAX, DRATE_MIN, TMAX, TMIN);
    double z_target_v = _platformState.getVz() - desc;
    double err_v = z_target_v - _state.getVz();

    z_target_v += params_automatic::KPCompV * (err_v);

    //Now we need to transform this velocity in a position setpoint since in Firmware:
    // VelSP = Kp * PosError then PosSP = ( VelSP / Kp ) + RobotPos

    _setPoint.setZ((z_target_v) + _state.getZ());

}

void Lander::clampZSP() {

    double temp;
    temp = common::clamp(_setPoint.getZ(),params_automatic::zMin,params_automatic::zMax);

    _setPoint.setZ(temp);
}

void Lander::land() {

    resetSetPoint();
    _setPoint.setZ(_state.getZ()-10);

}

void Lander::allign() {

    //Continuosly change yaw value
    double yaw_target = _platformState.getYawFromQuat();
    Eigen::Quaterniond q_interm = yawToQuaternion(yaw_target);

    _setPoint.setOrientation(q_interm);

}

void Lander::loadParam(Parameters *p) {

}
