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
    t = "myfile.ini";

    config.load(t);


    // read information
    //
    config.setSection("General Info");
    double cost = config.getDoubleValue("Cost");
    const char *company = config.getStringValue("Company Name");


    // create information
    //
    config.setSection("new section");
    config.setStringValue("animal type", "giraffe");
    config.setBoolValue("mammal", true);

    // save changes
    //
    config.save();

    return 0;
}