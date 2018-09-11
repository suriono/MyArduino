
// ***************** get gps **********************************
void get_gps(boolean newData, TinyGPS gps, float *fixlat, float *fixlon) {
  unsigned long chars;
  unsigned short sentences, failed;
  float flat, flon;      // fix lattitue and longitude
  unsigned long fix_age; // returns +- latitude/longitude in degrees
  gps.f_get_position(&flat, &flon, &fix_age);
  *fixlat = flat;
  *fixlon = flon;
  if (fix_age == TinyGPS::GPS_INVALID_AGE)
     Serial.println("No fix detected");
  else if (fix_age > 5000)
     Serial.println("Warning: possible stale data!");

  isGPSfixed = false;  // start with no GPS fixed
  
  if (newData) {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    
    #ifdef DEBUG_GPS
        Serial.print("SAT=");
        //if (gps.satellites() < 10) Serial.print(" ");
        Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
        Serial.print(" PREC=");
        if(gps.hdop() < 100) Serial.print(" ");
        Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
    #endif
    if (flat != TinyGPS::GPS_INVALID_F_ANGLE && flon != TinyGPS::GPS_INVALID_F_ANGLE) {
       #ifdef DEBUG_GPS
          Serial.print(" LAT=");
          Serial.print(flat,7);
          Serial.print(" LON=");
          Serial.print(flon,7);
       #endif
       last_time_gps = millis();
       last_lat = flat;
       last_lon = flon;
       
       if (isFenceOK) Distance_to_Fence();  // get Dist_to_Fence[nn] distance to fences

       isGPSfixed = true;
    }
  }
  
  gps.stats(&chars, &sentences, &failed);
  //Serial.print(" CHARS=");
  //Serial.print(chars);
  //Serial.print(" SENTENCES=");
  //Serial.print(sentences);
  #ifdef DEBUG_GPS
  Serial.print(" CSUM ERR=");
  Serial.print(failed);
  Serial.println("");
  #endif
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}  // *********** end of get_gps *********************



#define R_EARTH  6373000.0    // radius of earth in meters

// ************** Get distance *************************

double get_gps_distance(float flat1, float flon1, float flat2, float flon2) {
   double dlat = (flat1 - flat2)*PI/360.0; // and divided by 2
   double dlon = (flon1 - flon2)*PI/360.0; // and divided by 2
   
   double acord = sin(dlat)*sin(dlat)+cos(flat1*PI/180.0)*cos(flat2*PI/180.0)*sin(dlon)*sin(dlon);
   double ccord = 2.0*atan2(sqrt(acord),sqrt(1.0-acord));
   double distance = R_EARTH * ccord;
          
   //Serial.print(" distance=");
   //Serial.print(distance);
   return distance;
} // *********** end of get_distance **********************
