#include <include/Command/Idle.hpp>
#include "Automatic.h"
using namespace common;
Automatic::Automatic()  {}

MavState Automatic::getState()
{
    return _state;
}
exec::task Automatic::getTask()
{
    return _actualTask;
}

void Automatic::setState(MavState rState)
{
    _state = rState;
}

void Automatic::setTask(exec::task rTask)
{
    _actualTask.x=rTask.x;
    _actualTask.y=rTask.y;
    _actualTask.z=rTask.z;
    _actualTask.yaw=rTask.yaw;
    _actualTask.action=rTask.action;
    _actualTask.params[0]=rTask.params[0];
    _actualTask.params[1]=rTask.params[1];
    _actualTask.params[2]=rTask.params[2];
    _actualTask.params[3]=rTask.params[3];
}

void Automatic::handleCommands() {

    std::cout << "Command: " << printAction(_actualTask.action) <<std::endl;
    switch (_actualTask.action){



        case actions::IDLE:
            _actualCommand = std::unique_ptr<Command>(new Idle(&_state,&_comm, nullptr));
            break;

        case actions::MOVE:

            _actualCommand = std::unique_ptr<Command>(new Move(&_state,&_comm,&_actualTask));
            break;

        case actions::TAKE_OFF:
            _actualCommand.reset(new TakeOff(&_state,&_comm,&_actualTask));
           // _actualCommand = std::unique_ptr<Command>(new TakeOff(&_state,&_comm,&_actualTask));
            std::cout << "Actual command: TakeOff" << std::endl;
            break;

        case actions::LAND:

            _actualCommand = std::unique_ptr<Command>(new Land(&_state,&_comm,&_actualTask,&_platformState));
            std::cout << "Actual command: TakeOff" << std::endl;
            break;
        case actions::ROTATE:

            break;

        default:

            std::cout << "Unknown action" << std::endl;
            return;

    }

}
void Automatic::executeCommand() {
    this->_actualCommand->execute();
}

void Automatic::setPlatformState(MavState pose) {
    _platformState = pose;
}
