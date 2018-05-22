#pragma once
#include "ArAction.h"
#include "Aria.h"

class NavigateGate :
	public ArAction
{
public:
	// The state of the action
	enum State
	{
		NO_GATE, // no gate found
		GATE_DETECTED, // gate detected;
		GATE_NAVIGATING, // gate found and navigating;
		GATE_EXITING, // exiting gate
	};


	NavigateGate(ArSick* sick); // constructor
	~NavigateGate(); // destructor

	ArActionDesired* fire(ArActionDesired currentDesired); // action

	void setRobot(ArRobot* robot) override;

	// returns the state of the action
	State getState(void)
	{
		return myState;
	}

protected:
	ArActionDesired myDesired;
	ArSick* mySick;
	State myState;
};
