//Initialization of four length sensors (left, bottom, right, top) and FSR for control of data
const int left = A0;
const int bottom = A1;
const int top = A2;
const int right = A3;
const int button_fsr = A4;
const int pot_pin = A5;
const int ledPin = 9;

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
int incomingByte;



void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  
}

void loop() {

  if (Serial.available() > 0) {
    top_len = mmap(analogRead(top),213,964,5.26,8.34);
    right_len = mmap(analogRead(right),375,957,5.30,8.2);
    bottom_len = mmap(analogRead(bottom),230,970,5.27,8.21);
    left_len = mmap(analogRead(left),220,986,5.25,8.32);

    inputVal = analogRead(pot_pin);

    //mapping the potentiometer values (0-1024) to angles on RANDALL
    angle1 = (map(inputVal, 54, 389, 180, 90));
    square();
    incomingByte = Serial.read();
    
    if (incomingByte == 'l') {
      Serial.println(left_len);
    }
    if (incomingByte == 'r') {
      Serial.println(right_len);
    }
    if (incomingByte == 't') {
      Serial.println(top_len);
    }
    if (incomingByte == 'b') {
      Serial.println(bottom_len);
    }
    if (incomingByte == '1') {
      Serial.println(angle1);
    }
    if (incomingByte == '2') {
      Serial.println(degrees(angle2));
    }
    if (incomingByte == '3') {
      Serial.println(degrees(angle3));
    }
    if (incomingByte == '4') {
      Serial.println(degrees(angle4));
    }
    
  }
  button = analogRead(button_fsr);

  if (button > 150) {
    top_len = mmap(analogRead(top),213,964,5.26,8.34);
    right_len = mmap(analogRead(right),375,957,5.30,8.2);
    bottom_len = mmap(analogRead(bottom),230,970,5.27,8.21);
    left_len = mmap(analogRead(left),220,986,5.25,8.32);

    inputVal = analogRead(pot_pin);

    //mapping the potentiometer values (0-1024) to angles on RANDALL
    angle1 = (map(inputVal, 54, 389, 180, 90));
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

float mmap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
