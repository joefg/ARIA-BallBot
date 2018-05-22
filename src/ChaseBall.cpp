#include "ChaseBall.h"
#include "Constants.h"

/*
	Adapted from Chase example-- the only real changes are to do with opening 
	and closing the gripper.

*/

ChaseBall::ChaseBall(ArACTS_1_2* acts, ArVCC4* camera, ArGripper* gripper) :
	ArAction("Chase", "Chases the orange ball.")
{
	myGripper = gripper;
	myActs = acts;
	myCamera = camera;
	myChannel = 0;
	myState = NO_TARGET;
	setChannel(1);
	myLastSeen.setToNow();
	myMaxTime = 1000;

	gotBall = false;
}

// Destructor
ChaseBall::~ChaseBall(void)
{
}

// The chase action
ArActionDesired* ChaseBall::fire(ArActionDesired currentDesired)
{
	ArACTSBlob blob;
	ArACTSBlob largestBlob;
	bool flag = false;
	int numberOfBlobs;
	int blobArea = 10;
	double xRel, yRel;
	// Reset the desired action
	myDesired.reset();

	numberOfBlobs = myActs->getNumBlobs(myChannel);
	// If there are blobs to be seen, set the time to now
	if (numberOfBlobs != 0)
	{
		for (int i = 0; i < numberOfBlobs; i++)
		{
			myActs->getBlob(myChannel, i + 1, &blob);
			if (blob.getArea() > blobArea)
			{
				flag = true;
				blobArea = blob.getArea();
				largestBlob = blob;
			}
		}
		myLastSeen.setToNow();
	}
	// If we have not seen a blob in a while...
	if (myLastSeen.mSecSince() > myMaxTime)
	{
		if (myState != NO_TARGET) ArLog::log(ArLog::Normal, "Target Lost");
		myState = NO_TARGET;
	}
	else
	{
		// If we see a blob and haven't seen one before..
		if (myState != TARGET)
		{
			ArLog::log(ArLog::Normal, "Target Aquired");
			ArLog::log(ArLog::Normal, "(Using channel %d with %d blobs)", myChannel, numberOfBlobs);
		}
		myState = TARGET;
	}
	if (myState == TARGET && flag == true)
	{
		// Determine where the largest blob's center of gravity
		// is relative to the center of the camera
		xRel = (double)(largestBlob.getXCG() - WIDTH / 2.0) / (double)WIDTH;
		yRel = (double)(largestBlob.getYCG() - HEIGHT / 2.0) / (double)HEIGHT;

		// Tilt the camera toward the blob
		if (!(ArMath::fabs(yRel) < .20))
		{
			if (-yRel > 0)
				myCamera->tiltRel(1);
			else
				myCamera->tiltRel(-1);
		}

		// Open gripper
		myGripper->gripOpen();

		// Set the heading and velocity for the robot
		if (ArMath::fabs(xRel) < .10)
		{
			myDesired.setDeltaHeading(0);
		}
		else
		{
			if (ArMath::fabs(-xRel * 10) <= 10)
				myDesired.setDeltaHeading(-xRel * 10);
			else if (-xRel > 0)
				myDesired.setDeltaHeading(10);
			else
				myDesired.setDeltaHeading(-10);
		}
		myDesired.setVel(ADVANCE_VELOCITY);
		return &myDesired;
	}

	// If we have no target, then don't set any action and let lower priority
	// actions (e.g. stop) control the robot.
	return &myDesired;
}

// Set the channel that the blob info will be obtained from
bool ChaseBall::setChannel(int channel)
{
	if (channel >= 1 && channel <= ArACTS_1_2::NUM_CHANNELS)
	{
		myChannel = channel;
		return true;
	}
	else
		return false;
}
