#pragma once
#include "ArAction.h"

#include "Aria.h"

// Chase is an action that moves the robot toward the largest blob that
// appears in it's current field of view.
class ChaseBall : public ArAction
{
public:

	// The state of the chase action
	enum State
	{
		NO_TARGET, // There is no target in view
		TARGET, // This is a target in view
	};

	// Constructor
	ChaseBall(ArACTS_1_2* acts, ArVCC4* camera, ArGripper* gripper);

	// Destructor
	~ChaseBall(void);

	// The action
	ArActionDesired* fire(ArActionDesired currentDesired);
	// Set the ACTS channel that we want to get blob info from
	bool setChannel(int channel);

	// Return the current state of this action
	State getState(void)
	{
		return myState;
	}

	// Height and width of pixels from frame-grabber
	enum
	{
		WIDTH = 160,
		HEIGHT = 120
	};

protected:
	ArActionDesired myDesired;
	ArACTS_1_2* myActs;
	ArVCC4* myCamera;
	ArGripper* myGripper;
	ArTime myLastSeen;
	State myState;
	int myChannel;
	int myMaxTime;

	bool gotBall;
};
