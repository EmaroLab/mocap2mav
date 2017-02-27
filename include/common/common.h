//
// Created by andrea on 22/04/16.
//

#ifndef MOCAP2MAV_COMMON_H
#define MOCAP2MAV_COMMON_H

#include <chrono>
#include <map>
#include <string>


#define  MILLI2SECS 0.001
namespace common {

    namespace time {
        inline uint64_t getTimeNanoSecond() {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            return ts.tv_sec * (uint64_t) 1000000000 + ts.tv_nsec;
        }

        inline uint64_t getTimeMilliSecond() {

            return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

        }
    }//time


    enum actions {

        TAKE_OFF,
        MOVE,
        ROTATE,
        LAND,
        IDLE //helper

    };

    static std::string printAction(int a){

       static std::map<int , std::string> actionMap;

       if (actionMap.empty()){

           actionMap[actions::LAND]     = "Land";
           actionMap[actions::MOVE]     = "Move";
           actionMap[actions::TAKE_OFF] = "Take off";
           actionMap[actions::ROTATE]   = "Rotate";
           actionMap[actions::IDLE]     = "Idle";

       }

       return actionMap[a];


    }






} //common
#endif //MOCAP2MAV_COMMON_H
