

void get_Coordinates() {
    //latitude = myGNSS.getHighResLatitude();
    //latitudeHp = myGNSS.getHighResLatitudeHp();
    //longitude = myGNSS.getHighResLongitude();
    //longitudeHp = myGNSS.getHighResLongitudeHp();
    accuracy = myGNSS.getHorizontalAccuracy();
    //int32_t ellipsoid = myGNSS.getElipsoid();
    //int8_t ellipsoidHp = myGNSS.getElipsoidHp();
    //int32_t msl = myGNSS.getMeanSeaLevel();
    //int8_t mslHp = myGNSS.getMeanSeaLevelHp();
    // Assemble the high precision latitude and longitude
    d_lat =  ((double) myGNSS.getHighResLatitude()) / 10000000.0; // Convert latitude from degrees * 10^-7 to degrees
    d_lat += ((double) myGNSS.getHighResLatitudeHp() ) / 1000000000.0; // Now add the high resolution component (degrees * 10^-9 )
    d_lon =  ((double) myGNSS.getHighResLongitude()) / 10000000.0; // Convert longitude from degrees * 10^-7 to degrees
    d_lon += ((double) myGNSS.getHighResLongitudeHp()) / 1000000000.0; // Now add the high resolution component (degrees * 10^-9 )
}
