#include "AvoidWall.h"
#include "Constants.h"
#include "Aria.h"

AvoidWall::AvoidWall(ArSick* sick): ArAction("AvoidWall", "Behaviour for avoiding walls.")
{
	mySick = sick;
}


AvoidWall::~AvoidWall()
{
}

ArActionDesired* AvoidWall::fire(ArActionDesired currentDesired)
{
	/* At moment: robot does not take proper reading from the laser.
	
	It's connected. It's there. Just not taking the proper reading from the laser
	using the mySick->currentReadingPolar() methiod.
	
	*/

	ArLog::log(ArLog::Verbose, AVOIDWALL_CALLED);
	myDesired.reset();

	bool isWall = false;
	double idealHeading = 0.0;

	mySick->lockDevice();

	// get distance in front from sick
	double distanceFront = mySick->currentReadingPolar(-1, 1);
	printf("distance: %f \n", distanceFront);

	if (distanceFront < WALL_AVOID_DISTANCE) // object is within distance
	{
		double distanceLeft = mySick->currentReadingPolar(-46, -44);
		double distanceRight = mySick->currentReadingPolar(44, 46);

		if (distanceLeft < distanceFront && distanceFront < distanceRight) // rotating right
		{
			isWall = true;
			idealHeading = -WALL_AVOID_ROTATION_STEP;
		}
		else if (distanceLeft > distanceFront && distanceFront > distanceRight) // rotating left
		{
			isWall = true;
			idealHeading = WALL_AVOID_ROTATION_STEP;
		};

		if (isWall)
		{
			ArLog::log(ArLog::Normal, AVOIDWALL_ACTIVATED);
			myDesired.setDeltaHeading(idealHeading);
		}
	}
	else
	{
		mySick->unlockDevice();
		return NULL;
	}

	mySick->unlockDevice();
	return &myDesired;
}

/*
void avoidWall(void)
{
// get distances via sonar first, then calculate some rudimentary distances from the wall.
double distanceSonar4 = robot.getSonarRange(4); // remember: sonar sensors at the front are 10* from the centre.
double distanceSonar3 = robot.getSonarRange(3);
double robotDistanceFromWall4 = (distanceSonar4 * ArMath::sin(90)) / ArMath::sin(80);
double robotDistanceFromWall3 = (distanceSonar3 * ArMath::sin(90)) / ArMath::sin(80);

double minWallDist = 0.0;
double idealHeading = 0.0;

bool isWall = false;

// how do we detect a wall? a wall is a continuous barrier between two points.
if(robotDistanceFromWall4 < WALL_AVOID_DISTANCE
&& robotDistanceFromWall3 < WALL_AVOID_DISTANCE) // if the robot's two sensors are in range
{
// taking readings from SICK
double laserLeftDistance = sick.currentReadingPolar(-46, -44);
double laserRightDistance = sick.currentReadingPolar(44, 46);
double laserCentreDistance = sick.currentReadingPolar(-10, 10);

// taken three points, they form a wall if (and only if):
// left < centre < right;
// left > centre > right;
// left == centre == right;

if(laserLeftDistance < laserCentreDistance < laserRightDistance)
{
// in this, the rightmost distance is higher than the others, so the wall must slant to the left,
// so we must turn right.
isWall = true;
idealHeading = -90;

} else if (laserLeftDistance > laserCentreDistance > laserRightDistance)
{
// in this, the right distance is higher than the others, so the wall must slant to the right,
// so we must turn left.
isWall = true;
idealHeading = 90;
} else if (std::abs(laserLeftDistance - laserCentreDistance) <= 50 && std::abs(laserRightDistance - laserCentreDistance) <= 50){
// if all three are equal, the wall is perpendicular to the robot, so we must find other walls,
// lest we get boxed in.
isWall = true;
double rightHandDist = sick.currentReadingPolar(89, 90);
double leftHandDist = sick.currentReadingPolar(-90, -89);

if(rightHandDist > leftHandDist)
{
// turn right, because there's a wall to the left.
idealHeading = -90;
} else if(leftHandDist > rightHandDist)
{
// turn left, because there's a wall to the right.
idealHeading = 90;
};

}
else
{
isWall = false; // no wall, so nothing happens.
};
};

// wall avoidance logic
if (isWall)
{
printf("Avoiding wall. \t Robot Pose: X = %f, Y = %f, theta = %f, wall min at %f \n",
robot.getX(),
robot.getY(),
robot.getTh(),
minWallDist
);

// issuing movement commands to robot.
robot.lock();
robot.setDeltaHeading(idealHeading);
robot.unlock();
};
};

*/