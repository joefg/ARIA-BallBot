#include "PushToGoal.h"
#include "Constants.h"

// constructor
PushToGoal::PushToGoal(ArGripper* gripper): ArAction("PushToGoal", "Sends the ball to the goal if it detects it in its mitts.")
{
	myState = NOT_HOLDING_BALL;
	myGripper = gripper;
}

// destructor
PushToGoal::~PushToGoal()
{
}

// action
ArActionDesired* PushToGoal::fire(ArActionDesired currentDesired)
{
	ArLog::log(ArLog::Verbose, PUSH2GOAL_CALLED);

	myDesired.reset();

	if(myGripper->getGripState() == 2) // is there something in the gripper?
	{
		myGripper->gripClose(); // close gripper
		myState = HOLDING_BALL; // change state
		ArLog::log(ArLog::Verbose, PUSH2GOAL_ACTIVATED);
		myDesired.setHeading(0); // head north to the goal
		return &myDesired;
	} else
	{
		myState = NOT_HOLDING_BALL;
		return &myDesired;
	}
}

// setRobot-- necessary for removing old behaviours, moving robot, etc.
void PushToGoal::setRobot(ArRobot* robot)
{
	ArAction::setRobot(robot);
}
