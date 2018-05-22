#include <stdio.h>
#include "Aria.h"
#include <list>
#include <iostream>

// constants header
#include "Constants.h"

// our classes' headers
#include "Advance.h"
#include "AvoidWall.h"
#include "ChaseBall.h"
#include "NavigateGate.h"
#include "PushToGoal.h"


/* Global Variables */
ArRobot robot; // robot object and associated sensors
ArSonarDevice sonar; // sonar device
ArSick *sick; // laser scanner
ArVCC4 *camera; // Canon camera attached to the robot
ArACTS_1_2 acts; // ACTS method for "computer vision" on the robot
ArGripper *gripper; // Gripper interface

FILE* trajectoryFile; // trajectory plot map
FILE* sonarMapFile; // sonar map
FILE* sickMapFile; // laser map


/* Method declarations and associated functors */
void recordTrajectory(void);
ArGlobalFunctor trajectoryCB(&recordTrajectory);

void sickMap(void);
ArGlobalFunctor sickMapCB(&sickMap);

void sonarMap(void);
ArGlobalFunctor sonarMapCB(&sonarMap);

// helper methods
void primeFiles(void);


/* File output methods-- these are for mapping / plotting purposes. */
void primeFiles(void)
{
	ArLog::log(ArLog::Normal, FILES_PRIMED_PRIMING);

	// Primes the Trajectory file.
	trajectoryFile = fopen(TRAJECTORY_FILE_NAME, "w");
	if (trajectoryFile == NULL)
	{
		ArLog::log(ArLog::Terse, ERROR_ACCESSING_TRAJECTORY_MAP_MESSAGE);
	}
	else
	{
		fprintf(trajectoryFile, TRAJECTORY_FILE_HEADER);
		fclose(trajectoryFile);
		ArLog::log(ArLog::Normal, TRAJECTORY_FILE_PRIMED);
	};

	// Primes the SICK Map file.
	sickMapFile = fopen(SICK_MAP_FILE_NAME, "w");
	if (sickMapFile == NULL)
	{
		ArLog::log(ArLog::Terse, ERROR_ACCESSING_SICK_MAP_MESSAGE);
	}
	else
	{
		fprintf(sickMapFile, SICK_MAP_FILE_HEADER);
		fclose(sickMapFile);
		ArLog::log(ArLog::Normal, SICK_FILE_PRIMED);
	};

	// Primes the Sonar Map file.
	sonarMapFile = fopen(SONAR_MAP_FILE_NAME, "w");
	if (sonarMapFile == NULL)
	{
		ArLog::log(ArLog::Terse, ERROR_ACCESSING_SONAR_MAP_MESSAGE);
		fclose(sonarMapFile);
	}
	else
	{
		fprintf(sonarMapFile, SONAR_MAP_FILE_HEADER);
		fclose(sonarMapFile);
		ArLog::log(ArLog::Normal, SONAR_FILE_PRIMED);
	}

	ArLog::log(ArLog::Normal, FILES_PRIMED_SUCCESS);
}

void recordTrajectory(void)
{
	// opens the trajectory file
	trajectoryFile = fopen(TRAJECTORY_FILE_NAME, "a");

	if (trajectoryFile == NULL)
	{
		ArLog::log(ArLog::Terse, ERROR_ACCESSING_TRAJECTORY_MAP_MESSAGE);
		return;
	};

	// writes current pose info
	fprintf(trajectoryFile, TRAJECTORY_FILE_ROW,
		robot.getX(),
		robot.getY(),
		robot.getRightVel(),
		robot.getLeftVel(),
		robot.getTh(),
		robot.getPose().getTh()
	);

	// closes file
	fclose(trajectoryFile);
}

void sickMap(void)
{
	// open sick map file
	sickMapFile = fopen(SICK_MAP_FILE_NAME, "a");

	// checking if we can actually write to it
	if (sickMapFile == NULL)
	{
		ArLog::log(ArLog::Terse, ERROR_ACCESSING_SICK_MAP_MESSAGE);
		return;
	};

	// declaring a list of readings + an iterator
	std::list<ArPoseWithTime*>* sickReadings;
	std::list<ArPoseWithTime*>::iterator sIt;

	// using the contents in the sick objec's buffer for the list of readings.
	sickReadings = sick->getCurrentBuffer();

	// writing the list of readings to the map file
	for (sIt = sickReadings->begin(); sIt != sickReadings->end(); sIt++)
	{
		// get x and y coordinates from the reading
		double x = (*sIt)->getX();
		double y = (*sIt)->getY();

		// read the readings into the map file
		fprintf(sickMapFile, SICK_MAP_FILE_ROW,
			x, y
		);
	};

	// close file
	fclose(sickMapFile);
}

