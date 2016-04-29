//
// Created by andrea on 29/04/16.
//

#ifndef MOCAP2MAV_CALLBACKHANDLER_H
#define MOCAP2MAV_CALLBACKHANDLER_H

#include "MavState.h"
#include "lcm/lcm-cpp.hpp"
#include "lcm_messages/geometry/pose.hpp"

class CallbackHandler {

public:

    CallbackHandler();

    MavState _vision_pos;
    MavState _position_sp;

    bool _estimate_ready;
    bool _position_sp_ready;

    void visionEstimateCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg);
    void positionSetpointCallback(const lcm::ReceiveBuffer* rbuf, const std::string& chan, const geometry::pose* msg);

    MavState lcmPose2MavState(const geometry::pose lcmPose);

    geometry::pose mavState2LcmPose(MavState mavPose);

};

#endif //MOCAP2MAV_CALLBACKHANDLER_H
