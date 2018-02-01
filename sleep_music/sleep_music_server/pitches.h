/*************************************************
 * Public Constants
 *************************************************/

#define NOTE_G2  0x23
#define NOTE_A2  0x25
#define NOTE_B2  0x27
#define NOTE_C3  0x28
#define NOTE_D3  0x2A
#define NOTE_E3  0x2C
#define NOTE_F3  0x2D
#define NOTE_G3  0x2F
#define NOTE_A3  0x31

int melody[] = {NOTE_E3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, 
                NOTE_C3, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_E3, NOTE_D3, NOTE_D3, 0x00,
                NOTE_E3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3,
                NOTE_C3, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_D3, NOTE_C3, NOTE_C3, 0x00     
                };

// ==================== Play music ===================

boolean isPlay_Music = false;
unsigned long last_time_Play_Music;

void Play_Music() {
  int arraysize = sizeof(melody)/sizeof(melody[0]);
  static int note_index;

  note_index = (note_index + 1) % arraysize;

  Serial1.write(0x90);       // command
  Serial1.write(melody[note_index]);    // note
  Serial1.write(0x35);   // velocity

  last_time_Play_Music = millis();
  isPlay_Music = true;
}

// ========== Stop playing ==============
void noNote() {
  Serial1.write(0x90);      // command
  Serial1.write(0x01);      // note
  Serial1.write(0x01);      // velocity
  isPlay_Music = false;
}               
