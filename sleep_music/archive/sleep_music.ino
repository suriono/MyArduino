
#include <SoftwareSerial.h>
#include "pitches.h"

SoftwareSerial mySerial(3, 2); // RX, TX

//#define  PIN_MUSIC 3
#define PIN_LED 13

#define DEBUG

#define SAMPLING_FREQ 50    // in Hz 20
#define SAMPLING_MAX  151    // 51sampling to find the last max, typically 2x freq

// notes in the melody:


int melody[] = {NOTE_E3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, 
                NOTE_C3, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_D3, NOTE_D3, 0x00,
                NOTE_E3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3,
                NOTE_C3, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_D3, NOTE_C3, NOTE_C3, 0x00           };


unsigned long tone_last_played;
boolean is_Tone_ON;
  
void setup() {

  // Set MIDI baud rate:
  Serial.begin(9600);
  mySerial.begin(31250);
  
  pinMode(PIN_LED, OUTPUT);
}

#define NUM_INTERVALS 3

// ============== LOOP ====================
void loop() {
  static unsigned long last_time_beat, last_time_play_music;
  static int sig[3], sig_index; 
  static boolean is_Tone_ON = false;
  static float heartbeat;

  unsigned long millis_cur = millis();

  sig_index  = (sig_index + 1) % 3;
  int index0 = (sig_index + 1) % 3;
  int index1 = (sig_index + 2) % 3;
  sig[sig_index] = analogRead(A0);
  //int deriv = sig[sig_index] - sig[index1];
  int deriv2 = 2*sig[sig_index] - sig[index0] - sig[index1];


  //Serial.print(sig[sig_index]); Serial.print(",");
  //Serial.print(deriv2); Serial.print(",");
  Serial.print(sig[sig_index]);
  //Serial.print(heartbeat);
  Serial.println();


  if ( 2*deriv2 > Get_Max(deriv2) )     {
    if ( (millis_cur - last_time_beat) > 500)     { 

      is_Tone_ON = true;

      digitalWrite(PIN_LED, HIGH);
      delay(10);
      Play_Music();
      heartbeat = 60000.0 / (millis_cur - last_time_beat);
      
      last_time_beat = millis_cur;
    }
  } else if ( (millis_cur-tone_last_played) > 300 && is_Tone_ON) { // stop the tone
    // stop the tone playing:
    digitalWrite(PIN_LED, LOW);
    //noNote();
    is_Tone_ON = false;
  }
}   // ============= end of LOOP =============


// ==================== Play music ===================

void Play_Music() {
  int arraysize = sizeof(melody)/sizeof(melody[0]);
  static int note_index;

  note_index = (note_index + 1) % arraysize;

  //mySerial.write(0x90);       // command
  mySerial.write(0x90);       // command
  mySerial.write(melody[note_index]);    // note
  //mySerial.write(0x45);   // velocity
  mySerial.write(0x35);   // velocity
}
void noNote() {
  mySerial.write(0x90);       // command
  mySerial.write(0x01);    // note
  mySerial.write(0x01);   // velocity
}


// ============= get max values to detect pulse peak ======

int Get_Max(int newval) {
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
