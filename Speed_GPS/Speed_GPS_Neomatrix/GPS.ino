void GPS_refresh()
{
  static float last_flat, last_flon;
  static unsigned long last_speed_limit;
  float flat, flon, speed_mph;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  int Speed_MPH, Speed_increment;
  char coordinate_chain[60];
  
 // print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
//  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 11, 8);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 8);

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
  matrix.fillScreen(0);
  if (speed_mph != TinyGPS::GPS_INVALID_F_SPEED) {
   // Neopixel_Blank();
    Serial.print("Speed: "); Serial.print(String(speed_mph));
    Speed_increment = ((Speed_MPH + 2) / 5) * 5;  // only increment speed by 5 to reduce distraction
    matrix.setTextColor(matrix.Color(0, 255, 0));
    display_Number(Speed_increment,0);
     
    if (flat != TinyGPS::GPS_INVALID_F_ANGLE) {
      //Serial.print(" Lat: "); Serial.print(flat,8);
      if (flon != TinyGPS::GPS_INVALID_F_ANGLE) {
        snprintf (coordinate_chain, sizeof(coordinate_chain), "%f,%f:%f,%f", flat,flon,last_flat,last_flon);
        Serial.print("   "); Serial.print(coordinate_chain);

        if (speed_mph > MIN_SPEED_LIMIT) {
        
          if ( (millis()-last_speed_limit) > PERIOD_SPEED_LIMIT) {           // periodic speed limit request from TomTom
            try_get_Speed_Limit(coordinate_chain);
            if (speedLimit_mph == 0) {          // try again if fails the 1st time
              try_get_Speed_Limit(coordinate_chain);
            }
            last_speed_limit = millis();
          } else if ( speedLimit_mph > 0) {    // previously get speed limit
            matrix.setTextColor(matrix.Color(255, 0, 0));
            display_Number(speedLimit_mph,14); 
          }
        }
          
        last_flat = flat; last_flon = flon;
      }
    }
  } else {                           // blank the screen when no data
    Serial.println("No data");
    Neopixel_Blank();
  }
  Neopixel_Mirror();                 // because it is projected into the windshield
  matrix.show();

 // gps.stats(&chars, &sentences, &failed);
 // print_int(chars, 0xFFFFFFFF, 6);
 // print_int(sentences, 0xFFFFFFFF, 10);
//  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();
  
  smartdelay(1000);
}

// ======================== Get Speed Limit ============================

void try_get_Speed_Limit(char coord[]) {
  speedLimit_mph = get_Speed_Limit(coord);              // request speed limit from TomTom
  Serial.print("===== Final mph: "); Serial.println(speedLimit_mph);
  if (speedLimit_mph > 0) {
     matrix.setTextColor(matrix.Color(255, 0, 0));
     display_Number(speedLimit_mph,14);
  }
}

// ======================== Display Neomatrix number ===================

void display_Number(int num, int offsetx) {
  if ((num % 10) == 0) { // to replace zero by "O" when increment of 10
    if (num > 0) {
      Neopixel_Text( String(num/10) + "O",offsetx);     // increment of 10, zero->O better display of zero
    } else {
      Neopixel_Text("O",offsetx);                                   // zero speed          
    }
  } else {
    Neopixel_Text(String(num),offsetx);
  }
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
   // Serial.print("Speed: "); Serial.print(val, prec);
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
