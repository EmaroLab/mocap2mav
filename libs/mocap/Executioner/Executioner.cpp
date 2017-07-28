#include <iostream>

#include "include/Executioner.h"

#define PI 3.141592653589
#define GUI

int node = 0;
bool skip = false;

using namespace common;

Executioner::Executioner(){

    _actualNode = 0;
    _idle = false;
    _can_run = false;
    _newTask = true;
    _publish_task = false;
    _nodeList.clear();
    _nodeList.shrink_to_fit();

}

void Executioner::init(){

    if(_nodeList.size()>0){

        _can_run = true;
        _newTask = true;
        _publish_task = false;
        _idle = false;
    }
    else{

        _nodeList.clear();
        _nodeList.shrink_to_fit();
        std::cout << "WARNING, empty list"<<std::endl;
        _can_run = false;
        _idle = true;
    }

}

void Executioner::setState(MavState s){

    _state = s;

}

void::Executioner::setList(std::deque<exec::task> list) {

    _nodeList = list;

}

bool Executioner::readyToPublish() {

    if(_publish_task) {
        _publish_task = false;
        return true;
    }
    else return false;

}

void Executioner::run(){

    if(_nodeList.size() > 0) {

        loadTask();
        _can_run = true;
        _idle = false;
    }
    else{
        _can_run = false;

        if (!_idle){
            //Publishing last task, next will be idle
            _publish_task = true;
            _idle = true;
            _actualTask.action = actions ::IDLE;


        }else _publish_task = false;


    }

    //Detect a new task arrived
    if(_newTask) {

        bool done = false;
        char in;
        std::cout<<std::endl;
        std::cout << "***************************************"<<std::endl;
        std::cout << "Performing node: " << node++ << " with action: " << common::printAction(_actualTask.action)<<std::endl;
#ifdef GUI
        std::cout << "Do you want to proceed? Y/N"<<std::endl;
#endif
        std::cout << "***************************************"<<std::endl;
        std::cout<<std::endl;


        do {
        #ifdef GUI
            std::cin  >> in;
        #else
            in = 'y';
        #endif

            switch (in) {

                case 'y':

                    _publish_task = true;
                    done = true;
                    skip = false;
                    break;

                case 'n':

                    _publish_task = false;
                    skip = true;
                    std::cout << "Skipping task, moving to the next" << std::endl;
                    done = true;
                    break;

                default :

                    std::cout << "Wrong command, type y or n" << std::endl;
                    done = false;

            }
        }while(!done);

    }

    // Check for next task
    if((CheckActions(_actualTask.action) || skip) && _can_run) {

        if(_nodeList.size() > 0){

            _nodeList.pop_front();
            _nodeList.shrink_to_fit();
            std::cout << "popping, size: "<< _nodeList.size() << std::endl;

            if (_nodeList.empty()) _newTask = false;
            else                   _newTask = true;

        }
        else{
            std::cout<<"Empty list, sending last setpoint"<<std::endl;

            _newTask = false;
        }
    }
    else{
        _newTask = false;
    }

    //std::cout << _nodeList.size() << std::endl;
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

            return (fabs(_state.getZ()) >= (fabs(_actualTask.params[0]) - 0.15));

            //ROTATE
        case actions::ROTATE:

            return (fabs(fabs(_actualTask.yaw) - fabs(_state.getYawFromQuat())) < PI/10);


            //LAND
        case actions::LAND:

            if(_actualTask.params[0] == 1){

                return false;//TODO: implement stopping condition

            }
            else{

                return (_landed);

            }


        default:
            std::cout << "IDLE" << std::endl;
            return true;
    }

}

void Executioner::setLastTask(const exec::task task){

    _nodeList.shrink_to_fit();
    _nodeList.push_back(task);

}

void Executioner::setNextTask(const exec::task task){

    _nodeList.shrink_to_fit();
    _nodeList.push_front(task);

}

void Executioner::clearList(){

    _nodeList.clear();
    _nodeList.shrink_to_fit();

}

void Executioner::abort() {



}

void Executioner::rtl() {

}

void Executioner::loadTask() {

    _actualTask.x         = _nodeList[_actualNode].x;
    _actualTask.y         = _nodeList[_actualNode].y;
    _actualTask.z         = _nodeList[_actualNode].z;
    _actualTask.yaw       = _nodeList[_actualNode].yaw;
    _actualTask.action    = _nodeList[_actualNode].action;
    _actualTask.id        = _nodeList[_actualNode].id;
    _actualTask.params[0] = _nodeList[_actualNode].params[0];
    _actualTask.params[1] = _nodeList[_actualNode].params[1];
    _actualTask.params[2] = _nodeList[_actualNode].params[2];
    _actualTask.params[3] = _nodeList[_actualNode].params[3];

}

void Executioner::setStatus(bool armed, bool landed) {

    _armed = armed;
    _landed = landed;
}