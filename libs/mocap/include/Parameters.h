//
// Created by andreanistico on 15/12/17.
//

#ifndef MOCAP2MAV_PARAMETERS_H
#define MOCAP2MAV_PARAMETERS_H
//
// Created by andreanistico on 24/10/17.
//

#include <string>
#include <iostream>
#include "rude/config.h"


class Parameters {

public:
    Parameters(const char* config_file);
    Parameters();

private:

    rude::Config _ini;
    bool _ini_loaded;

    void loadConfigFile(const char* config_file);

    //Read each parameter and store it in a local variable
    void updateParams();


public:

    bool initLoaded();

    //PARAMETERS FIELDS

    //Number of consecutive frames in which tracking is considered valid
    int    NFramesHold;

    //Number of consecutive frames in which tracking is considered not valid
    int    NFramesLost;

    //Number of consecutive frames in which tracking is considered valid
    //and robot is ready for compensation
    int    NFramesComp;

    //Platform dimension
    double platformLenght;

    //Max altitude for landing procedure
    double zMax;

    //Minimum altitude for landing procedure (before compensating, this value should be above the maximum platform altitude)
    double zMin;

    //Proportional gain times platform velocity
    double KpHoldV;

    //Proportional gain times horizontal error
    double KpHold; // 0.5

    //Differential gain times horizontal error
    double KdHold;

    //Integral gain times integral horizontal error
    double KiHold; //0.1

    //Proportional gain for velocity tracking
    double KPCompV;

    //Integral clamping values
    double maxIntValue;
    double minIntValue;

    //Max total PID output
    double maxOutput;

};

#endif //MOCAP2MAV_PARAMETERS_H
