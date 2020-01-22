#include <Sparki.h>

#define STATE_INITIAL 0
#define STATE_ROTATE_FIND_OBJECT 1
#define STATE_GO_TO_OBJECT 2
#define STATE_PICK_UP_OBJECT 3
#define STATE_ROTATE_180 4
#define STATE_DRIVE_TO_LINE 5
#define STATE_FOLLOW_LINE 6
#define STATE_FINAL 7

// Set up some global variables with default values to be replaced during operation
int current_state = STATE_INITIAL;
const int threshold = 700; // IR reading threshold to detect whether there's a black line under the sensor
int cm_distance = 1000;
int line_left = 1000;
int line_center = 1000;
int line_right = 1000;


void setup() {
  // put your setup code here, to run once:
  sparki.RGB(RGB_RED); // Turn on the red LED
  sparki.servo(SERVO_CENTER); // Center the ultrasonic sensor
  delay(1000); // Give the motor time to turn
  sparki.gripperOpen(); // Open the gripper
  delay(5000); // Give the motor time to open the griper
  sparki.gripperStop(); // 5 seconds should be long enough
  sparki.RGB(RGB_GREEN); // Change LED to green so we know the robot's setup is done!
}

void readSensors() {
  cm_distance = sparki.ping(); // Replace with code to read the distance sensor
  line_left = sparki.lineLeft(); // Replace with code to read the left IR sensor
  line_right = sparki.lineRight(); // Replace with code to read the right IR sensor
  line_center = sparki.lineCenter(); // Replace with code to read the center IR sensor
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors(); // Read sensors once per loop() call

  sparki.clearLCD();
  sparki.print("STATExx: ");
  sparki.println(current_state);

  switch(current_state)
  {
    case STATE_INITIAL:
      current_state = STATE_ROTATE_FIND_OBJECT;
    case STATE_ROTATE_FIND_OBJECT:
      if(cm_distance < 30)
        current_state = STATE_GO_TO_OBJECT;
      else
        //keep rotating
        sparki.moveRight();
      break;
    case STATE_GO_TO_OBJECT:
      if(cm_distance >= 7){
        sparki.moveForward(1);
      }
      else{
        current_state = STATE_PICK_UP_OBJECT;
      }
      break;
    case STATE_PICK_UP_OBJECT:
      sparki.gripperClose();
      break;
    case STATE_ROTATE_180:
      break;
    case STATE_DRIVE_TO_LINE:
      if(line_center < threshold)
        //we've reached the line
        current_state = STATE_FOLLOW_LINE;
       else
        sparki.moveForward(1);
      break;
    case STATE_FOLLOW_LINE:
        if ( lineLeft < threshold ){  
          sparki.moveLeft(); // turn left
        }
        if ( lineRight < threshold ){  
          sparki.moveRight(); // turn right
        }
       
        // if the center line sensor is the only one reading a line
        if ( (lineCenter < threshold) && (lineLeft > threshold) && (lineRight > threshold) )
        {
          sparki.moveForward(); // move forward
        }  
      break;
    case STATE_FINAL:
      break;
  }

  sparki.updateLCD();
  delay(100); // Only run controller at 10Hz
}
