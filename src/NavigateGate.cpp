#include "NavigateGate.h"
#include "Constants.h"

// Constructor
NavigateGate::NavigateGate(ArSick* sick): ArAction("NavigateGate", "Goes through the gates.")
{
	myState = NO_GATE;
	mySick = sick;
}

// Destructor
NavigateGate::~NavigateGate()
{
}

// Navigate Action
ArActionDesired* NavigateGate::fire(ArActionDesired currentDesired)
{
	// find the two posts
	// post criterion: two objects, 1400mm apart.

	// to get (x, y) of posts:
	// x_p = RANGE * cos(th) + SICK X
	// y_p = RANGE * sin(th) + SICK Y

	// xR = (x_p * cos(robth)) - ((y_p * sin(robth))
	// yR = (x_p * sin(robth)) - ((y_p * cos(robth))

	// x_real = xR + robotX
	// y_real = yR + robotY


	double distanceGate1, thetaGate1;
	distanceGate1 = mySick->currentReadingPolar(10, 60, &thetaGate1);

	double distanceGate2, thetaGate2;
	distanceGate2 = mySick->currentReadingPolar(-60, -10, &thetaGate2);

	// simple test of whether the posts are gates:
	// {\displaystyle c^{2}=a^{2}+b^{2}-2ab\cos \gamma ,}

	ArLog::log(ArLog::Normal, GATENAV_CALLED);

	// are the detected objects close enough to care about?
	if ((GATE_TOO_CLOSE_DISTANCE < distanceGate1 && distanceGate1 < GATE_NOTICE_DISTANCE) &&
		(GATE_TOO_CLOSE_DISTANCE < distanceGate2 && distanceGate2 < GATE_NOTICE_DISTANCE))
	{
		printf("g1: %d < %f < %d \n",
		       GATE_TOO_CLOSE_DISTANCE,
		       distanceGate1,
		       GATE_NOTICE_DISTANCE
		);

		printf("g2: %d < %f < %d \n",
		       GATE_TOO_CLOSE_DISTANCE,
		       distanceGate2,
		       GATE_NOTICE_DISTANCE
		);

		ArLog::log(ArLog::Normal, GATENAV_ACTIVATED);

		// incrementally setting delta headings
		if (distanceGate1 < distanceGate2)
		{
			myDesired.reset();
			// turning right
			ArLog::log(ArLog::Normal, GATENAV_TURN_RIGHT);
			myDesired.setDeltaHeading(-GATE_ROTATION_STEP);
			myState = GATE_NAVIGATING;
		}
		else if (distanceGate2 < distanceGate1)
		{
			myDesired.reset();
			// turning left
			ArLog::log(ArLog::Normal, GATENAV_TURN_LEFT);
			myDesired.setDeltaHeading(GATE_ROTATION_STEP);
			myState = GATE_NAVIGATING;
		}
	}
	else
	{
		myState = NO_GATE;
		return NULL;
	}

	printf("desired delta heading: %f \n", myDesired.getDeltaHeading());
	myDesired.setRotVel(ROTATIONAL_VELOCITY_GATES);
	//myDesired.setVel(ADVANCE_VELOCTIY);

	return &myDesired;
}

void NavigateGate::setRobot(ArRobot* robot)
{
	ArAction::setRobot(robot);
}

/* ref

void navigateGate(void)
{
// detecting the gates with SICK
double distanceGate1, thetaGate1;
distanceGate1 = sick.currentReadingPolar(10, 90, &thetaGate1);

double distanceGate2, thetaGate2;
distanceGate2 = sick.currentReadingPolar(-90, -10, &thetaGate2);

// are the detected objects close enough to care about?
if (!(distanceGate1 > GATE_NOTICE_DISTANCE || distanceGate2 > GATE_NOTICE_DISTANCE
|| distanceGate1 < GATE_TOO_CLOSE_DISTANCE || distanceGate2 < GATE_TOO_CLOSE_DISTANCE)) {
printf("NavigateGate : d1=%f th=%f, d2=%f th=%f \n",
distanceGate1,
thetaGate1,
distanceGate2,
thetaGate2
);

// incrementally setting delta headings
if (distanceGate1 < distanceGate2) {

printf("Turning right. \n");

if (robot.isHeadingDone()) {
robot.lock();
robot.setDeltaHeading(-ROTATION_STEP);
robot.unlock();
};

} else if (distanceGate2 < distanceGate1) {

printf("Turning left. \n");

if (robot.isHeadingDone()) {
robot.lock();
robot.setDeltaHeading(ROTATION_STEP);
robot.unlock();
};
};
};
};

*/