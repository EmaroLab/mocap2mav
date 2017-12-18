//
// Created by andreanistico on 17/02/17.
//

#include "mocap/include/Automatic.h"
#include "mocap/include/Executioner.h"

int main(int argc, char** argv){

    Automatic a;
    Executioner e;
    e.init();
    a.handleCommands();
    return 0;

}

