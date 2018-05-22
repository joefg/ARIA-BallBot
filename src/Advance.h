#pragma once
#include "ArAction.h"

class Advance :
	public ArAction
{
public:

	// the state of the advance action
	enum State
	{
		ADVANCING, // moving forward to the goal
		STATIONARY, // stopped
		RETREATING, // moving in reverse
	};

	Advance(); // constructor
	~Advance(); // destructor

	ArActionDesired* fire(ArActionDesired currentDesired); // the action

protected:
	ArActionDesired myDesired;
	State myState;
};
