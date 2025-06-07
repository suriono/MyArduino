void GPS_refresh()
{
  float flat, flon, speed_mph;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  int Speed_MPH, Speed_increment;
  //static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
 // print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
//  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
//  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
//  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
//  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
//  print_date(gps);
//  print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
//  print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  
   speed_mph = gps.f_speed_mph();
   print_float(speed_mph, TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
   Speed_MPH = int(speed_mph);
  
 // print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
 // print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
 // print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? TinyGPS::GPS_INVALID_F_ANGLE : TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
 // print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

  if (speed_mph != TinyGPS::GPS_INVALID_F_SPEED) {
     Speed_increment = ((Speed_MPH + 2) / 5) * 5;  // only increment speed by 5 to reduce distraction
     if ((Speed_increment % 10) == 0) { // to replace zero by "O"
        if (Speed_increment > 0) {
           Neopixel_Text( String(Speed_increment/10) + "O");     // increment of 10, non zero
        } else {
           Neopixel_Text("O");                             // zero speed          
        }
     } else {
        Neopixel_Text(String(Speed_increment));
     }
     //Serial.print(", Speed: "); Serial.print(speed_mph);
  } else {                           // blank the screen when no data
     Neopixel_Blank();
  }

 // gps.stats(&chars, &sentences, &failed);
 // print_int(chars, 0xFFFFFFFF, 6);
 // print_int(sentences, 0xFFFFFFFF, 10);
//  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();
  
  smartdelay(1000);
}

// ======================================================================

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    //while (Serial1.available())
    while(Ss.available())
//      gps.encode(Serial1.read());
      gps.encode(Ss.read());
  } while (millis() - start < ms);
}

static float print_float(float val, float invalid, int len, int prec)
{
  float ret_speed = 0.0;
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print("Speed: "); Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
    ret_speed = val;
  }
  smartdelay(0);
  return ret_speed;
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
