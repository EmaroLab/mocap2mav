//
// Created by andreanistico on 08/02/17.
//

#include "Lander/StatesClasses.hpp"
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

    bool descValid = _holding && (_setPoint.getZ()   > params_automatic::zMin + 0.1);
    bool asceValid = _lost    && (_setPoint.getZ()   < params_automatic::zMax - 0.1);
    bool compValid = _holding && (fabs(_state.getZ() - params_automatic::zMin) < 0.2) && _centered;

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

    bool onTarget = _NComp > params_automatic::NFramesComp;

    if (!onTarget || !_centered){
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
    }

   std::cout << "VERRRRRRRR: " << _verticalErr << std::endl;

    if(onTarget && _centered && (fabs(_verticalErr) < 0.20)){
        this->_contextL->setStatePtr(_nextLanState);
        printStateTransition();
    }

}

void RToLandState::handle() {

    getSignals();
    if (_NComp > params_automatic::NFramesComp){
        this->_contextL->setStatePtr(_nextComState);
        printStateTransition();
        return;
    } else if (!_centered){
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
        return;
    }

}

void LandState::handle() {

    getSignals();
    bool onTarget = _NComp > params_automatic::NFramesComp;

    if (!onTarget || !_centered){
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
    }

}
