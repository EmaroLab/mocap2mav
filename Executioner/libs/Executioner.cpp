#include "Executioner.h"
#include <iostream>


#define PI 3.141592653589



bool endList = false;




Executioner::Executioner(){
    _actualNode = 0;
    _can_run = false;
    _newTask = true;
    _publish_task = false;

    // Fill Node list
    exec::task node1;
    node1.action = actions::TAKE_OFF;
    node1.params[0] = -1; //height
    _nodeList.push_back(node1);

    exec::task  move;
    move.action = actions::MOVE;
    move.x = 1.0;
    move.y = 0.0;
    move.z = -1;
    move.params[0] = 1;
    move.params[1] = 3;
    _nodeList.push_back(move);

    exec::task  move2;
    move2.action = actions::MOVE;
    move2.x = 0.0;
    move2.y = 0.0;
    move2.z = -1;
    move2.params[0] = 1;
    move2.params[1] = 3;
    _nodeList.push_back(move2);

    exec::task rotate;
    rotate.action= actions::ROTATE;
    rotate.params[0] = 1;
    rotate.yaw = PI/2;
    _nodeList.push_back(rotate);

    exec::task land;
    land.action= actions::LAND;
    _nodeList.push_back(land);

    if(_nodeList.size()>0){

        _can_run = true;
    }
    else{

        std::cout << "WARNING, empty list"<<std::endl;
        _can_run = false;
    }
}

void Executioner::run(){


if(!_nodeList.empty()) {

    _actualTask.x = _nodeList[_actualNode].x;
    _actualTask.y = _nodeList[_actualNode].y;
    _actualTask.z = _nodeList[_actualNode].z;
    _actualTask.yaw = _nodeList[_actualNode].yaw;
    _actualTask.action = _nodeList[_actualNode].action;
    _actualTask.id = _nodeList[_actualNode].id;
    _actualTask.params[0] = _nodeList[_actualNode].params[0];
    _actualTask.params[1] = _nodeList[_actualNode].params[1];
    _actualTask.params[2] = _nodeList[_actualNode].params[2];
    _actualTask.params[3] = _nodeList[_actualNode].params[3];

    _can_run = _actualNode < _nodeList.size();
}
    else{
    _can_run = false;
}
    if(_newTask) {

        std::cout << "Performing node: " << _actualNode << " with action: " << _actualTask.action<<std::endl;
        _publish_task = true;

    }

    // Check for next task
    if(CheckActions(_actualTask.action)) {

        if(!_nodeList.empty()){
            _nodeList.pop_front();
            _nodeList.shrink_to_fit();
            _newTask = true;
        }
        else{
            std::cout<<"Empty list, send last setpoint"<<std::endl; //TODO: implement idle function
            endList = true;
            _newTask = false;
        }
    }
    else{
        _newTask = false;
    }

}

bool Executioner::CheckActions(int a)
{
    int c = a;
    switch (c)
    {
        //MOVE
        case actions::MOVE:

            return (fabs(_state.getX() - _nodeList[_actualNode].x) < 0.15 &&
                    fabs(_state.getY() - _nodeList[_actualNode].y) < 0.15 &&
                    fabs(_state.getZ() - _nodeList[_actualNode].z) < 0.15 );

            //TAKE_OFF
        case actions::TAKE_OFF:

            return (fabs(_state.getZ() - _nodeList[_actualNode].params[0]) < 0.1 );

            //ROTATE
        case actions::ROTATE:

            return (fabs(fabs(_actualTask.yaw) - fabs(_state.getYawFromQuat())) < PI/10);


            //LAND
        case actions::LAND:
            if(fabs(_state.getVz()) < 0.01 && (_state.getZ() - _actualTask.params[1]) >= - 0.10)
                return true;
            else
                return false;

        default:
            std::cout << "Unrecognized type" << std::endl;
            return true;
    }

}

void Executioner::setNextTask(const exec::task task){

    _nodeList.shrink_to_fit();
    _nodeList.push_front(task);

}
void Executioner::setLastTask(const exec::task task){

    _nodeList.shrink_to_fit();
    _nodeList.push_back(task);

}
void Executioner::clearList(){

    _nodeList.clear();
    _nodeList.shrink_to_fit();

}