void sonarMap(void)
{
	// open sonar map file
	sonarMapFile = fopen(SONAR_MAP_FILE_NAME, "a");

	// checking if we can write to the sonar map file
	if (sonarMapFile == NULL)
	{
		ArLog::log(ArLog::Terse, ERROR_ACCESSING_SONAR_MAP_MESSAGE);
		fclose(sonarMapFile);
		return;
	};

	// declaring a list of readings + an iterator
	std::list<ArPoseWithTime*>* sonarReadings;
	std::list<ArPoseWithTime*>::iterator sIt;
	// using the contents in the sonar objec's buffer for the list of readings.
	sonarReadings = sonar.getCurrentBuffer();

	// writing the list of readings to the map file
	for (sIt = sonarReadings->begin(); sIt != sonarReadings->end(); sIt++)
	{
		fprintf(sonarMapFile, SONAR_MAP_FILE_ROW,
			(*sIt)->getX(), (*sIt)->getY()
		);
	};

	// close sonar map file
	fclose(sonarMapFile);
}

/*
---------------------------------------------- MAIN ----------------------------------------------
*/

int main(int argc, char** argv)
{
	// Initialisation & priming the device.
	Aria::init();

	ArLog::init(ArLog::File,
		ArLog::Normal,
		"log.txt",
		true,
		true,
		true
	);

	// load and load our camera
	ArVCC4 camera(&robot);
	acts.openPort(&robot);
	camera.init();

	// Load argument parser.
	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();

	// Add our gripper
	gripper = new ArGripper(&robot);

	// Instantiate our laser
	sick = new ArSick();

	// Connect robot to argparser and laser to argparser.
	ArRobotConnector robotConnector(&argParser, &robot);
	ArLaserConnector laserConnector(&argParser, &robot, &robotConnector);

	// Always try to connect to the first laser:
	argParser.addDefaultArgument("-connectLaser");

	// Try and connect to the robot.
	if (!robotConnector.connectRobot())
	{
		ArLog::log(ArLog::Terse, "Could not connect to the robot.");
		if (argParser.checkHelpAndWarnUnparsed())
		{
			// -help not given, just exit.
			Aria::logOptions();
			Aria::exit(1);
		}
	}

	// Try and connect to the laser.
	if (!laserConnector.connectLaser(sick))
	{
		ArLog::log(ArLog::Terse, "Could not connect to the laser.");
		if (argParser.checkHelpAndWarnUnparsed())
		{
			// -help not given, just exit.
			Aria::logOptions();
			Aria::exit(1);
		}
	};

	// Trigger argument parsing
	if (!Aria::parseArgs() || !argParser.checkHelpAndWarnUnparsed())
	{
		Aria::logOptions();
		Aria::exit(1);
	}

	// Scrub and prime the output files.
	primeFiles();

	// Add sonar & sick -- they're globals, like the Robot.
	robot.addRangeDevice(&sonar);
	robot.addRangeDevice(sick);

	// Prime the robot's odometry and motor state
	ArPose space(3800, 3500, 270);
	robot.moveTo(space);
	robot.enableMotors();

	// Tilt the camera down 45 degrees to help it locate the ball
	camera.tilt(-45);

	// Add tasks to robot -- priorities by number.
	robot.addUserTask("mapTrajectory", 40, &trajectoryCB);
	robot.addUserTask("sickMap", 30, &sickMapCB);
	robot.addUserTask("sonarMap", 30, &sonarMapCB);

	// creating behaviour objects
	AvoidWall wallAvoid(sick);
	NavigateGate gateNav(sick);
	ChaseBall chaser(&acts, &camera, gripper);
	PushToGoal goalPush(gripper);
	Advance advancer;

	// adding our behaviour objects to our robot
	robot.addAction(&advancer, 1); // low priority
	robot.addAction(&gateNav, 2);
	robot.addAction(&wallAvoid, 3);
	robot.addAction(&chaser, 4);
	//robot.addAction(&goalPush, 5); // high priority

	// set the robot on its merry way
	sick->runAsync();
	robot.runAsync(true);

	// wait for robot task loop to end before exiting the program
	robot.waitForRunExit();

	// exit cleanly
	Aria::exit(0);
}
