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

    static double interpolate(double t,double y_max,double y_min, double tmax, double tmin){

        //Calculate the descend rate profile (linear) through y = mx + q
        //where x is vertical distance between robot and platform
        if(t > tmax)      return y_max;
        else if(t < tmin) return y_min;
        else{

            double m = (y_max - y_min) / (tmax - tmin);
            double q = y_min - m * tmin;
            return m * t + q;

        }
    }
    static double clamp(double value, double min,double max){

        if (value < min) return min;
        if (value > max) return max;

        return  value;

    }



} //common
#endif //MOCAP2MAV_COMMON_H
