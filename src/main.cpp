// RC PulseIn Serial Read out By: Nick Poole
// extended By Jason Mclaughlin
// 2015
#include <Servo.h> // for servo in example
#include <Arduino.h>
#include <SoftwareSerial.h>

Servo myservo; // servo entity
int pos = 0; // position for servo
//int ch1, ch2, ch3, ch4, ch5, ch6;
char outStr[256];
char buffer[256];
unsigned long reading;
u16 rdng; // We expect u16's, but might get UL's
int percent;
u16 rdng_hi[6];
u16 rdng_lo[6] = {5000};

#define FIRST_PIN 2
#define LAST_PIN 7
#define FIRST_CH (n) - FIRST_PIN
#define LAST_CH (n) - LAST_PIN

void setup()
{
  //myservo.attach(9);
  for (int i = FIRST_PIN; i<=LAST_PIN; i++){
    pinMode(i, INPUT);
  }

  Serial.begin(115200);
}

void loop()
{
  strcpy(outStr, "\0");
  int i = 1;
  for (int i = 0; i <= (LAST_PIN - FIRST_PIN); i++){
    reading = pulseIn(i + FIRST_PIN, HIGH, 50000);
    if (reading > UINT16_MAX){
      reading = UINT16_MAX;
    }

    rdng = (u16)reading; // danger!

    // check for changing bounds
    if ((rdng > 500) && (rdng < rdng_lo[i])){
      rdng_lo[i] = rdng;
    } 
    
    if (rdng > rdng_hi[i]){
      rdng_hi[i] = rdng;
    }

    // map reading within bounds to 0-100%
    // return = map(value, fromLow, fromHigh, toLow, toHigh)
    percent = map(reading, rdng_lo[i], rdng_hi[i], 0, 100);
    sprintf(buffer,  "ch%i, %5i ", i + 1, percent);
    strcat(outStr, buffer);
    strcpy(buffer, "\0"); // clear for next loop
  }

  //sprintf(outStr, "Channel 1:%i\t2:%i\t3:%i4:%i\t5:%i\t6:%i\n", ch1, ch2, ch3, ch4, ch5,ch6);
  Serial.println(outStr); // Print the value of

  /*if ((ch3 >= 1500) && (ch3 <= 1600))
  { // the center postion for controller
    Serial.println("between");
    pos = 90; // set servo to center
    myservo.write(pos); // set to pos which is 90
  }

  else
  {
    Serial.println("not between");
    for (pos = 0; pos < 180; pos += 1) // goes from 0 degrees to 180 degrees
    { // in steps of 1 degree
      myservo.write(pos); // tell servo to go to position in variable 'pos'
      delay(1); // waits 1ms for the servo to reach the position
    }
    for (pos = 180; pos >= 1; pos -= 1) // goes from 180 degrees to 0 degrees
    {
      myservo.write(pos); // tell servo to go to position in variable 'pos'
      delay(1); // waits 1ms for the servo to reach the position
    }
  }*/
  delay(10); // I put this here just to make the terminal
  // window happier
}
