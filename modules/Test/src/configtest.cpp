//
// Created by andreanistico on 23/10/17.
//

#include <iostream>
#include <string>
#include "rude/config.h"


int main(int argc, char** argv){

    // Create config object
    //
    rude::Config config;

    // load a configuration/.ini file
    //

    const char* t;
    t = "default.ini";
    config.load(t);
    config.setSection("Parameters");

    // create information

    //Number of consecutive frames in which tracking is considered valid
    config.setDoubleValue("NFramesHold",120);
    //Number of consecutive frames in which tracking is considered not valid
    config.setDoubleValue("NFramesLost",70);
    //Number of consecutive frames in which tracking is considered valid
    //and robot is ready for compensation
    config.setDoubleValue("NFramesComp",20);
    //Platform dimension
    config.setDoubleValue("platformLenght",1);
    //Max altitude for landing procedure
    config.setDoubleValue("zMax",7);
    //Minimum altitude for landing procedure (before compensating, this value should be above the maximum platform altitude)
    config.setDoubleValue("zMin",4);
    //Proportional gain times platform velocity
    config.setDoubleValue("KpHoldV",0.9);
    //Proportional gain times horizontal error
    config.setDoubleValue("KpHold",1.35);
    //Differential gain times horizontal error
    config.setDoubleValue("KdHold",0.00);
    //Integral gain times integral horizontal error
    config.setDoubleValue("KiHold",0.02);
    //Proportional gain for velocity tracking
    config.setDoubleValue("KPCompV",0.1);
    //Integral clamping values
    config.setDoubleValue("maxIntValue",5);
    //Max total PID output
    config.setDoubleValue("maxOutput",10);
    // save changes
    //
    config.save();

    const char* t2;
    t2 = "raspi.ini";
    config.load(t2);
    config.setSection("Parameters");

    //Number of consecutive frames in which tracking is considered valid
    config.setDoubleValue("NFramesHold",120);
    //Number of consecutive frames in which tracking is considered not valid
    config.setDoubleValue("NFramesLost",70);
    //Number of consecutive frames in which tracking is considered valid
    //and robot is ready for compensation
    config.setDoubleValue("NFramesComp",20);
    //Platform dimension
    config.setDoubleValue("platformLenght",0.65);
    //Max altitude for landing procedure
    config.setDoubleValue("zMax",2.5);
    //Minimum altitude for landing procedure (before compensating, this value should be above the maximum platform altitude)
    config.setDoubleValue("zMin",2.5);
    //Proportional gain times platform velocity
    config.setDoubleValue("KpHoldV",1);
    //Proportional gain times horizontal error
    config.setDoubleValue("KpHold",0.78);
    //Differential gain times horizontal error
    config.setDoubleValue("KdHold",0.00);
    //Integral gain times integral horizontal error
    config.setDoubleValue("KiHold",0.001);
    //Proportional gain for velocity tracking
    config.setDoubleValue("KPCompV",0.1);
    //Integral clamping values
    config.setDoubleValue("maxIntValue",1);
    //Max total PID output
    config.setDoubleValue("maxOutput",1.5);


    // save changes
    //
    config.save();

    return 0;
}