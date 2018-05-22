#pragma once
#include "ArAction.h"
#include "Aria.h"

class PushToGoal :
	public ArAction
{
public:

	enum State
	{
		NOT_HOLDING_BALL,
		HOLDING_BALL
	};

	// constructors and destructors
	PushToGoal(ArGripper* myGripper);
	~PushToGoal();

	ArActionDesired *fire(ArActionDesired currentDesired); // our action

	void setRobot(ArRobot* robot) override;

protected:
	ArActionDesired myDesired;
	ArGripper* myGripper;
	State myState;
};

