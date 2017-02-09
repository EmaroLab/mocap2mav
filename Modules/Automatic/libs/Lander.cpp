//
// Created by andreanistico on 06/02/17.
//

#include <iostream>
#include "Lander/Lander.h"

Lander::Lander()  {


   // std::cout << *_machine._NLost << std::endl;
    //InitState init(&_machine,states::INIT);
    //_machine.setStatePtr(new InitState(&_machine,states::INIT));
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

  //  _machine._NLost = &a;
    LandMachine m;

    InitState in(&m,0);

    m.setStatePtr( AbstractStatePtr(new InitState(&m,states::INIT)));

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


