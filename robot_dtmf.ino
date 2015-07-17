#include <SPI.h>
#include <Adb.h>

// Define motor driver pins
// pins 9-13 are used by USB Host Shield 2.0
// use pins 0-8
int MotorA1 = 2; // left motor, forward
int MotorA2 = 3; // left motor, backward
int MotorB1 = 4; // right motor, forward
int MotorB2 = 5; // right motor, backward

// Move time and speed per button click
int moveTime = 500; // ms
int turnTime = 100; // ms
int Speed = 255; // speed 0..255

void setup()
{
  // Motor driver pins
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);
  
  // Initialise serial port (for debug purposes)
  Serial.begin(57600);
  // Initialise the ADB subsystem.  
  ADB::init();
  // Open an ADB stream to the phone's shell. Auto-reconnect
  ADB::addConnection("shell:exec logcat", true, adbEventHandler);  
}

void loop()
{
  // Poll the ADB subsystem.
  ADB::poll();
}

// Event handler for the shell connection. 
void adbEventHandler(Connection * connection, adb_eventType event, uint16_t length, uint8_t * data)
{
  // example of string received if button '1' pressed:
  // "D/Linphone( 5727): DTMF received: 49"
    
  int command;
  String str = "";
  String snum = "";

  if (event == ADB_CONNECTION_RECEIVE)
    for (int i=0; i<length; i++)
      str += (char)data[i];
    //Serial.print(str);
    
    int j = str.indexOf("DTMF");
    if (j >=0) {
      for (int k=0; k<2; k++)
        if (isDigit(str[j+15+k]))
          snum += str[j+15+k];

      command = snum.toInt()-48;
      Serial.print(str);
      Serial.println(command);
    }
  
  if (command == 2) moveForward();
  if (command == 8) moveBackward();
  if (command == 4) moveLeft();
  if (command == 6) moveRight();    

}

void moveForward()
{
  analogWrite(MotorA1, Speed);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, Speed);
  analogWrite(MotorB2, 0);
  delay(moveTime);
  moveStop();
}

void moveBackward()
{
  analogWrite(MotorA1, 0);
  analogWrite(MotorA2, Speed);
  analogWrite(MotorB1, 0);
  analogWrite(MotorB2, Speed );
  delay(moveTime);
  moveStop();
}

void moveLeft()
{
  analogWrite(MotorA1, 0);
  analogWrite(MotorA2, Speed);
  analogWrite(MotorB1, Speed);
  analogWrite(MotorB2, 0);
  delay(turnTime);
  moveStop();
}

void moveRight()
{
  analogWrite(MotorA1, Speed);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, 0);
  analogWrite(MotorB2, Speed);
  delay(turnTime);
  moveStop();
}

void moveStop()
{
  analogWrite(MotorA1, 0);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, 0);
  analogWrite(MotorB2, 0);
}
