//
// Created by andreanistico on 06/02/17.
//

#include <iostream>
#include <include/Lander/StatesClasses.hpp>
#include "Lander/Lander.h"

Lander::Lander()
        : _horizontaErr((double)0), _tauHold((double)0), _tauLost((double)0), _tauErr((double)0), _NHold(0),
          _NLost(0) {}

void Lander::setState(MavState pose) {

    _state = pose;

}

MavState Lander::getState() {
    return _state;
}

MavState Lander::getCommand() {
    return _setPoint;
}

void Lander::init() {
    std::cout << "INIT" << std::endl;
}

void Lander::hold() {
    std::cout << "HOLD" << std::endl;
}

void Lander::desc() {

}

void Lander::asce() {

}

void Lander::test() {

    int a = 3;

    LandMachine m;
    //Create states
    InitState init(&m);
    HoldState hold(&m);
    HoldState asce(&m);
    HoldState desc(&m);

    //Link states
    init._nextState    = &hold;
    hold._nextAscState = &asce;
    hold._nextDesState = &desc;
    asce._nextState    = &hold;
    desc._nextState    = &hold;

    m.setStatePtr(&init);

    std::cout << m.getActualNodeId() << std::endl;
    m.handle();
    std::cout << m.getActualNodeId() << std::endl;
    m.handle();
    std::cout << m.getActualNodeId() << std::endl;
    m.handle();
    std::cout << m.getActualNodeId() << std::endl;
    m.handle();
    std::cout << m.getActualNodeId() << std::endl;
    m.handle();
    std::cout << m.getActualNodeId() << std::endl;
    m.handle();
    std::cout << m.getActualNodeId() << std::endl;

}


