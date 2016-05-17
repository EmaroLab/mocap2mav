#include "Security.h"
#include <iostream>


//TODO set square points
Security::Security()
{

    _d = 0.1;
    _x1Max = 0.7;
    _y1Max = 0.7;
    _x1Min = -0.7;
    _y1Min = -0.7;
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
    _setPoint.setX((float)position[0]);
    _setPoint.setY((float)position[1]);
    
    
    

}

void Security::CalculateIntersection(double position[])
{
    if(_state.getX()-_x1Max > 0)
       {
        position[0] = _x2Max;
        position[1] = _state.getY();
	
       }
    if(_state.getX()-_x1Min < 0)
       {
        position[0] = _x2Min;
        position[1] = _state.getY();
        
       }
    if(_state.getY()-_y1Max > 0)
       {
        position[0] = _state.getX();
        position[1] = _y2Max;
       
       }
    if(_state.getY()-_y1Min < 0)
       {
        position[0] = _state.getX();
        position[1] = _y2Min;
              }

}

bool Security::IsOutside()
{
    if(_state.getX()-_x1Max > 0)
       {
        return true;
       }
    if(_state.getX()-_x1Min < 0)
       {
       return true;
        
       }
    if(_state.getY()-_y1Max > 0)
       {
        return true;
       
       }
    if(_state.getY()-_y1Min < 0)
       {
        return true;
              }
     return false;
}
