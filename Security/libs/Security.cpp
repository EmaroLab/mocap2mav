#include "Security.h"


//TODO set square points
Security::Security()
{

    _d = 0.1;
    _x1Max = 0.8;
    _y1Max = 0.8;
    _x1Min = -0.8;
    _y1Min = -0.8;
    _x2Max = _x1Max - _d;
    _y2Max = _y1Max - _d;
    _x2Min = _x2Min + _d;
    _y2Min = _y2Min + _d;
}

void Security::setState(MavState rState)
{
    _state = rState;
}

MavState Security::getState()
{
    return _state;
}

void Security::setSetPoint(MavState rSP)
{
    _setPoint = rSP;
}

MavState Security::getSetPoint()
{
    return _setPoint;
}

void Security::CalculateNewSP()
{
    double position[2];
    CalculateIntersection(position);
    _comm.setX((float)position[0]);
    _comm.setY((float)position[1]);

}

void Security::CalculateIntersection(double position[])
{
    if(_state.getX()-_x1Max < 0 && _setPoint.getX()-_x1Max > 0)
       {
        position[0] = _x2Max;
        position[1] = _setPoint.getY();
       }
    if(_state.getX()-_x1Min > 0 && _setPoint.getX()-_x1Min < 0)
       {
        position[0] = _x2Min;
        position[1] = _setPoint.getY();
       }
    if(_state.getY()-_y1Max < 0 && _setPoint.getX()-_y1Max > 0)
       {
        position[0] = _setPoint.getX();
        position[1] = _y2Max;
       }
    if(_state.getY()-_y1Min > 0 && _setPoint.getX()-_y1Min < 0)
       {
        position[0] = _setPoint.getX();
        position[1] = _y2Min;
       }
}
