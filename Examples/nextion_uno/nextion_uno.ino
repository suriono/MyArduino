#include "NexButton.h"
#include "NexText.h"
//#include "NexNumber.h"
//#include "Arduino.h"
#include "Wire.h"
#include "RTCLib.h"

// Use by Nextion
SoftwareSerial HMISerial(10, 11);//RX,TX Change the nexConfig.h file otherwise Mega is required (Serial2)

/* User Arduino Mega
 *  TX -> RX2, RX -> TX2
 */


NexButton b0 = NexButton(0, 1, "Power Up");
NexButton b1 = NexButton(0, 2, "Power Down");
//NexNumber n0 = NexNumber(0, 3, "n0");
//NexButton b2 = NexButton(0, 0, "b2");
//NexButton b3 = NexButton(0, 3, "b3");
//NexNumber n0 = NexNumber(0,3, "n_power");

NexText t0 = NexText(0, 4, "t0");
/*
NexText t6 = NexText(0, 11, "t6");
NexText t7 = NexText(0, 12, "t7");
NexText t8 = NexText(0, 13, "t8");
NexText t9 = NexText(0, 14, "t9");
*/

//char buffer[10] = {0};
char buffer_temp[10] = {0};
/*
char buffer_hour[10] = {0};
char buffer_week[10] = {0};
char buffer_second[10]  = {0};
char buffer_year[10] = {0};
char buffer_temperature[10] = {0};
char buffer_maxpower[4] = {0};
*/

RTCLib rtc;

//uint8_t number = 0;
//uint16_t sensor_pin = A0;    // select the input pin for the potentiometer
//uint16_t sensor_value = 0;  // variable to store the value coming from the sensor
//uint16_t temperature = 0;
//uint16_t temp = 0;
byte     Last_Power = 30;

NexTouch *nexListenList[] = 
{
    &b0,
    &b1,
    &t0,
    NULL
};



/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
int32_t mynumber;
//int8_t newnumber = 4;
void b0PopCallback(void *ptr)
{
  Serial.println("Power UP pressed");

  Last_Power += 10;
  memset(buffer_temp, 0, sizeof(buffer_temp)); 
  itoa(Last_Power, buffer_temp, 10);
  t0.setText(buffer_temp);
  
  //n0.getValue(&mynumber);
  //Serial.println(mynumber);
  
  /*
  digitalWrite(10, LOW);

    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
        digitalWrite(9, HIGH);
        strcpy(buffer, "ON");
    }
    else
    {
        digitalWrite(9, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
   */
}

/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
void b1PopCallback(void *ptr)
{
  Serial.println("Power Down pressed");
 

  Last_Power -= 10;
  memset(buffer_temp, 0, sizeof(buffer_temp)); 
  itoa(Last_Power, buffer_temp, 10);
  t0.setText(buffer_temp);
  /*
    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
         digitalWrite(10, HIGH);
         strcpy(buffer, "ON");
    }
    else
    {
        digitalWrite(10, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
    */
}
/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
 
void b2PopCallback(void *ptr)
{
  Serial.println("b2 pressed");
  /*
    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
        digitalWrite(11, HIGH);
        strcpy(buffer, "ON");
     
    }
    else
    {
        digitalWrite(11, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
    */
}

/**
 * Button to return the response.
 * 
 * @param ptr - the parameter was transmitted to pop event function pointer. 
 * 
 */
void b3PopCallback(void *ptr)
{
  Serial.println("b3 pressed");
  /*
    NexButton *btn = (NexButton *)ptr;
    memset(buffer, 0, sizeof(buffer));
    btn->getText(buffer, sizeof(buffer));
    if (strcmp(buffer,"ON"))
    {
        digitalWrite(12, HIGH);
        strcpy(buffer, "ON");
    }
    else
    {
        digitalWrite(12, LOW);
        strcpy(buffer, "OFF");
    }
    btn->setText(buffer);
    */
}

