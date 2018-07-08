
#define SPEEDSENSOR_POTENTIO_RATIO 0.05

void getSpeedSensor( int leftPotentio, int rightPotentio) {
  // min() to prevent spike in speed ready
  float readspeed = SpeedSensorSerial.parseFloat();

  if ( readspeed < 0) {
    //rightspeedsensor = -readspeed;
    rightspeedsensor = min(-readspeed*SPEEDSENSOR_POTENTIO_RATIO, abs(rightPotentio)*3/2);
    //Serial.print(readspeed);
    //Serial.print("right"); Serial.print(readspeed);
  } else {
    
    leftspeedsensor = min(readspeed*SPEEDSENSOR_POTENTIO_RATIO, abs(leftPotentio)*3/2);
    //leftspeedsensor = readspeed;
    //Serial.print(","); Serial.println(readspeed);
    //Serial.print("left"); Serial.println(readspeed);
  }
  //Serial.println(readspeed);
  //Serial.print(leftspeedsensor); Serial.print(","); Serial.println(rightspeedsensor);
}
