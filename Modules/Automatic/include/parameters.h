//
// Created by andreanistico on 21/02/17.
//

#ifndef MOCAP2MAV_PARAMETERS_H
#define MOCAP2MAV_PARAMETERS_H

namespace params_automatic {

    static const int    NFramesHold     = 300;
    static const int    NFramesLost     = 100;
    static const double platformLenght  = 2;
    static const double zMax            = 10;
    static double       zMin            = 3;

    static const double KpHold          = 1;
    static const double KiHold          = 0.037;

}

#endif //MOCAP2MAV_PARAMETERS_H
