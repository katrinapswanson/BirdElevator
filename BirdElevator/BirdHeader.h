#ifndef BIRDHEADER_H
#define BIRDHEADER_H

//Direction enum indicates motor configuration. 
enum DIRECTION {
	UP,
	DOWN,
	OFF
};

//Representation of bird elevator as state machine.
enum STATE {
	INVALID,
	MOVING_UP,
	MOVING_DOWN,
	WAITING,
};

//SIZE represents the number of entries maintained in history struct. May be changed
//to better suit the timing of actual sensor operation. NOISE represents a threshold for
//sensor jitter -- may be changed if needed. 
const char SIZE = 5;  //# of entries
const char NOISE = 5; //cm

//History struct stores recent readings from sensors for comparison.
typedef struct {
	double samples[SIZE];
	char index = 0;
	double average = 0;
} history;

//Prototypes for functions which take nonstandard-type parameters.
void setMotor(enum DIRECTION);
void store(history*, double);

#endif

