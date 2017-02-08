//
// Created by andreanistico on 08/02/17.
//

#include "StatesClasses.hpp"

//Define actual states

void InitState::handle(){

    //Got everything (assume it)
    std::cout << "INIT HANDLE" << std::endl;
   // HoldState* ho = new HoldState(this->_contextL,1);

    this->_context->setStatePtr(new HoldState(this->_contextL,states::HOLD));

}

void HoldState::handle(){
    std::cout << "HOLD HANDLE" << std::endl;
    _context->setStatePtr(new InitState(_contextL, states::INIT));
    /*
    if ((*_NHold >= *_tauHold) && (_horizontaErr <= _tauErr) ){
        _context->setStatePtr(new DescState(_context,states::DESC));
    }
     */

}
void DescState::handle() {}
void AsceState::handle() {}