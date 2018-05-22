#include "Advance.h"
#include "Constants.h"

// Constructor
Advance::Advance(): ArAction("Advance", "Advance behaviour for moving toward the goal. Base behaviour.")
{
	myState = STATIONARY;
};

// Destructor
Advance::~Advance()
{
};

// The action
ArActionDesired* Advance::fire(ArActionDesired currentDesired)
{
	ArLog::log(ArLog::Verbose, ADVANCE_CALLED);

	// set our robot to advance in a straight line

	myDesired.reset();

	if (!myDesired.isAnythingDesired())
	{
		myDesired.setVel(ADVANCE_VELOCITY);
		ArLog::log(ArLog::Normal, ADVANCE_ACTIVATED);
		myState = ADVANCING;
	}
	else
	{
		myState = STATIONARY;
	}

	return &myDesired;
};
