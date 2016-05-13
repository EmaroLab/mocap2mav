#include "Security.h"


//TODO set square points
Security::Security()
{

    _x1Max = 0;
    _y1Max = 0;
    _x1Min = 0;
    _y1Min = 0;
    _x2Max = 0;
    _y2Max = 0;
    _x2Min = 0;
    _y2Min = 0;
}

void Security::setState(MavState rState)
{
    rState = _state;
}

MavState Security::getState()
{
    return _state;
}

void Security::setSetPoint(MavState rSP)
{
    rSP = _setPoint;
}

MavState Security::getSetPoint()
{
    return _setPoint;
}

void Security::CalculateNewSP()
{
	double position[2];
        CalculateIntersection(_state,_setPoint, position);
        _comm.setX((float)position[0]);
        _comm.setY((float)position[1]);

}

void Security::CalculateIntersection(const MavState rState, MavState rSP, double position[])
{
    if(rState.getX()-_x1Max < 0 && rSP.getX()-_x1Max > 0)
       {
        position[0] = _x2Max;
        position[1] = rSP.getY();
       }
    if(rState.getX()-_x1Min > 0 && rSP.getX()-_x1Min < 0)
       {
        position[0] = _x2Min;
        position[1] = rSP.getY();
       }
    if(rState.getY()-_y1Max < 0 && rSP.getX()-_y1Max > 0)
       {
        position[0] = rSP.getX();
        position[1] = _y2Max;
       }
    if(rState.getY()-_y1Min > 0 && rSP.getX()-_y1Min < 0)
       {
        position[0] = rSP.getX();
        position[1] = _y2Min;
       }
}
