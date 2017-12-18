//
// Created by andreanistico on 24/10/17.
//


#include "Parameters.h"

Parameters::Parameters(const char* config_file) {
        loadConfigFile(config_file);

    }
Parameters::Parameters() {

        //Throw a bunch of default params
        _ini_loaded = false;
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
     void Parameters::loadConfigFile(const char *config_file) {
         _ini_loaded = _ini.load(config_file);
     }


    //Read each parameter and store it in a local variable
    void Parameters::updateParams(){
        if(_ini_loaded){

            NFramesHold = _ini.getIntValue("NFramesHold");
            NFramesLost = _ini.getIntValue("NFramesLost");
            NFramesComp = _ini.getIntValue("NFramesComp");
            platformLenght = _ini.getDoubleValue("platformLenght");
            zMax = _ini.getDoubleValue("zMax");
            zMin = _ini.getDoubleValue("zMin");
            KpHoldV = _ini.getDoubleValue("KpHoldV");
            KpHold = _ini.getDoubleValue("KpHold");
            KdHold = _ini.getDoubleValue("KdHold");
            KiHold = _ini.getDoubleValue("KiHold");
            KPCompV = _ini.getDoubleValue("KPCompV");
            maxIntValue = _ini.getDoubleValue("maxIntValue");
            minIntValue = -maxIntValue;
            maxOutput = _ini.getDoubleValue("maxOutput");

        } else
            std::cout << "Please load a config file" << std::endl;

    }