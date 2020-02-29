/*This code is crude for making stepper motors move without
 * any control system within the code. All feedback system 
 * will be incorporated in later versions. IMU is used as the 
 * feedback sensor, but NANO contains code for "control system"
 * 
 * V1.0: Crude Code with no PID functions. 
 */

// defines pin and variable numbers
const int stepPin1 = 3; 
const int dirPin1 = 4; 
const int stepPin2 = 6;
const int dirPin2 = 7;
const int ENpin = 10;
int SPS = 0;

unsigned long steps;
unsigned long turnStps;
unsigned long T;

// Example 5 (from website) - Receive with start- and end-markers combined with parsing

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char dirRobot[numChars] = {0};
float degreeVal = 0.0;
int motorSpd = 0;
int timeSet = 0;

boolean newData = false;

//============

void setup() {
    Serial.begin(9600);
    Serial.println("This expects board to receive direction, degrees, speed percentage, and time desired (ms)");
    Serial.println("Enter data in this style <Foward, 45.5, 50, 2500>  ");
    Serial.println();
  
// Sets the two pins as Outputs
    pinMode(stepPin1,OUTPUT); 
    pinMode(dirPin1,OUTPUT);
    pinMode(stepPin2,OUTPUT); 
    pinMode(dirPin2,OUTPUT);
    pinMode(ENpin,OUTPUT);
  
    digitalWrite(ENpin, HIGH); //determine the power state of driver. LOW = on and HIGH = off
 

//Define the speed of motors (steps/sec), 1rev = 200steps
    SPS = 1000*(motorSpd/100);  
//Calculate the amount of steps for foward/backward direction
    steps = SPS*(timeSet/1000);
//Calculate the amount of steps for turning 
    turnStps = (200*degreeVal)/360;
//Calculate pulse period
    T = 1000000/SPS;
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
      if (strcmp(dirRobot,"Foward") == 0){
        moveFoward();
        showParsedData();
        Serial.println("Moved Foward!");
      } else if (strcmp(dirRobot,"Backward") == 0) {
        moveBackward();
        showParsedData();
        Serial.println("Moved Backward!");
      } else if (strcmp(dirRobot,"Turn") == 0) {
        turnRobot();
        showParsedData();
        Serial.println("You turned... TO THE DARK SIDE!");
      }
        newData = false;
    }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(dirRobot, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    degreeVal = atof(strtokIndx);     // convert this part to an float, floatFromPC

    strtokIndx = strtok(NULL, ",");
    motorSpd = atoi(strtokIndx);     // convert this part to a int, integerFromPC

    strtokIndx = strtok(NULL, ",");
    timeSet = atoi(strtokIndx);  

}

/*============
 * These functions determine how the robot will move
 */
void moveFoward() {
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, LOW);
    for(int x = 0; x < steps; x++) {
      digitalWrite(stepPin1,HIGH);
      digitalWrite(stepPin2,HIGH);
      delayMicroseconds(T/2);
      digitalWrite(stepPin1,LOW);
      digitalWrite(stepPin2,LOW);
      delayMicroseconds(T/2);
     }
}

void moveBackward() {
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, HIGH);
    for(int x = 0; x < steps; x++) {
      digitalWrite(stepPin1,HIGH);
      digitalWrite(stepPin2,HIGH);
      delayMicroseconds(T/2);
      digitalWrite(stepPin1,LOW);
      digitalWrite(stepPin2,LOW);
      delayMicroseconds(T/2);
     }
}

void turnRobot() {
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, HIGH);
    for(int x = 0; x < turnStps; x++) {
      digitalWrite(stepPin1,HIGH);
      digitalWrite(stepPin2,HIGH);
      delayMicroseconds(T/2);
      digitalWrite(stepPin1,LOW);
      digitalWrite(stepPin2,LOW);
      delayMicroseconds(T/2);
     }
}

//============

void showParsedData() {
    Serial.print("Direction: ");
    Serial.println(dirRobot);
    Serial.print("Degrees: ");
    Serial.println(degreeVal);
    Serial.print("Speed (%): ");
    Serial.println(motorSpd);
    Serial.print("Time (ms): ");
    Serial.println(timeSet);
}
