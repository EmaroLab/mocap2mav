//
// Created by andreanistico on 08/02/17.
//

#include "include/Lander/StatesClasses.hpp"
#include "parameters.h"
//Define actual states

void InitState::handle(){

    //Got everything (assume it)
    getSignals();
    static int wait = 0;

    //Wait 100 iterations
    if(wait++ > 100) {
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
    }
}
void HoldState::handle(){
    getSignals();

    bool holding = (_NHold > params_automatic::NFramesHold);
    bool lost    = (_NLost > params_automatic::NFramesLost);

    bool descValid = holding && (_setPoint.getZ()   > params_automatic::zMin + 0.1);
    bool asceValid = lost    && (_setPoint.getZ()   < params_automatic::zMax - 0.1);
    bool compValid = holding && (fabs(_state.getZ() - params_automatic::zMin) < 0.2) && (_horizontaErr < _tauHold * 0.5);

    if(descValid){
        this->_contextL->setStatePtr(_nextDesState);
        printStateTransition();
        return;
    }
    if (asceValid){
        this->_contextL->setStatePtr(_nextAscState);
        printStateTransition();
        return;
    }
    if(compValid){
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
        return;
    }

}
void DescState::handle() {
    getSignals();
    this->_contextL->setStatePtr(_nextState);
    printStateTransition();
}
void AsceState::handle() {
    getSignals();
    this->_contextL->setStatePtr(_nextState);
    printStateTransition();
}

void CompState::handle() {
    getSignals();
    if (_NComp < params_automatic::NFramesComp || _horizontaErr > _tauHold * 0.5){
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
        return;
    }
}

void RToLandState::handle() {

    getSignals();
    if (_NComp > params_automatic::NFramesComp){
        this->_contextL->setStatePtr(_nextComState);
        printStateTransition();
        return;
    } else if (_horizontaErr > _tauHold * 0.5){
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
        return;
    }

}

void LandState::handle() {
}
