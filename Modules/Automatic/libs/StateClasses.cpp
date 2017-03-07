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

    //Wait 200 iterations
    if(wait++ > 100) {
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
    }
}
void HoldState::handle(){
    getSignals();
    if((_NHold > params_automatic::NFramesHold) && (_horizontaErr < _tauHold) && (_setPoint.getZ() < -params_automatic::zMin - 0.1)){
        this->_contextL->setStatePtr(_nextDesState);
        printStateTransition();
        return;
    }
    if ((_NLost > params_automatic::NFramesLost) && (_setPoint.getZ() > -params_automatic::zMax) && (_horizontaErr > _tauLost)){
        this->_contextL->setStatePtr(_nextAscState);
        printStateTransition();
        return;
    }
    if((fabs(_state.getZ() - (-params_automatic::zMin)) < 0.1) && (_horizontaErr < _tauHold * 0.5)){
        this->_contextL->setStatePtr(_nextComState);
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
    if ((_horizontaErr > _tauHold * 0.5)){
        this->_contextL->setStatePtr(_nextState);
        printStateTransition();
        return;
    }
}
