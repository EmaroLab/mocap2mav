#ifndef UTILS_H
#define UTILS_H

#include<vector>
#include "MavState.h"
#define PI 3.141592653589


struct position{

    double x = 0;
    double y = 0;
    double z = 0;
    double yaw = 0;


    position operator-(position p){

        position temp;
        temp.x = x - p.x;
        temp.y = y - p.y;
        temp.z = z - p.z;
        return temp;

    }

    position operator+(position p){

        position temp;
        temp.x = x + p.x;
        temp.y = y + p.y;
        temp.z = z + p.z;
        return temp;

    }

    void operator=(position p){


        this->x = p.x;
        this->y = p.y;
        this->z = p.z;


    }

    position operator*(float f){

        position temp;
        temp.x = f*x;
        temp.y = f*y;
        temp.z = f*z;

        return temp;

    }



};



struct action{
    char type;
    double params[4] = {0,0,0,0};
};

struct node {

    int id;
    position p;
    action a;

};

void calculateYawIntem(double yawSP,double robotHeading,double &yawComm);
void calculatePositionInterm(double alpha, position target, position robot_state , MavState &comm);

#endif // UTILS_H
