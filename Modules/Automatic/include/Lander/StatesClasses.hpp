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
    MavState *_state;

};

class AbstractLandState : public AbstractState {

public:
    AbstractState* _nextState;
    enum states{

        INIT,
        HOLD,
        DESC,
        ASCE

    };

    AbstractLandState(LandMachine *context) : AbstractState(context){
        _contextL = context;
        init(_contextL->_horizontaErr,_contextL->_tauHold,_contextL->_tauLost,_contextL->_tauErr,
             _contextL->_NHold,_contextL->_NLost,_contextL->_state);

    }

    double*  _horizontaErr;
    double*  _tauHold;
    double*  _tauLost;
    double*  _tauErr;
    int*     _NHold;
    int*     _NLost;
    MavState* _state;

protected:
    LandMachine* _contextL;
    void init(double* hErr, double* tHold, double* tLost, double* tErr, int* nHold, int* nLost, MavState* state){

            //Attach external signals
            // _horizontaErr = hErr;
            //_tauHold      = tHold;
            //_tauLost      = tLost;
            //_tauErr       = tErr;
            //_NHold        = nHold;
            _NLost          = nLost;
            //_state        = state;

    }


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


#endif //MOCAP2MAV_STATESCLASSES_HPP
