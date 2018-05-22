#pragma once
#include "ArAction.h"
#include "Aria.h"

class AvoidWall :
	public ArAction
{
public:

	enum State
	{
		NOT_WALL,
		WALL,
	};

	AvoidWall(ArSick* sick); // constructor
	~AvoidWall(); // destructor

	ArActionDesired* fire(ArActionDesired currentDesired); // action

protected:
	ArActionDesired myDesired;
	ArSick* mySick;
};
