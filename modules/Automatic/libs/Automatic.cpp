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

double calculateDescendRate(double dz,double drate_max,double drate_min, double tmax, double tmin){

    //Calculate the descend rate profile (linear) through y = mx + q
    //where x is vertical distance between robot and platform
    if(dz > tmax)      return drate_max;
    else if(dz < tmin) return drate_min;
    else{

        double m = (drate_max - drate_min) / (tmax - tmin);
        double q = drate_min - m * tmin;
        return m * dz + q;

    }
}
void Automatic::handleCommands() {

    std::cout << "Command: " << printAction(_actualTask.action) <<std::endl;
    switch (_actualTask.action){



        case actions::IDLE:

            break;

        case actions::MOVE:

            _actualCommand = std::unique_ptr<Command>(new Move(&_state,&_comm,&_actualTask));
            break;

        case actions::TAKE_OFF:
          //  std::unique_ptr<Command> temp(new TakeOff(&_state,&_comm,&_actualTask));
            _actualCommand = std::unique_ptr<Command>(new TakeOff(&_state,&_comm,&_actualTask));
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
