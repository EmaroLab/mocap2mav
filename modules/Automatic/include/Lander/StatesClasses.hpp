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
    double *_verticalErr;
    double *_tauHold;
    double *_tauLost;
    double *_tauErr;
    int *_NHold;
    int *_NLost;
    int      *_NComp;
    MavState *_state;
    MavState *_setPoint;

    //New Signals
    bool *_holding;
    bool *_lost;
    bool *_centered;

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
        COMP,
        LAND

    };

    AbstractLandState(LandMachine *context) : AbstractState(context){
        _contextL = context;
    }

    void getSignals(){

         _horizontaErr = *(_contextL->_horizontaErr);
         _verticalErr  = *(_contextL->_verticalErr);
         _tauHold      = *(_contextL->_tauHold);
         _tauLost      = *(_contextL->_tauLost);
         _tauErr       = *(_contextL->_tauErr);
         _NHold        = *(_contextL->_NHold);
         _NLost        = *(_contextL->_NLost);
         _NComp        = *(_contextL->_NComp);
         _state        = *(_contextL->_state);
         _setPoint     = *(_contextL->_setPoint);

        //New Signals
        _holding       = *(_contextL->_holding);
        _centered      = *(_contextL->_centered);
        _lost          = *(_contextL->_lost);

    }
    void printStateTransition(){
        std::cout << "Actual state: " << _contextL->getActualNodeId() << std::endl;
    }

protected:

    double   _horizontaErr;
    double   _verticalErr;
    double   _tauHold;
    double   _tauLost;
    double   _tauErr;
    int      _NHold;
    int      _NLost;
    int      _NComp;
    MavState _state;
    MavState _setPoint;
    LandMachine* _contextL;

    //New Signals
    bool _centered;
    bool _holding;
    bool _lost;
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
    AbstractLandState* _nextLanState;
    CompState(LandMachine *context) : AbstractLandState(context) {
        setId();
    }

    void setId() override {

        _id = COMP;

    }
    void handle();
};

class LandState : public AbstractLandState {
public:
    AbstractLandState* _restartState;
    LandState(LandMachine *context) : AbstractLandState(context) {
        setId();
    }

    void setId() override {

        _id = LAND;

    }
    void handle();
};



#endif //MOCAP2MAV_STATESCLASSES_HPP
