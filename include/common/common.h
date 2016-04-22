//
// Created by andrea on 22/04/16.
//

#ifndef MOCAP2MAV_COMMON_H
#define MOCAP2MAV_COMMON_H

#include <chrono>

#define  MILLI2SECS 0.001

inline uint64_t getTimeNanoSecond()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * (uint64_t)1000000000 + ts.tv_nsec;
}

inline uint64_t getTimeMilliSecond(){

return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

}

#endif //MOCAP2MAV_COMMON_H
