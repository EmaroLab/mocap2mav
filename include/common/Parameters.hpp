//
// Created by andreanistico on 24/10/17.
//

#ifndef MOCAP2MAV_PARAMETERS_HPP
#define MOCAP2MAV_PARAMETERS_HPP

#include <string>
#include "rude/config.h"


class Parameters {

public:
    Parameters(const char* config_file) {
        loadConfigFile(config_file);
    }
    Parameters() : _ini_loaded(false){

        //Number of consecutive frames in which tracking is considered valid
        NFramesHold     = 120;

        //Number of consecutive frames in which tracking is considered not valid
        NFramesLost     = 70;

        //Number of consecutive frames in which tracking is considered valid
        //and robot is ready for compensation
        NFramesComp     = 20;

        //Platform dimension
        platformLenght  = 1;

        //Max altitude for landing procedure
        zMax            = 7;

        //Minimum altitude for landing procedure (before compensating, this value should be above the maximum platform altitude)
        zMin            = 4;

        //Proportional gain times platform velocity
        KpHoldV         = 0.9;

        //Proportional gain times horizontal error
        KpHold          = 1.35; // 0.5

        //Differential gain times horizontal error
        KdHold          = 0.00;

        //Integral gain times integral horizontal error
        KiHold          = 0.02; //0.1

        //Proportional gain for velocity tracking
        KPCompV         = 0.1;

        //Integral clamping values
        maxIntValue     = 5;
        minIntValue     = -maxIntValue;

        //Max total PID output
        maxOutput       = 10;

    }

private:

    rude::Config _ini;
    bool _ini_loaded;

    void loadConfigFile(const char* config_file){
        _ini.load(config_file);
        _ini_loaded = true;
    }

    //Read each parameter and store it in a local variable
    void updateParams(const char* config_file){






    }


public:

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
#endif //MOCAP2MAV_PARAMETERS_HPP
