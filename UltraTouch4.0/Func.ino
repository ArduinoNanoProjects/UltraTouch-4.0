//------------------------------------------------ MIDI Functions ------------------------------------------------

/**
 * sendMIDICC() 
 * 
 * Infos: CC = Continuous Controller (vordefinierter oder festgelegter Controller)
 * 
 * Status Bit:         1     |     0   1   1    |  1   1   1   1
 * --------------------------------------------------------------
 * Dez                128    |    64  32  16    |  8   4   2   1
 * --------------------------------------------------------------
 *                  Neue Msg |  Control Change  |    Channel
 * 
 * Data Byte 1: Controler Number
 * Data Byte 2: Velue 0/127 (an/aus)
 * 
 */
void sendMidiCC(int channel, int controler, int value) {
  Serial.write(channel);    // Channel: 176 bis 191 (0xB0)
  Serial.write(controler);  // Controler Number: 0-99
  Serial.write(value);      // On/Off: 0/127
}

/**
 * sendMidiNote()
 * 
 * Wird die Taste gedrückt wird der Wert über TasterSave gespeichert
 * und eine Note gesendet.
 * 
 * Ist der Taster nicht gedrückt wird die Note mit der Länge beendet.
 * 
 * MIDI Beispiel:
 * -----------------------------------------------------
 * 
 * (1.) Packet = Status Byte (1 Byte / 8 Bit)
 * 
 * 8 Bit       0        |     0  0   0     | 0    0   0   0
 * ----------------------------------------------------------
 * Potenz     2/7 (-1)  |    2/6 2/5 2/4   | 2/3 2/2 2/1 2/0
 * ----------------------------------------------------------
 * Dez        128       |    64  32  16    | 8    4   2   1
 * ----------------------------------------------------------
 * 
 * Status      1        |     0  0   1     | 0    0   0   0 
 * ----------------------------------------------------------
 *       Neue Nachricht |     Note on      |      Kanal 1
 *
 * Rechnung:  128 + 16
 *            10010000 = 144
 *                    
 * Kanal 01 = 144
 * Kanal 02 = 145
 * Kanal 03 = 146
 * Kanal 04 = 147
 * Kanal 05 = 148
 * Kanal 06 = 149
 * Kanal 07 = 150
 * Kanal 08 = 151
 * Kanal 09 = 152
 * Kanal 10 = 153
 * Kanal 11 = 154
 * Kanal 12 = 155
 * Kanal 13 = 156
 * Kanal 14 = 157
 * Kanal 15 = 158
 * Kanal 16 = 159
 *
 * 
 * (2.) Packet = Data Bit
 * 
 * Beispiel: 0 001 0000
 * 
 * Data   0        |     0  0   1     |     0    0    0    0 
 * --------------------------------------------------------------------
 *       Nachricht |  Notenwert (kk)  | Velocity (vv = Anschlagsstärke)
 * 
 * Notenwerte:
 *   Note(24, 0);   //C1
 *   Note(36, 0);   //C2
 *   Note(48, 0);   //C3 (standard)
 *   Note(60, 0);   //C4 
 *   Note(72, 0);   //C5
 *   Note(84, 0);   //C6
 *   Note(96, 0);   //C7
 *   Note(108, 0);  //C8
 * 
 * (3.) Packet = Data Bit
 * 
 *  Velocity = 0 bis 127
 */
void sendMidiNote(int Channel, int Note, int Velocity) {
  Serial.write(Channel);    // Note on/off, Channel: 1-16
  Serial.write(Note);       // Note: (kk) 1-128
  Serial.write(Velocity);   // Velocity:  (vv) 0-127
}

/**
 * Sende eine Note mit an,aus
 */
void Note(int note){
  sendMidiNote(159, note, 127);  // Note: 1/127
  sendMidiNote(159, note, 0);    // Note: 1/0
}

/**
 * Mute or Unmute
 */
void Speak(String State){
  //Unmute
  if(State == "ON"){
    sendMidiCC(191, 1, 0);      // Unmute: CH 16, 0
  }
  //Mute
  if(State == "OFF"){
    sendMidiCC(191, 1, 127);    // Mute:  CH 16, 127
  }
}


