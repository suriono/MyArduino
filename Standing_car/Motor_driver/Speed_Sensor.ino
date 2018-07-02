
#define SPEEDSENSOR_POTENTIO_RATIO 0.05



void getSpeedSensor(int rightPotentio) {
  // min() to prevent spike in speed ready
  rightspeedsensor = min(SpeedSensorSerial.parseFloat()*SPEEDSENSOR_POTENTIO_RATIO, abs(rightPotentio)*3/2);

 
}
