
/*Bird Elevator (i.e., A Wish For Wings That Work)
 * Author: Katrina Swanson
 * Date: 09/25/2017
 * Version: 1.0
 * Description: This program runs a homebuilt elevator controlled by two 
 * distance sensors. One sensor, mounted at floor level, determines when a 
 * bird has entered the elevator. The second sensor, mounted at the top of the 
 * bird cage, determines when the elevator has reached the top of the cage. 
 * 
 * Sensors used are Sharp GP2Y0A21YK 10-80cm General Purpose Type Distance 
 * Measuring Sensors. Sensor output curve is from the Sharp datasheet located at
 * https://www.sparkfun.com/datasheets/Components/GP2Y0A21YK.pdf as accessed 
 * September 25th, 2017.
 * 
 * Environment: Program written on Intel Latitude PC running Windows 10 and 
 * compiles under Arduino IDE v.1.8.4.
*/
#include "BirdHeader.h"

//Establish initial elevator state.
enum STATE elevator_state = WAITING;

//Structs used to store historical sensor values between iterations of loop().
history birdSensor;
history topSensor;


/* Fxn: setup()
 *  Arduino initialization. Establishes pins 1-4 in output mode. Pins link to
 *  motor to set motor state.
*/
void setup() {
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

/* Fxn: loop()
 *  Arduino main body loop. Collects readings from both sensors. Sets state 
 *  based on comparing recent sensor readings to current reading and determining 
 *  if reading has changed. 
*/
void loop() {
  double birdDistance = sensorConvert(analogRead(A0));
  double topDistance = sensorConvert(analogRead(A1));
  
  switch (elevator_state) {
    case MOVING_UP: 
      setMotor(UP);
      store(&topSensor, topDistance);
      if (topDistance < topSensor.average - NOISE) //bird reached the top of the cage
      {
        delay(10000);
        elevator_state = MOVING_DOWN;
      }
      break;
    case MOVING_DOWN:
      setMotor(DOWN);
      store(&birdSensor, birdDistance);
      if (birdDistance > birdSensor.average + NOISE) //elevator reached the floor
        {
          elevator_state = WAITING;
        }
      break;
    case WAITING: //elevator waits at floor height
      setMotor(OFF);
      store(&birdSensor, birdDistance);
      if(birdDistance < birdSensor.average - NOISE) //bird got on the elevator
      {
        elevator_state = MOVING_UP;
      }
      break;
    default:
      setMotor(OFF);
    break;
  }
}

/* Fxn: sensorConvert()
 *  Distance sensor conversion. Accepts sensor reading as an integer value from 0 - 1023 
 *  (from analogRead()) and returns the distance measurement in cm as a double.
 *  
 *  Sensor is a Sharp GP2Y0A21YK 10-80cm General Purpose Type Distance Measuring Sensor. Sensor 
 *  output curve and voltage conversion constant obtained from the sensor datasheet linked at 
 *  the top of this program. 
*/
double sensorConvert(int sensorIn) {
  //Equation fit to sensor output curve, courtesy of Wolfram Alpha.
  
  float voltage = sensorIn * 0.0049;
  double distance = 7.4104e-1 + (40461870 - 7.41049e-1)/(1 + pow((voltage / 8.21102e-6), 1.21768));

  return distance;
}

/* Fxn: setMotor()
 *  Sets voltage values of output pins 1-4 to control direction of the motor. Accepts an enum DIRECTION
 *  value to specify the appropriate pin settings. Function prototyped in BirdHeader.h.
*/
void setMotor(enum DIRECTION dir) {
  switch(dir) {
    case UP: 
    digitalWrite(1, HIGH);
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    break;
    case DOWN: 
    digitalWrite(1, LOW);
    digitalWrite(2, HIGH);
    digitalWrite(3, LOW);
    digitalWrite(4, HIGH);
    break;
    default: 
    digitalWrite(1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    break;
  }
}

/* Fxn: store()
 *  Adds current sensor reading to struct containing recent sensor readings. Accepts a pointer to 
 *  the history struct of interest and a double value to store in the struct. History struct
 *  and SIZE of member array are defined in BirdHeader.h.
*/
void store(history* p, double toStore) 
{
  p->samples[p->index] = toStore;
  p->index++;
  if (p->index == SIZE)
    p->index = 0;
  double sum = 0;
  for (char i = 0; i < SIZE; i++) 
  {
    sum += p->samples[i];
  }
  p->average = sum / SIZE;
}