/*
void updateTime()
{
    memset(buffer, 0, sizeof(buffer)); 
    number = rtc.hour();
    itoa(number, buffer, 10);
    strcat(buffer, ":");
    number = rtc.minute();
    itoa(number, buffer_temp, 10);
    if (rtc.minute() < 10)
    {
        strcat(buffer, "0");
    }
    strcat(buffer, buffer_temp);
    strcat(buffer, ":");
    if (strcmp(buffer_hour, buffer))
    {
        t6.setText(buffer);
        strcpy(buffer_hour, buffer); 
    }
        
    memset(buffer, 0, sizeof(buffer)); 
    memset(buffer_temp, 0, sizeof(buffer_temp)); 
        
    number = rtc.second();
    itoa(number, buffer_temp, 10);
    if (number < 10)
    {
        strcpy(buffer, "0");
        strcat(buffer, buffer_temp);
        if (strcmp(buffer, buffer_second))
        {
            t9.setText(buffer);
            strcpy(buffer_second, buffer);
         }
    } 
    else
    {
        if (strcmp(buffer_temp, buffer_second))
        {
            t9.setText(buffer_temp);
            strcpy(buffer_second, buffer_temp);
        }
    }
       
// week's data dispaly
 
    switch (rtc.dayOfWeek())
    {
        case 1:
                strcpy(buffer, "Sunday");
                break;
        case 2: 
                strcpy(buffer, "Monday");
                break;
        case 3:
                strcpy(buffer, "Tuesday");
                break;
        case 4:
                strcpy(buffer, "Wednesday");
                break;
        case 5:
                strcpy(buffer, "Thursday");
                break;
        case 6:
                strcpy(buffer, "Friday");
                break;
        case 7:
                strcpy(buffer, "Saturday");
                break;
        default:
                strcpy(buffer, "fail");
                break;
    }

    if (strcmp(buffer_week, buffer))
    {
        t7.setText(buffer);
        strcpy(buffer_week, buffer);
    }

    memset(buffer, 0, sizeof(buffer));
    memset(buffer_temp, 0, sizeof(buffer_temp));
      
//receive  year's data
    number = rtc.year();
    strcat(buffer, "20");
    itoa(number, buffer_temp, 10);
    strcat(buffer, buffer_temp);
    strcat(buffer, "/");
        
//receive  month's data
    number = rtc.month();
    itoa(number, buffer_temp, 10);
    strcat(buffer, buffer_temp);
    strcat(buffer, "/");

//receive  day's data
    number = rtc.day();
    itoa(number, buffer_temp, 10);
    strcat(buffer, buffer_temp);
        
//send to Nextion txt5
    if (strcmp(buffer_year, buffer))
    {
        t5.setText(buffer);
        strcpy(buffer_year, buffer);
    }
}
void ntcTemperature()
{
    sensor_value = analogRead(sensor_pin);
    temp = sensor_value-185;
    temperature = 100-temp/5.4;

    memset(buffer, 0, sizeof(buffer));
    memset(buffer_temp, 0, sizeof(buffer_temp));

    itoa(temperature, buffer, 10);
    
    if (strcmp(buffer, buffer_temperature))
    {
        t8.setText(buffer);
        strcpy(buffer_temperature, buffer);
    }
    delay(100);
    
  
}
*/
void setup(void)
{
    dbSerial.begin(57600);
    nexInit();
    b0.attachPop(b0PopCallback, &b0);
    b1.attachPop(b1PopCallback, &b1);
    //b2.attachPop(b2PopCallback, &b2);
    //b3.attachPop(b3PopCallback, &b3);
    dbSerial.println("setup done");
/*setting port OUTPUT*/
/*
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    */
/*initializing  port */
/*
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    */
    
/* Only used once, then disabled
 *rtc.set(0, 5, 16, 6, 28, 7, 15);
 *RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte
 *dayOfMonth, byte month, byte year);*/

 }

void loop(void)
{
    unsigned long nowtime = millis(); 
    unsigned long update = 10000;
    if (nowtime > update)
    {
        //ntcTemperature();
        update+=10000;
    }
    nexLoop(nexListenList);
    rtc.refresh();
    //updateTime();
    
}

