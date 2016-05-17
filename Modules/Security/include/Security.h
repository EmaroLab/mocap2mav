// class for flight window
#ifndef SECURITY_H
#define SECURITY_H
#include "lcm_messages/geometry/pose.hpp"
#include "lcm_messages/exec/task.hpp"
#include "MavState.h"

class Security
{
public:
	Security();
	// sides of the window
	double _x1Max;
	double _y1Max;
	double _x1Min;
	double _y1Min;
	double _x2Max;
	double _y2Max;
	double _x2Min;
	double _y2Min;
	double _d;
	void CalculateNewSP();
	void CalculateIntersection(double position[]);
	MavState getState();
	void setState(MavState pose);
	MavState getSetPoint();
	void setSetPoint(MavState SP);
	bool IsOutside();
	
private:
	MavState _state;
	MavState _setPoint;
};	

#endif // SECURITY_H
