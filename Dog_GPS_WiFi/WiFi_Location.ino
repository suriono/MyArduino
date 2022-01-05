// Set time via NTP, as required for x.509 validation
void setClock () {
    configTime (0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.print ("Waiting for NTP time sync: ");
    time_t now = time (nullptr);
    while (now < 8 * 3600 * 2) {
        delay (500);
        Serial.print (".");
        now = time (nullptr);
    }
    struct tm timeinfo;
    gmtime_r (&now, &timeinfo);
    Serial.print ("\n");
    Serial.print ("Current time: ");
    Serial.print (asctime (&timeinfo));
}


void setWiFiLocation() {
   setClock ();

   Serial.println("Waiting for Google GeoLocation .... ");
   WifiLocation location (googleApiKey); // API key is hidden for security
 
   location_t loc = location.getGeoFromWiFi();

   Lattitude = loc.lat;
   Longitude = loc.lon;
   Accuracy  = loc.accuracy;
   Response  = location.wlStatusStr (location.getStatus ());

   // Serial.println("Location request data");
   Serial.println(location.getSurroundingWiFiJson()+"\n");
   Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
    
   Serial.println ("Accuracy: " + String (loc.accuracy));
   Serial.println ("Result: " + Response);
}
