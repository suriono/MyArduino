
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
     
  if (newData) {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    //Serial.print("LAT=");
    //Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Serial.print(" LON=");
    //Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
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
       last_time = millis();
       last_lat = flat;
       last_lon = flon;
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



/*
// ************** Get distance *************************
double get_distance(float flat1, float flon1, float flat2, float flon2) {
   double dlat = (flat1 - flat2)*PI/360.0; // and divided by 2
   double dlon = (flon1 - flon2)*PI/360.0; // and divided by 2
   
   Serial.print(" dlat="); Serial.print(dlat,8);
   Serial.print(" dlon="); Serial.print(dlon,8);
   
   double acord = sin(dlat)*sin(dlat)+cos(flat1*PI/180.0)*cos(flat2*PI/180.0)*sin(dlon)*sin(dlon);
          //double alat = sin(dlat)*sin(dlat);   // lattitue a
          //double alon = cos(flat*PI/180.0)*cos(firstflat*PI/180.0)*sin(dlon)*sin(dlon); // longitude a
          //double clat = 2.0*atan2(sqrt(alat),sqrt(1.0-alat));
          //double clon = 2.0*atan2(sqrt(alon),sqrt(1.0-alon));
   double ccord = 2.0*atan2(sqrt(acord),sqrt(1.0-acord));
   double distance = R_earth * ccord;
          //double distlat = R_earth * clat;
          //double distlon = R_earth * clon;
          
   Serial.print(" distance=");
   Serial.print(distance);
          //Serial.print(" d_lat=");
          //Serial.print(distlat,6);
          //Serial.print(" d_lat_avg=");
          //Serial.print(Avg_Lat(distlat),6);
          //Serial.print(" d_lon=");
          //Serial.print(distlon,6);
          
       //}
   return distance;

} // *********** end of get_distance **********************
*/
