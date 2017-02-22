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
    if(wait++ > 200) {
        this->_contextL->setStatePtr(_nextState);
    }
}
void HoldState::handle(){
    getSignals();
    if((_NHold > params_automatic::NFramesHold) && (_horizontaErr < _tauHold)){
        this->_contextL->setStatePtr(_nextDesState);
        return;
    }
    if ((_NLost > params_automatic::NFramesLost) && (_setPoint.getZ() < params_automatic::zMax) && (_horizontaErr > _tauLost)){
        this->_contextL->setStatePtr(_nextAscState);
        return;
    }

}
void DescState::handle() {
    getSignals();
    this->_contextL->setStatePtr(_nextState);
}
void AsceState::handle() {
    getSignals();
    this->_contextL->setStatePtr(_nextState);
}