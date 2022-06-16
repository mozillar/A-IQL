#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H
const int WIDTH = 9;
const int HEIGHT = 9;
// unit for discretization. It can be set to a posotive value rather than 1.0 if the unit for movement is not 1.0.(the default unit for movement is 1.0)
const double UNIT_DISCRETE = 1.0;
// the index of point for detecting collision
const int POINT_COUNT = 11;
const int LEFT_UP = 0;
const int RIGHT_UP = 1;
const int LEFT_DOWN = 2;
const int RIGHT_DOWN = 3;
const int CENTER = 4;
const int MIDDLE_LEFT = 5;
const int MIDDLE_RIGHT = 6;
const int MIDDLE_UP = 7;
const int MIDDLE_DOWN = 8;
const int LEFT_CENTER = 9;
const int RIGHT_CENTER = 10;

// the action of the worker(agent/robot)
const int STAY = 0;
const int UP = 1;
const int DOWN = 2;
const int LEFT = 3;
const int RIGHT = 4;
// the unit for movement
const double UNIT_DIS = 1.0;
// noise in each movement
const double RAND_DIS = 0.0; 
const int MAX_BAR_COUNT = 4;                     // the number of barriers
const int MAX_STATE_COUNT = 59049;               // the number of states (9*9*3)^2 = 59049
const int MAX_ACTION_COUNT = 5;                  // the number of actions of the worker(agent/robot)
const int MAX_MO_COUNT = 3;                      // the number of MovingObject: robot1, robot2, and the combination of robot1, robot2 and the box (when the box is grasped by both the robots)
const int WORKER1 = 0;                           // the index of agent1/robot1
const int WORKER2 = 1;                           // the index of agent2/robot2
const int COMBINATION = 2;                       // the index of the combination of agent1, agent2 and the box

const int MAX_TEAMMATE = 2;                      // the number of workers(agents/robots)
const int TEAM1 = 0;
const int MODE_COUNT = 2; 
const double THRESHOLD = 1.0E-3;

const int MINIMAL_STEP = 14;                     // the minimal(optimal) steps used for the task in the second half of the training stage and the evaluation stage



const int MAX_RUN_COUNT = 50;                    // The number of times of experiment. One run contains MAX_LEARNING_COUNT episodes for training and MAX_EVALUATION_COUNT episodes for evaluation.
const int MAX_LEARNING_COUNT = 40000;            // The number of episodes for training in one experiment (one run). L in the paper.
const int MAX_EVALUATION_COUNT = 1;              // The number of episodes for evaluation in one experiment (one run).
const int MAX_EPISODE_COUNT = MAX_LEARNING_COUNT + MAX_EVALUATION_COUNT;  //DO NOT CHANGE THIS LINE.   The number of total episodes in one experiment (one run).
const int MAX_STEP_COUNT = 400;                  // The maximal number of steps in an episode
const int episodePerBlock = 1000;                
static unsigned seed;                            // seed for generating random numbers
const double NEG_INF = -10000000.0;              // negative infinity (used to find the maximal element in an array)

//FMRQ
const int SV_MAX = 30;

#endif