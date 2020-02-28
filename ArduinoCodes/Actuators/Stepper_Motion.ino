// defines pin and variable numbers
const int stepPin1 = 3; 
const int dirPin1 = 4; 
const int stepPin2 = 6;
const int dirPin2 = 7;
const int ENpin = 10;
int var = 0;
int SPS = 0;

// defines the time variables
unsigned long myDesiredTime;
unsigned long steps;
unsigned long T;

// Example 5 - Receive with start- and end-markers combined with parsing

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char dirRobot[numChars] = {0};
int degreeValue = 0;
float timeSet = 0.0;

boolean newData = false;

//============

void setup() {
    Serial.begin(9600);
    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    Serial.println();
  
// Sets the two pins as Outputs
    pinMode(stepPin1,OUTPUT); 
    pinMode(dirPin1,OUTPUT);
    pinMode(stepPin2,OUTPUT); 
    pinMode(dirPin2,OUTPUT);
    pinMode(ENpin,OUTPUT);
  
    digitalWrite(ENpin, HIGH); //determine the power state of driver. LOW = on and HIGH = off
 
//Define the amount of time (ms) wanted
//    myDesiredTime=2000;
//Define the speed of motors (steps/sec), 1rev = 200steps
    SPS = 1000;  
// calculate the amount of steps
//    steps = SPS*(myDesiredTime/1000);
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
        showParsedData();
        
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
    degreeValue = atoi(strtokIndx);     // convert this part to an integer, integerFromPC

    strtokIndx = strtok(NULL, ",");
    timeSet = atof(strtokIndx);     // convert this part to a float, floatFromPC

}

//============
//This function can also be used to set the motion for the steppers!

void showParsedData() {
    Serial.print("Direction: ");
    Serial.println(dirRobot);
    Serial.print("Degrees: ");
    Serial.println(degreeValue);
    Serial.print("Time: ");
    Serial.println(timeSet);
}
