//
// Created by andreanistico on 12/06/17.
//

#include <iostream>
#include "lcm/lcm-cpp.hpp"
#include "poll.h"
#include "utils/TimeHelpers.hpp"
#include "lcm_messages/geometry/pose.hpp"

int main(int argc, char** argv){

    lcm::LCM handler;
//vision_position_estimate

    Spinner spinner(120);
    geometry::pose p;

    p.position[0] = 1;
    p.position[1] = 2;
    p.position[2] = 3;

    p.orientation[0] = 1;
    p.orientation[1] = 0;
    p.orientation[2] = 0;
    p.orientation[3] = 0;

    while(spinner.ok()) {
        handler.publish("vision_position_estimate",&p);

    }

    return 0;

}