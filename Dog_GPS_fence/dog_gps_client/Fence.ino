
// ===================== Fence =========================
bool get_Fence() {   // read stored fence coordinates save into memory
   EEPROM.begin(128);
   double normalize;
   int nn = 0;
   byte foundtwice = 0;
   String ptstr;
   while (nn < 128 && foundtwice < 2) {
      char readee = EEPROM.read(nn);
      ptstr += readee;
      if (readee == ']') {
         foundtwice++;
      } else {
         foundtwice = 0;
      }
      nn++;
   }

   if (foundtwice < 2) return false;

   int lastind1;
   int lastind2 = -1;

   Serial.println("Fence coordinates:");
   for (nn=0 ; nn<4 ; nn++) {
      lastind1 = lastind2+2;
      lastind2 = ptstr.indexOf(',', lastind1);
      Fx[nn] = ptstr.substring(lastind1+1, lastind2).toFloat();

      lastind1 = lastind2;
      lastind2 = ptstr.indexOf(']', lastind1);
      Fy[nn] = ptstr.substring(lastind1+1, lastind2).toFloat();

      Serial.print(Fx[nn],5); Serial.println(Fy[nn],5);
   }

   double vx, vy;
   for (nn=0 ; nn<4 ; nn++) {  // four fences vectors
      int mm = (nn+1) % 4;
      vx = Fx[mm] - Fx[nn];
      vy = Fy[mm] - Fy[nn];
      normalize = 1.0/sqrt(vx*vx + vy*vy);
      FvxN[nn] = vx * normalize;
      FvyN[nn] = vy * normalize;
   }

   // Serial.println(ptstr);

   return true;
}

// ================== Send Fence coordinates =======================
void send_Fence() {
  Serial.println("Sending Fence info to the phone...");
  EEPROM.begin(128);
  client.print(",RP ");
  byte nn = 0;
  byte foundtwice = 0;
  while (nn < 128 && foundtwice < 2) {
    char readee = EEPROM.read(nn);
    client.print(readee);
    if (readee == ']') {
      foundtwice++;
    } else {
      foundtwice = 0;
    }
      nn++;
  }

  //Distance_to_Fence(); // for testing
}

// =================== Write EEPROM Fence coordinates ==============
void write_Fence(String datastr) {
  Serial.println("Writing to EEPROM Fence coordinates ... ");
  client.print(",RP OK_FENCE");
        
  EEPROM.begin(128);
  for (byte nn=0; nn < datastr.length(); nn++) {
     EEPROM.put(nn, datastr.charAt(nn));
  }
  EEPROM.end();

  isFenceOK = get_Fence();
}

// ================== Distance to the closest fence =================

void Distance_to_Fence() {

   double px, py;
   double crossproduct, orthogonal_lat, orthogonal_lon;

   double min_dist = 999999.0;

   for (byte nn=0; nn<4 ; nn++) {    // from four walls
      px = last_lat - Fx[nn];        // vector from a fence coordinate
      py = last_lon - Fy[nn];
      //Serial.print(last_lat,5); Serial.print(","); Serial.println(last_lon,5);
      //Serial.print(Fx[nn],5); Serial.print(","); Serial.println(Fy[nn],5);
      //Serial.print(px,7); Serial.print(","); Serial.println(py,7);

      // (+) inside the fence and (-) outside the fence
      crossproduct = px * FvyN[nn] - py * FvxN[nn];

      orthogonal_lat = crossproduct * FvyN[nn];
      orthogonal_lon = -crossproduct * FvxN[nn];

      Dist_to_Fence[nn] =((crossproduct>0)-(crossproduct<0))*get_gps_distance(last_lat, last_lon, last_lat+orthogonal_lat, last_lon+orthogonal_lon);

      if (Dist_to_Fence[nn] < min_dist) min_dist = Dist_to_Fence[nn];
      
      //Serial.print("cross product: "); Serial.print(crossproduct,9); // + = inside the fence
      //Serial.print(", dist: "); Serial.println(Dist_to_Fence[nn]);

      //if (isAutoBuzzer) {
      //   if ( crossproduct < 0) {   // outside the fence
       //     Buzz_Max();
       //  }
      //}
   }
   if (isAutoBuzzer) {
      if (min_dist < 0.0) {
          Buzz_Max();
          Serial.println("Max Buzz");
      } else if (min_dist < BUZZ_MIN_DISTANCE_METER) {
          int buzz_amp = map(int(min_dist*1000.0),0, int(BUZZ_MIN_DISTANCE_METER*1000.0), BUZZ_MAX_AMPLITUDE, 0 );
          //Buzz(300, buzz_amp);
          Buzz_Delay(300, buzz_amp, 200);
          Serial.print("Buzz: "); Serial.println(buzz_amp);
      //} else {
       //   Buzz_Stop();
      }
   //} else {
    //  Buzz_Stop();
   }
}
