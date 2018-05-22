#pragma once

// Filenames, headers, and formatting.
const char TRAJECTORY_FILE_NAME[] = "trajectory1.csv"; // file to write to
const char TRAJECTORY_FILE_HEADER[] = "PosX, PosY, v(right), v(left), local theta, pose theta \n"; // header of said file
const char TRAJECTORY_FILE_ROW[] = "%f, %f, %f, %f, %f, %f \n"; // "template" of file rows

const char SONAR_MAP_FILE_NAME[] = "sonar_map.csv";
const char SONAR_MAP_FILE_HEADER[] = "X, Y \n ";
const char SONAR_MAP_FILE_ROW[] = "%f, %f \n";

const char SICK_MAP_FILE_NAME[] = "sick_map.csv";
const char SICK_MAP_FILE_HEADER[] = "X, Y \n";
const char SICK_MAP_FILE_ROW[] = "%f, %f \n";

// Error Messages
const char ERROR_ACCESSING_SICK_MAP_MESSAGE[] = "ERROR: SICK map could not be opened.";
const char ERROR_ACCESSING_SONAR_MAP_MESSAGE[] = "ERROR: Sonar map could not be opened.";
const char ERROR_ACCESSING_TRAJECTORY_MAP_MESSAGE[] = "ERROR: Trajectory map could not be opened.";

// logging messages
const char FILES_PRIMED_PRIMING[] = "Priming files...";
const char TRAJECTORY_FILE_PRIMED[] = "Trajectory file primed...";
const char SICK_FILE_PRIMED[] = "SICK file primed...";
const char SONAR_FILE_PRIMED[] = "SONAR file primed...";
const char FILES_PRIMED_SUCCESS[] = "Files primed successfully...";

const char AVOIDWALL_CALLED[] = "AvoidWall called...";
const char AVOIDWALL_ACTIVATED[] = "AvoidWall activated (%f) - L=(%f) R=(%f) -- rotating...";

const char CHASEBALL_CALLED[] = "ChaseBall called...";
const char CHASEBALL_ACTIVATED[] = "ChaseBall activated-- following ball...";

const char GATENAV_CALLED[] = "GateNav called...";
const char GATENAV_ACTIVATED[] = "GateNav activated-- navigating gate...";
const char GATENAV_TURN_RIGHT[] = "GateNav: Turning right...";
const char GATENAV_TURN_LEFT[] = "GateNav: Turning left...";

const char ADVANCE_CALLED[] = "Advance called...";
const char ADVANCE_ACTIVATED[] = "Advance activated-- advancing...";

const char CHASER_CALLED[] = "ChaseBall called...";
const char CHASER_LOST_TARGET[] = "ChaseBall lost target...";
const char CHASER_TARGET_ACQUIRED[] = "ChaseBall TARGET ACQUIRED";
const char CHASER_OPENING_CLAWS[] = "ChaseBall opening claws...";

const char PUSH2GOAL_CALLED[] = "PushToGoal called...";
const char PUSH2GOAL_CLOSECLAWS[] = "PushToGoal - closing claws...";
const char PUSH2GOAL_ACTIVATED[] = "PushToGoal activated-- pushing robot to goal...";

// advance constants
const int ADVANCE_VELOCITY = 70;
const int ROTATION_STEP = 10;

// wall avoid constants							  
const int WALL_AVOID_DISTANCE = 900;
const int ROTATIONAL_VELOCITY_WALL = 30;

// gate constants
const int GATE_NOTICE_DISTANCE = 700;
const int GATE_TOO_CLOSE_DISTANCE = 500;
const int GATE_POST_DISTANCE = 1400;

const int ROTATIONAL_VELOCITY_GATES = 10;
const int GATE_ROTATION_STEP = 5;
const int GATE_TOLERANCE = 100;

// map constants
const int GOAL_MIN_X = 1250;
const int GOAL_MAX_X = 3250;
const int GOAL_Y = 3800;