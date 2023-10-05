// ============= Take ultra sonic distance in cm =================
int Get_Sonic_Distance() {
  for (int nn = 0 ; nn < 10; nn++) {
    int tmp_distance = int(sonar.ping_median(41)/50); // in cm
    if (tmp_distance > 0) {
      return tmp_distance;
    }
    delay(100);
  }
  return 400;   // assuming it's close when it's too far away
}
