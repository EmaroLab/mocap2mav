#include "Executioner.h"
#include <vector>
#include<math.h>
#include <iostream>


#define PI 3.141592653589

namespace executioner{
namespace land{

bool landed;


}
namespace move{

bool move_done;


}
namespace take_off{

bool take_off_done;


}
namespace rotate{

bool rotate_done;


}
namespace trajectory{

bool traj_done;

}
}

std::vector<exec::task> nodeList;

bool endList = false;

Executioner::Executioner(){
    _actualNode = 0;
    _can_run = false;
    _newTask = true;
    _publish_task = false;

    // Fill Node list
    exec::task node1;
    node1.action = "t";
    node1.params[0] = -1; //height
    nodeList.push_back(node1);

    exec::task  move;
    move.action = "m";
    move.x = 1.0;
    move.y = 0.0;
    move.z = -1;
    move.params[0] = 1;
    move.params[1] = 3;
    nodeList.push_back(move);

    exec::task  move2;
    move2.action = "m";
    move2.x = 0.0;
    move2.y = 0.0;
    move2.z = -1;
    move2.params[0] = 1;
    move2.params[1] = 3;
    nodeList.push_back(move2);

    exec::task rotate;
    rotate.action= "r";
    rotate.params[0] = 1;
    rotate.yaw = PI/2;
    nodeList.push_back(rotate);

    exec::task land;
    land.action= "l";
    nodeList.push_back(land);

    if(nodeList.size()>0){

        _can_run = true;
    }
    else{

        std::cout << "WARNING, empty list"<<std::endl;
        _can_run = false;
    }
}

void Executioner::run(MavState state){

    _actualTask.x = nodeList[_actualNode].x;
    _actualTask.y = nodeList[_actualNode].y;
    _actualTask.z = nodeList[_actualNode].z;
    _actualTask.yaw = nodeList[_actualNode].yaw;
    _actualTask.action= nodeList[_actualNode].action;
    _actualTask.id = nodeList[_actualNode].id;
    _actualTask.params[0] = nodeList[_actualNode].params[0];
    _actualTask.params[1] = nodeList[_actualNode].params[1];
    _actualTask.params[2] = nodeList[_actualNode].params[2];
    _actualTask.params[3] = nodeList[_actualNode].params[3];

    _can_run =_actualNode < nodeList.size();

    if(_newTask) {

        std::cout << "Performing node: " << _actualNode << " with action: " << _actualTask.action<<std::endl;
        _publish_task = true;

    }

    // Check for next task
    if(CheckActions(_actualTask.action, state)) {
        if(_actualNode != nodeList.size()-1){
            _actualNode++;
            _newTask = true;
        }
        else if(!endList){
            std::cout<<"tasks finished"<<std::endl;
            endList = true;
            _newTask = false;
        }
    }
    else{
        _newTask = false;
    }

}

bool Executioner::CheckActions(std::string a,MavState state)
{
    char c = a[0];
    switch (c)
    {
    //MOVE
    case 'm':

        if(fabs(state.getX()- nodeList[_actualNode].x) < 0.15 &&
                fabs(state.getY() - nodeList[_actualNode].y) < 0.15 &&
                fabs(state.getZ() - nodeList[_actualNode].z) < 0.15 ){

            executioner::move::move_done = true;

        }
        else
            executioner::move::move_done = false;

        return executioner::move::move_done;
        break;

        //TAKE_OFF
    case 't':

        if(fabs(state.getZ() - nodeList[_actualNode].params[0]) < 0.1 ){
            executioner::take_off::take_off_done = true;

        }
        else{
            executioner::take_off::take_off_done = false;
        }

        return executioner::take_off::take_off_done;
        break;

        //ROTATE
    case 'r' :
        if( fabs(fabs(_actualTask.yaw) - fabs(state.getYawFromQuat())) < PI/10){
            executioner::rotate::rotate_done = true;
        }
        else{ executioner::rotate::rotate_done = false;
            // if(++rot_count == rot_wait * r_auto){


            //rot_count = 0;}

        }
        return executioner::rotate::rotate_done;
        break;

        //LAND
    case 'l' :
        if(fabs(state.getVz()) < 0.01 && (state.getZ() - _actualTask.params[1]) >= - 0.10)
            executioner::land::landed = true;
        else
            executioner::land::landed = false;
        return executioner::land::landed;
        break;
    }
}

