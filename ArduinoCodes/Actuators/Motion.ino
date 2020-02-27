String readString;

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
 
void setup() {
  Serial.begin(9600);
  
  // Sets the two pins as Outputs
  pinMode(stepPin1,OUTPUT); 
  pinMode(dirPin1,OUTPUT);
  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
  pinMode(ENpin,OUTPUT);
  
  digitalWrite(ENpin, HIGH); //determine the power state of driver. LOW = on and HIGH = off
 
  //Define the amount of time (ms) wanted
  myDesiredTime=2000;

  //Define the speed of motors (steps/sec), 1rev = 200steps
  SPS = 1000;
  
  // calculate the amount of steps
  steps = SPS*(myDesiredTime/1000);
  //Calculate pulse period
  T = 1000000/SPS;
}

void loop() { 
if (Serial.available() >0) {
  var = Serial.read();

  switch (var) {
    //Make robot stop.
    case 83:
        stopRobot();
      break;
    //Make robot move forward.
    case 70:
        moveForward();
      break;
    //Make robot move backward.
    case 66:
        moveBackward();
      break;
    }
  }
}

void moveForward () {
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

void moveBackward () {
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

void stopRobot () {
      digitalWrite(stepPin1, LOW);
      digitalWrite(stepPin2, LOW);
}
