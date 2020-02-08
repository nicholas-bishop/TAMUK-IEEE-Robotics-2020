// defines pins numbers
const int stepPin1 = 3; 
const int dirPin1 = 4; 
const int stepPin2 = 6;
const int dirPin2 = 7;
 
void setup() {
  // Sets the two pins as Outputs
  pinMode(stepPin1,OUTPUT); 
  pinMode(dirPin1,OUTPUT);
  pinMode(stepPin2,OUTPUT); 
  pinMode(dirPin2,OUTPUT);
}
void loop() {
  digitalWrite(dirPin1,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin1,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin1,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay

    digitalWrite(dirPin2,HIGH); // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for(int x = 0; x < 200; x++) {
    digitalWrite(stepPin2,HIGH); 
    delayMicroseconds(500); 
    digitalWrite(stepPin2,LOW); 
    delayMicroseconds(500); 
  }
  delay(1000); // One second delay
  
  digitalWrite(dirPin1,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 400; x++) {
    digitalWrite(stepPin1,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin1,LOW);
    delayMicroseconds(500);
  }
  delay(1000);

    digitalWrite(dirPin2,LOW); //Changes the rotations direction
  // Makes 400 pulses for making two full cycle rotation
  for(int x = 0; x < 400; x++) {
    digitalWrite(stepPin2,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin2,LOW);
    delayMicroseconds(500);
  }
  delay(1000);
}




  if(stopTime-startTime<=myDesiredTime && x <= steps) 
  {
      stopTime=millis();
      digitalWrite(stepPin1,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin1,LOW);
      delayMicroseconds(500);
      x++;
      Serial.print(stopTime);
    }
