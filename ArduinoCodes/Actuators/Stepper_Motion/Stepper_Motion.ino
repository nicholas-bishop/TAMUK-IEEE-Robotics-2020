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

// Example 5 (from website) - Receive with start- and end-markers combined with parsing

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

// variables to hold the parsed data
char dirRobot[numChars] = {0};
float degreeVal = 0.0;
int motorSpd = 0;
int timeSet = 0;
int LED;

boolean newData = false;

//Define the speed of motors (steps/sec), 1rev = 200steps
int SPS = 1000*(motorSpd/100);  
//Calculate the amount of steps for foward/backward direction
unsigned long steps = SPS*(timeSet/1000);
//Calculate the amount of steps for turning 
unsigned long turnStps = (200*degreeVal)/360;
//Calculate pulse period
unsigned long T = 1000000/SPS;

//============

void setup() {
    Serial.begin(9600);
    Serial.println("This expects board to receive direction, degrees, speed percentage, and time desired (ms)");
    Serial.println("Enter data in this style <Forward, 45.5, 50, 2500>  ");
    Serial.println();
  
// Sets the two pins as Outputs
    pinMode(stepPin1,OUTPUT); 
    pinMode(dirPin1,OUTPUT);
    pinMode(stepPin2,OUTPUT); 
    pinMode(dirPin2,OUTPUT);
    pinMode(ENpin,OUTPUT);
    pinMode(LED, OUTPUT);
  
    digitalWrite(ENpin, HIGH); //determine the power state of driver. LOW = on and HIGH = off
 
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        
        parseData(tempChars);

        // Print calculated data values - EmeraldRaspberry
        Serial.println("================");
        Serial.print("SPS: ");
        Serial.println(SPS);
        Serial.print("Steps: ");
        Serial.println(steps);
        Serial.print("TurnStps: ");
        Serial.println(turnStps);
        Serial.print("T: ");
        Serial.println(T);
        Serial.println("================");
        
      if (strcmp(dirRobot,"Forward") == 0){
        moveFoward();
        showParsedData();
        Serial.println("Moved Forward!");
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

int parseData(char* x) {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(x,",");      // get the first part - the string
    strcpy(dirRobot, strtokIndx); // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    degreeVal = atof(strtokIndx);     // convert this part to an float, floatFromPC

    strtokIndx = strtok(NULL, ",");
    motorSpd = atoi(strtokIndx);     // convert this part to a int, integerFromPC

    strtokIndx = strtok(NULL, ",");
    timeSet = atoi(strtokIndx);

    Serial.println("Parsing data...");
    Serial.print("degreeVal <- ");
    Serial.println(degreeVal);
    Serial.print("motorSpd <- ");
    Serial.println(motorSpd);
    Serial.print("timeSet <- ");
    Serial.println(timeSet);

    // Equations used for processing data - EmeraldRaspberry
    SPS = (int)(1000*( (double) motorSpd/100)); 
    steps = (unsigned long)(SPS*((double) timeSet/1000));
    turnStps = (unsigned long)((double)(200*degreeVal)/360);
    T = (unsigned long)((double)1000000/SPS);
    
    return(0);

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
