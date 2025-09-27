void Servo_Run(int pan_angle) {
    myservo.attach(0,600,2600);  // attaches the servo on pin 9 to the servo object
    myservo.write(pan_angle);
    delay(300);
    myservo.detach();
}

void Old_Servo_Run(int pan_angle) {
   myservo.attach(0,600,2600);  // attaches the servo on pin 9 to the servo object

  int pos;
  if (pan_angle > last_angle) {
    for (pos = last_angle; pos <= pan_angle; pos += 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(25);                       // waits 15 ms for the servo to reach the position
    }
  } else {
    for (pos = last_angle; pos >= pan_angle; pos -= 5) { // goes from 0 degrees to 180 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(25);                       // waits 15 ms for the servo to reach the position
    }
  }
  last_angle = pos;
  myservo.detach();
}

void Servo_test() {
  int pos = 0;    // variable to store the servo position

  for (pos = 0; pos <= 180; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(25);                       // waits 15 ms for the servo to reach the position
  }
  delay(2000);
  for (pos = 180; pos >= 0; pos -= 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(25);                       // waits 15 ms for the servo to reach the position
  }
  delay(5000);
}