//------------------------------------------------ NeoPixel ------------------------------------------------

/**
 * Schaltet die LED rgb nach sein Farben an oder aus
 * und ermöglicht die Helligkeit festzulegen
 * 
 * neo(255, 255, 255, 1);
 */
void neo(int r, int g, int b, int pixel){
  pixels.setPixelColor(pixel -1, pixels.Color(r,g,b));
  pixels.show();
  delay(1); 
}

/**
 * Schaltet die LED rgb nach sein Farben an oder aus
 * und ermöglicht die Helligkeit festzulegen
 * 
 * neoGlue("red", "long", 1);
 * neoGlue("red", "small", 1);
 * neoGlue("red", "long", 1);
 */
void neoGlue(String type, String mode, int pixel){
  if(mode == "long"){
    glue = 128-127*cos(2*PI/periode*millis());
  } else if (mode == "small"){
    glue = 128-60*cos(2*PI/periode*millis());
  } else {
    glue = 128+127*cos(2*PI/periode*millis());
  }
  if(type == "red"){
    neo(glue, 0, 0, pixel);
  }
  if(type == "green"){
    neo(0, glue, 0, pixel);
  }
  if(type == "blue"){
    neo(0, 0, glue, pixel);
  }
  if(type == "yellow"){
    neo(glue, glue, 0, pixel);
  }
  if(type == "pink"){
    neo(glue, 0, glue, pixel);
  }
  if(type == "white"){
    neo(glue, glue, glue, pixel);
  }
  delay(10); 
}

/**
 * Ausschalten aller NeoPixel
 */
void neoOFF(){
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 
    pixels.show();
    delay(1); 
  }
}

/**
 * LED rgb Farbwechsel mit Impuls
 * 
 * RainbowLED(200);
 */
void rainbowLED(int periode){
  int glue1 = 128+127*cos(2*PI/(periode+2000)*millis());
  int glue2 = 128+127*cos(2*PI/(periode+3500)*(500+millis()));
  int glue3 = 128+127*cos(2*PI/(periode+4000)*(300-millis()));
  pixels.setPixelColor(0, pixels.Color(glue1,glue2,glue3));
  pixels.setPixelColor(1, pixels.Color(glue1,glue2,glue3)); 
  pixels.setPixelColor(2, pixels.Color(glue1,glue2,glue3));
  pixels.setPixelColor(3, pixels.Color(glue1,glue2,glue3));
  pixels.setPixelColor(4, pixels.Color(glue1,glue2,glue3)); 
  pixels.setPixelColor(5, pixels.Color(glue1,glue2,glue3)); 
  pixels.show();
  delay(periode); 
}



/**
 * Bodenfarbe
 * 
 * neoBottom(255, 255, 255);
 */
void neoBottom(int r, int g, int b){
  pixels.setPixelColor(0, pixels.Color(r,g,b));
  pixels.setPixelColor(1, pixels.Color(r,g,b));
  pixels.setPixelColor(2, pixels.Color(r,g,b));
  pixels.show();
}

/**
 * Border Color
 */
void ColorBottom(int color){
  if(color == 1){
    neoBottom(255, 0, 0);     //red
  } else if(color == 2){
    neoBottom(255, 30, 0);    //blut orange
  } else if(color == 3){
    neoBottom(255, 100, 0);   //orange
  } else if(color == 4){
    neoBottom(255, 255, 0);   //yellow
  } else if(color == 5){
    neoBottom(90, 255, 0);    //apple green
  } else if(color == 6){
    neoBottom(0, 255, 0);     //green
  } else if(color == 7){
    neoBottom(0, 255, 50);    //mint
  } else if(color == 8){
    neoBottom(0, 255, 255);   //cyan
  } else if(color == 9){
    neoBottom(0, 0, 255);     //blue
  } else if(color == 10){
    neoBottom(100, 0, 255);   //purple
  } else if(color == 11){
    neoBottom(255, 0, 255);   //pink
  } else {
    neoBottom(255, 255, 255); //white
  }
}
