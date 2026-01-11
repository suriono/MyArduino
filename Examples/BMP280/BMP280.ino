/**************************************************************************
   Tests the getPressure functions
 **************************************************************************/
#include <BMP280.h>
BMP280 bmp280_A(0x76);  // SDO=GND for 0x76 address
BMP280 bmp280_B(0x77);  // SDO=VCC for 0X77 address


void setup()
{
  Serial.begin(57600);
  delay(10);
  Serial.println("BMP280 example");

  Wire.begin(); //Join I2C bus
  bmp280_A.begin();
  bmp280_B.begin();
}

void loop()
{
  //Get pressure value
  uint32_t pressure_A = bmp280_A.getPressure();
  uint32_t pressure_B = bmp280_B.getPressure();
  delay(1000);
  float temperature_A = bmp280_A.getTemperature();
  float temperature_B = bmp280_B.getTemperature();

  //Print the results
  Serial.print("T: ");
  Serial.print(temperature_A); Serial.print(",");Serial.print(temperature_B);
  Serial.print(" °C \t");
  Serial.print("Pressure: ");
  Serial.print(pressure_A); Serial.print(",");Serial.print(pressure_B);
  Serial.print(" Pa diff:"); Serial.println(pressure_A-pressure_B);

`  delay(1000);
}
