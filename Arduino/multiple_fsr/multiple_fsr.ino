/********************************************************************
  This program is used for finding the length and angles of the
    Smart Quadrialteral Manipulative - Spring 2021
    By: Scott Lambert, CHROME Lab Parks College @ SLU

  The length sensors used are 4inch Force Sensitive Resistors,
  and one potentiometer for angle sensing.

  So many times I shot myself in the foot with radian vs degree calculations
  and just being an idiot in general and especially with the length variables.

  Enjoy the finished product because I sure do after the self-induced frustration.

*********************************************************************/
#include <SoftwareSerial.h>

//Initialization of four length sensors (left, bottom, right, top) and FSR for control of data
const int left = A0;
const int bottom = A1;
const int top = A2;
const int right = A3;
const int button_fsr = A4;
const int pot_pin = A5;
int incomingByte;

// Whole lot of variables used in computation and stuff
float inputVal;
float center_len;
float angle1;
float angle2;
float angle3a;
float angle3b;
float angle3;
float angle4;
float left_len;
float bottom_len;
float top_len;
float right_len;
float button;

/*SoftwareSerial MyBlue(2, 3); // RX | TX //Bluetooth connection
const int ledPin = 8;
String msg;
*/
void setup()
{
  //pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, LOW);

  //MyBlue.begin(9600);
  Serial.begin(9600);

  //msg = "";
  //honestly not sure how necessary these lines are...
  pinMode(left, INPUT);
  pinMode(bottom, INPUT);
  pinMode(top, INPUT);
  pinMode(right, INPUT);
  pinMode(button_fsr, INPUT);
  pinMode(pot_pin, INPUT);
}

void loop()
{
  /*This is all Jen's, for the bluetooth stuff
  /if (MyBlue.available() > 0) { // Check if any data is waiting
    msg = MyBlue.readString();
    //Serial.println("Tablet: " + msg);
  }

  if (msg == "<turn on>") {
    digitalWrite(ledPin, HIGH);
    Serial.println(msg);*/
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'H') {
      digitalWrite(ledPin, HIGH);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') {
      digitalWrite(ledPin, LOW);
    }
    if (incomingByte == 'D') {
      left_len = analogRead(left);
      left_len = map(left_len,392,999,5.30,8.29);
      Serial.println(left_len);
    }
  }
  if (button > 150) // If the singular FSR gets pressed,
      //then data calculations are done and results printed

    {
      //The division by 250 and addition of ~ 4.4 is calibration for RANDALL
      left_len = analogRead(left) / 250 + 4.45;
      bottom_len = analogRead(bottom) / 250 + 4.4;
      right_len = analogRead(right) / 250 + 4.4;
      top_len = analogRead(top) / 250 + 4.45;

      inputVal = analogRead(pot_pin);
      //mapping the potentiometer values (0-1024) to angles on RANDALL
      angle1 = (map(inputVal, 66, 550, 180, 60));
      square();

      //MyBlue.println(top_len);
      delay(500);
    }
    //MyBlue.println("Arduino: Hello from Arduino"); //send to Android
    delay(1000);
    //msg = ""; // reset command


  }

  else {
    //if (msg == "<turn off>") {
      //digitalWrite(ledPin, LOW);
      //Serial.println("LED is off\n");
      //msg = "";
    }
  }

  //here's my stuff, for the sensors and output
  button = analogRead(button_fsr);

  if (button > 150) // If the singular FSR gets pressed, data calculations are done and results printed
  {
    //The division by 250 and addition of ~ 4.4 is calibration for RANDALL
    left_len = map(analogRead(left),392,999,5.30,8.29);
    bottom_len = analogRead(bottom) / 250 + 4.4;
    right_len = analogRead(right) / 250 + 4.4;
    top_len = analogRead(top) / 250 + 4.45;

    inputVal = analogRead(pot_pin);
    //mapping the potentiometer values (0-1024) to angles on RANDALL
    angle1 = (map(inputVal, 66, 550, 180, 60));
    square();

    Serial.println(top_len);
    Serial.println(right_len);
    Serial.println(bottom_len);
    Serial.println(left_len);
    Serial.println();

    
    //had lots of trouble with radians & degrees
    //Angle 1 is mapped to degrees, then converted to radians for calcuations
    //other angles are calculated in radians and converted to degrees during output:
    Serial.println(angle1);
    Serial.println(degrees(angle2));
    Serial.println(degrees(angle3));
    Serial.println(degrees(angle4));

    delay(500);
  }
  else
  {
    // No pressure detected
  }
}

void square() {
  //This is the calculation of all angles with 4 side lengths and 1 angle known
  //Credit to Anthony Rehberger, for helping me figure this out lol bless high school geometry
  //Uses Law of Cosines to find center length, then again to find angle 2 (opposite known angle)
  //Angle 3 is found using the Law of Sines twice, then added. Angle 4 is what is left over so 360 - other angles
  //Angle 3 is calculated as 3a and 3b for debugging, can be combined if you really care about variable streamlining

  center_len = sqrt((sq(bottom_len) + sq(right_len)) - (2 * bottom_len * right_len * cos(radians(angle1))));
  angle2 = acos((sq(left_len) + sq(top_len) - sq(center_len)) / (2 * left_len * top_len));
  angle3a = asin(right_len * sin(radians(angle1)) / center_len);
  angle3b = asin(top_len * sin(angle2) / center_len);
  angle3 = angle3a + angle3b;
  angle4 = 2 * 3.1415 - radians(angle1) - angle2 - angle3;


}


float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
