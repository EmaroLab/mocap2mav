//
// Created by andreanistico on 08/02/17.
//

#ifndef MOCAP2MAV_STATESCLASSES_HPP
#define MOCAP2MAV_STATESCLASSES_HPP

#include <iostream>
#include "StateMachine/include/Machine.h"
#include "common/MavState.h"


class LandMachine : public Machine {

public:
    double *_horizontaErr;
    double *_tauHold;
    double *_tauLost;
    double *_tauErr;
    int *_NHold;
    int *_NLost;
    int      *_NComp;
    MavState *_state;
    MavState *_setPoint;

};

class AbstractLandState : public AbstractState {

public:
    AbstractState* _nextState;
    enum states{

        INIT,
        HOLD,
        DESC,
        ASCE,
        R2LA,
        COMP

    };

    AbstractLandState(LandMachine *context) : AbstractState(context){
        _contextL = context;
    }

    void getSignals(){

         _horizontaErr = *(_contextL->_horizontaErr);
         _tauHold      = *(_contextL->_tauHold);
         _tauLost      = *(_contextL->_tauLost);
         _tauErr       = *(_contextL->_tauErr);
         _NHold        = *(_contextL->_NHold);
         _NLost        = *(_contextL->_NLost);
         _NComp        = *(_contextL->_NComp);
         _state        = *(_contextL->_state);
         _setPoint     = *(_contextL->_setPoint);

    }
    void printStateTransition(){
        std::cout << "Actual state: " << _contextL->getActualNodeId() << std::endl;
    }

protected:

    double   _horizontaErr;
    double   _tauHold;
    double   _tauLost;
    double   _tauErr;
    int      _NHold;
    int      _NLost;
    int      _NComp;
    MavState _state;
    MavState _setPoint;
    LandMachine* _contextL;
};

class InitState : public AbstractLandState {
public:
    InitState(LandMachine *context) :  AbstractLandState(context){
        setId();
    }
    void setId() override {

        _id = INIT;

    }
    void handle();
};

class HoldState : public AbstractLandState {
public:
    AbstractLandState* _nextAscState;
    AbstractLandState* _nextDesState;

    HoldState(LandMachine *context) : AbstractLandState(context) {
        setId();
    }
    void setId() override {

        _id = HOLD;

    }
    void handle();
};

class DescState : public AbstractLandState {
public:
    DescState(LandMachine *context) : AbstractLandState(context) {
        setId();
    }
    void setId() override {

        _id = DESC;

    }
    void handle();
};

class AsceState : public AbstractLandState {
public:
    AsceState(LandMachine *context) : AbstractLandState(context) {
        setId();
    }

    void setId() override {

        _id = ASCE;

    }
    void handle();
};
class RToLandState : public AbstractLandState {
public:
    AbstractLandState* _nextComState;
    RToLandState(LandMachine *context) : AbstractLandState(context) {
        setId();
    }

    void setId() override {

        _id = R2LA;

    }
    void handle();
};
class CompState : public AbstractLandState {
public:
    CompState(LandMachine *context) : AbstractLandState(context) {
        setId();
    }

    void setId() override {

        _id = COMP;

    }
    void handle();
};



#endif //MOCAP2MAV_STATESCLASSES_HPP
