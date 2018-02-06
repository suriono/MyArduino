/* NodeMCU pins mapping to Arduino
D0   = 16;    D1   = 5;     D2   = 4;
D3   = 0;     D4   = 2;     D5   = 14;    D6   = 12;
D7   = 13;    D8   = 15;    D9   = 3;     D10  = 1;
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>  


#define AMPLITUDE_RANGE_THRESHOLD 200
#define MAX_SLOPE_THRESHOLD 700
#define SAMPLING_FREQ 50    // in Hz 20
#define SAMPLING_MAX  151    // 51sampling to find the last max, typically 2x freq


#define localUDPPort  2391      // local port to listen for UDP packets
#define REMOTE_IP "192.168.254.94"
//#define REMOTE_IP "192.168.4.2"
#define REMOTE_PORT 2390

WiFiUDP Udp;
#define LED D0

// data type to send to server
#define HEARTBEAT_OUT 3   // send heart beat trigger
#define ANALOG_OUT 1      // send raw pulse signal
#define SLOPE_OUT  2      // send pulse slope data
#define INFO_OUT   0      // send pulse information such as pulse rate, max slope, etc
byte Out_Type;

#define DEBUG

char packetBuffer[UDP_TX_PACKET_MAX_SIZE/128]; // UDP_TX_PACKET_MAX_SIZE = 8192
char readBuffer[UDP_TX_PACKET_MAX_SIZE/128];

// ========================== Setup ===============================

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);   // LED pin as output. 
  digitalWrite(LED, HIGH); 
  
  
  // Starting WiFi AP server
  /*
  Serial.print("Setting soft-AP ... ");
  WiFi.disconnect();
  WiFi.enableAP(true);
  WiFi.setOutputPower(0.3);          // Output WiFi power
  //WiFi.enableSTA(true);
  WiFi.mode(WIFI_AP_STA);
  boolean isAP_ready = WiFi.softAP("sleep_AP", "sleep1234");
  Serial.println(WiFi.softAPIP());
  if(isAP_ready) {
    Serial.println("Access Point Ready");
  } else {
    Serial.println("Access PointFailed!");
  }
  Serial.print("UDP_TX_PACKET_MAX_SIZE = "); Serial.println(UDP_TX_PACKET_MAX_SIZE);
  */

  
   // connect to wifi.
  WiFi.setOutputPower(20.0);          // Output WiFi power
  WiFi.enableAP(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin("philip", "xxxx");
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  

  // Start UDP
  Serial.println("Starting UDP");
  Udp.begin(localUDPPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

 // udp.listen( true );           // and wait for incoming message

}


// ================ get signal slope =============================

int get_Slope(int analogsignal) {

   static int sig[5], sig_index;

   sig_index = (sig_index + 1)%5;

   byte i0 = (sig_index + 1)%5;
   byte i1 = (sig_index + 2)%5;
   byte i2 = (sig_index + 3)%5;
   byte i3 = (sig_index + 4)%5;

   int deriv1 = -sig[sig_index] +  8*sig[i3] - 8*sig[i1] + sig[i0];
   //double deriv2 = -sig[sig_index] + 16.0*sig[i3] -30.0*sig[i2] + 16.0*sig[i1] - sig[i0];
   
   //Serial.print(deriv2); Serial.print(",");
   sig[sig_index] = analogsignal;
   
   return deriv1;
}


// ============= get max values to detect pulse peak ======

int Get_Max_Derivative(int newval) {
  static byte index;
  static int arr[SAMPLING_MAX];
  arr[index] = newval;
  int tmpmax = -10000;
  for (byte nn = 0 ; nn < SAMPLING_MAX ; nn++) {
    if (arr[nn] > tmpmax) {
      tmpmax = arr[nn];
    }
  }
  index = (index + 1) % (SAMPLING_MAX);
  return tmpmax;
}


// ============= get max values to detect pulse peak ======

int Get_Amplitude_Range(int newval) {
  static byte index;
  static int arr[SAMPLING_MAX];
  arr[index] = newval;
  int tmpmax = -10000;
  int tmpmin = 10000;
  for (byte nn = 0 ; nn < SAMPLING_MAX ; nn++) {
    if (arr[nn] > tmpmax) {
      tmpmax = arr[nn];
    }
    if (arr[nn] < tmpmin) {
      tmpmin = arr[nn];
    }
  }
  index = (index + 1) % (SAMPLING_MAX);
  return tmpmax - tmpmin;
}


// ============ smoothing data =====================

int smoothData(int indata, float coef) {
  static float last_smooth;

  last_smooth = coef * float(indata) + (1.0-coef)*last_smooth;
  return round(last_smooth);
}

// ===========================================

void loop() {
  static unsigned long last_time, last_beat_sent, last_info_sent;
  static int last_slope, pulse_interval;

  int packetSize = Udp.parsePacket();
  if (packetSize) {    // reading data type for sending data
    
    Udp.read(readBuffer, packetSize); // NTP_PACKET_SIZE);
    String tmpstr = String(readBuffer);
    if (tmpstr.startsWith("analog")) {
      Out_Type = ANALOG_OUT;
    } else if (tmpstr.startsWith("slope")) {
      Out_Type = SLOPE_OUT;
    } else if (tmpstr.startsWith("heartbeat")) {
      Out_Type = HEARTBEAT_OUT;
    } else if (tmpstr.startsWith("info")) {
      Out_Type = INFO_OUT;
    }
   
  } else if ( (micros() - last_time) > 50000) {  // send packet
    last_time = micros();

    int analogsignal = analogRead(A0);
    int smoothanalog = smoothData(analogsignal, 0.5);
    int analogslope = get_Slope(smoothanalog);
    
    int max_slope = Get_Max_Derivative(analogslope);
    //int amplitude_range = Get_Amplitude_Range(analogsignal);

    #ifdef DEBUG
    if (Out_Type ==  ANALOG_OUT && (WiFi.status() == WL_CONNECTED || WiFi.softAPgetStationNum())) {
      Serial.print(analogsignal); Serial.print(",");
      Serial.print(smoothanalog); Serial.print(",");   
      Serial.print(analogslope/2) ; Serial.print(",");
      Serial.print(max_slope/2) ; Serial.print(",");
    }
    #endif

    // Detecting heart pulse
    boolean detect_beat = false;
    if ((last_slope > 0.5 * max_slope) && analogslope < last_slope && (millis()-last_beat_sent) > 500) {
      if( (millis() - last_beat_sent) < 1500) {
        pulse_interval = millis() - last_beat_sent;
      }
      last_beat_sent = millis();

      #ifdef DEBUG
      if (Out_Type ==  ANALOG_OUT && (WiFi.status() == WL_CONNECTED || WiFi.softAPgetStationNum())) {
        Serial.println(1000); 
      }
      #endif
      detect_beat = true;

    #ifdef DEBUG
    } else if (Out_Type ==  ANALOG_OUT && (WiFi.status() == WL_CONNECTED || WiFi.softAPgetStationNum())) {
      Serial.println(100);
      #endif
    }
    
    

    String tmpstr;
    static boolean isInfoNotSent = true;
    switch(Out_Type) {
      case ANALOG_OUT:
        tmpstr = "data=" + String(analogsignal);
        Send_Data(tmpstr);
        break;
      case SLOPE_OUT:
        tmpstr = "data=" + String(analogslope);
        Send_Data(tmpstr);
        break;
      case HEARTBEAT_OUT:
        if (detect_beat) {
          Send_Data("HeartBeat");
        }
        break;
      //case INFO_OUT:
      default:
      if ( (millis()-last_info_sent) > 830) {
          digitalWrite(LED, HIGH);
          last_info_sent = millis();
          isInfoNotSent = true;
      } else if ( (millis()-last_info_sent) > 800 && isInfoNotSent) {  // send info every second
          tmpstr = "interval=" + String(pulse_interval) + "&maxSlope=" + String(max_slope);
          Send_Data(tmpstr);
          digitalWrite(LED, LOW);
          isInfoNotSent = false;
          
          #ifdef DEBUG
          Serial.println("Send: " + tmpstr);
          #endif
        }             
    }

    last_slope = analogslope;
    
  }

  delay(1);
}

// ================= Send Data ===================

void Send_Data(String indata) {
  
  indata.toCharArray(packetBuffer, indata.length()+2);
  Udp.beginPacket(REMOTE_IP, REMOTE_PORT);
  Udp.write(packetBuffer, indata.length()+2);
  Udp.endPacket();
}







