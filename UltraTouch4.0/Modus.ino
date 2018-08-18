//------------------------------------------- Key Sessions --------------------------------------------------
/**
 * Hier sind die Tastenkürzel einer MiniTastatur oder Fernbedinung festgelegt: 
 */
int KeySession(){
  keyChapter = digitalRead(keyMarkerPIN); // TASTE CAPTERS
  keyMute = digitalRead(keyMutePIN);      // TASTER MUTE/UNMUTE 
  keyModus = digitalRead(keyModusPIN);    // TASTER MODUS

  
  if (irrecv.decode(&results)){
    //Modus OK
    if (results.value == BUTTON_OK){
        keyModus = HIGH;
    }
    //Modi
    if (results.value == BUTTON_1){
        modeType = 1;     //mudus 1
    }
    if (results.value == BUTTON_2){
        modeType = 2;     //mudus 2
    }
    if (results.value == BUTTON_3){
        modeType = 3;     //mudus 3
    }
    if (results.value == BUTTON_4){
        modeType = 4;     //mudus 4
    }
    if (results.value == BUTTON_5){
        modeType = 5;     //mudus 5
    }
    //Color Down (+)
    if (results.value == BUTTON_TOP){
      if(coloring <= 11){
        coloring++;
      } else {
        coloring = 12;
      }
      ColorBottom(coloring);
    }
    //Color Down (-)
    if (results.value == BUTTON_BOTTOM){
      if(coloring >= 0){
        coloring--;
      } else {
        coloring = 1;
      }
      ColorBottom(coloring);
    }
    //Chapter #
    if (results.value == BUTTON_STAR){
      keyChapter = HIGH;
    }
    //Mute *
    if (results.value == BUTTON_HASH){
      keyMute = HIGH;
    }
    
    delay(1);
    irrecv.resume();
    delay(200);
  }
  
  
  //Taster & IR (OK = 1000)
  if (keyModus == HIGH) {
    timer = millis();                     // aktualisiere tasterZeit
    keyModusValue = 1;                    // speichert, dass Taster1 (Mute) gedrückt wurde
    keyModus = LOW;
  }
  if ((millis() - timer > prellTime) && keyModusValue == 1) {
    keyModusValue = 0;                    // setzt gedrückten Taster 1 (Mute) zurück
    modeType++;                           // Modus wird um +1 erhöht
  }
  delay(20);
}



//------------------------------------------------ Modus --------------------------------------------------
/**
 * Hier werden die Funktionen als Modi festgelegt, die die UltrTouch (TM) unterstützt:
 */

void Modus(){
  KeySession();
  
  // MODUS 1 
  // (Mute/Unmute)
  if (modeType == 1) {
    UnmuteToMute();
    Chapters();
  }
  
  // MODUS 2 
  // (Unmute/Mute)
  else if (modeType == 2) {
    MuteToUnmute();
    Chapters();
  }
  
  // MODUS 3 
  // (Schalter: Mute/Unmute)
  else if (modeType == 3) {
    SwitchUnmuteToMute();
    Chapters();
  }
  
  //  MODUS 4 
  // (Schalter: Spectral/Spectral+Peaks/Peaks)
  else if (modeType == 4) {
    SpectralAndPeaks();
    ChapterOpen();
  }

  //  MODUS 5 
  // (MIDI Controler)
  else if (modeType == 5) {
    MoreMidi1();
    MoreMidi2();
  }

  // BACK 
  else {
    Back();
  }
}

//------------------------------------------------ Functions --------------------------------------------------

/**
 * Hier sind die einzelenen Funktionen der Modis festgelegt und können abgeändert werden:
 */

 
/**
 * MODUS 1
 * 
 * Unmute / Mute
 */
void UnmuteToMute() {
    neoGlue("red", "long", LEDMODUS);
    //neo(On, 0, 0, LEDMODUS); //1-grün 

    //NEO PIXEL STATUS
    if (keyMute == HIGH) {
      neo(On, 0, 0, LEDMUTE); //2-rot
    } else {
      neo(0, On, 0, LEDMUTE); //2-grün
    }
        
    //Taster gedrückt: => red
    if (keyMute == HIGH && keyMuteOld == LOW) {
      Speak("OFF"); // an Mute Taste CH 16, 127
      keyMuteOld = keyMute;
    }
    
    //Taster loslassen: => green
    if (keyMute == LOW && keyMuteOld == HIGH) {
      Speak("ON"); // aus unmute Taste CH 16, 127
      keyMuteOld = keyMute;
    }
}

/**
 * MODUS 2 
 * 
 * (Mute/Unmute)
 */
void MuteToUnmute() {
    //neo(0, On, 0, LEDMODUS); //1-grün
    neoGlue("green", "long", LEDMODUS);
    
    //NEO PIXEL STATUS
    if (keyMute == HIGH) {
      neo(0, On, 0, LEDMUTE); //2-grün
    } else {
      neo(On, 0, 0, LEDMUTE); //2-rot
    }
        
    //Taster gedrückt: => red
    if (keyMute == HIGH && keyMuteOld == LOW) {
      Speak("ON");
      keyMuteOld = keyMute;
    }
    
    //Taster loslassen: => green
    if (keyMute == LOW && keyMuteOld == HIGH) {
      Speak("OFF");
      keyMuteOld = keyMute;
    }
}

/**
 * MODUS 3 
 * 
 * (Schalter: Mute/Unmute)
 */
void SwitchUnmuteToMute() {
    //neo(0, 0, On, LEDMODUS);
    neoGlue("blue", "long", LEDMODUS);

    // Taster gedrückt:
    if (keyMute == HIGH && keyMuteOld == LOW) {
       keyMuteOld = keyMute;
       switcherMute = switcherMute + 1; //1, 2
       switcherUnmute = 1;
    }
    // Taster loslassen:
    if (keyMute == LOW && keyMuteOld == HIGH) {
       keyMuteOld = keyMute;
    }
    
    
    // Schalter wurde geschlossen 1 =>
    if(switcherMute == 1 && switcherUnmute == 1){
      Speak("OFF");
      switchBridge = 1;
      switcherMute = 1;
      switcherUnmute = 0;
    }

    // Schalter wurde geschlossen 2 => Mute
    if(switcherMute == 2 && switcherUnmute == 1){
      Speak("ON"); //Sprechen -OFF (Mute)
      
      switchBridge = 0;
      switcherMute = 0;
      switcherUnmute = 0;
    }

    //NEO PIXEL STATUS
    if(switchBridge == 1){
      neoGlue("green", "small", LEDMUTE);
      //neo(0, AN, 0, LEDMUTE); //2-blau
    } 
    else {
      neoGlue("red", "small", LEDMUTE);
      //neo(AN, 0, 0, LEDMUTE); //2-rot
    }
}

/**
 * MODUS 4 
 * 
 * (Schalter: Spectral/Spectral+Peaks/Peaks)
 */
void SpectralAndPeaks() {
    neoGlue("yellow", "long", LEDMODUS);
    
    // Standard:
    // specralState = 1
    // peakBridge   = 1
    
    // 3|specralState = Peaks
    // 1|specralState = SpectralPeaks
    // 2|specralState = Spectral
    
    // Taster gedrückt:
    if (keyMute == HIGH && keyMuteOld == LOW) {
       if(specralState == 3 && peakBridge == 1) { //P (l.blue) 1
          specralState = 1;
          peakBridge = 0;
       }
       if(specralState == 1 && peakBridge == 1){ //SP (pink) 2
          specralState = 2;
          peakBridge = 0;
       } 
       if(specralState == 2 && peakBridge == 1){ //S (yellow) 3
          specralState = 3;
          peakBridge = 0;
       } 
       
       keyMuteOld = keyMute;
    }
    
    // Taster loslassen:
    if (keyMute == LOW && keyMuteOld == HIGH) {
       keyMuteOld = keyMute;
    }
    
    // Schalter wurde geschlossen: Peak
    if(specralState == 1 && peakBridge == 0){
      //Serial.println("MODUS: PEAK");
      Note(1);
 
      specralState = 1;
      peakBridge = 1;
    }
    
    // Schalter wurde geschlossen: SpectralPeaks
    if(specralState == 2 && peakBridge == 0){
      //Serial.println("MODUS: SpectralPeaks");
      Note(2);

      specralState = 2;
      peakBridge = 1;
    }
    
    // Schalter wurde geschlossen: Spectral
    if(specralState == 3 && peakBridge == 0){
      //Serial.println("MODUS: Spectral");
      Note(3);

      specralState = 3;
      peakBridge = 1;
    }

    // Farbdarstellung
    if(specralState == 1) { //P (l.blue) 1 
      neo(0, On, On, LEDMUTE);
    }
    if(specralState == 2){ //SP (pink) 2
      neo(On, 0, On, LEDMUTE);
    } 
    if(specralState == 3){ //S (yellow) 3
      neo(On, On, 0, LEDMUTE);
    }
}

/**
 * Taste 3 / Chapters
 */
void Chapters() {
    // NEO PIXEL STATUS
    if (keyChapter == LOW && keyChapOld == LOW) {
      neo(On, On, On, LEDMARKER);                     //3-grün
    }
        
    // Taster gedrückt: => weiß
    if (keyChapter == HIGH && keyChapOld == LOW) {
      neo(0, On, 0, LEDMARKER);                       //3-weiß
      keyChapOld = keyChapter;
    }
    
    // Taster loslassen: => gelb / Wartezeit
    if (keyChapter == LOW && keyChapOld == HIGH) {
      keyChapOld = keyChapter;
    }
}


/**
 * Taste 3 / Chapters Open
 */
void ChapterOpen() {
    //NEO PIXEL STATUS
    if (keyChapter == LOW) {
      neo(On, On, On, LEDMARKER);     //3-weiß
    }
        
    //Taster gedrückt: => grün
    if (keyChapter == HIGH && keyChapOld == LOW) {
      neo(On-17, 0, On, LEDMARKER);     //3-pink
      keyChapOld = keyChapter;
    }
    
    //Taster loslassen: => weiß
    if (keyChapter == LOW && keyChapOld == HIGH) {
      keyChapOld = keyChapter;
    }
}


/**
 * BACK
 */
void Back(){
  for(int a; a<400;a++){
    rainbowLED(10);
  }
  ColorBottom(coloring);
  
  Note(88);       // Envelope all Tracks: "write"
  Speak("ON");    // Send Unmute: Sprechen ist wieder an
  keyIR = 999;
  modeType = 1;
}


//------------------------------- More MIDI Conroller ------------------------------

/**
 * MODUS 5 (1)
 * 
 * MIDI: Steuerung
 */
void MoreMidi1() {
    neoGlue("pink", "long", LEDMODUS);
    
    // Standard:
    // specralState = 1
    // peakBridge   = 1
    
    // 3|specralState = Peaks
    // 1|specralState = SpectralPeaks
    // 2|specralState = Spectral
    
    // Taster gedrückt:
    if (keyMute == HIGH && keyMuteOld == LOW) {
       if(specralState == 3 && peakBridge == 1) { //P (l.blue) 1
          specralState = 1;
          peakBridge = 0;
       }
       if(specralState == 1 && peakBridge == 1){ //SP (pink) 2
          specralState = 2;
          peakBridge = 0;
       } 
       if(specralState == 2 && peakBridge == 1){ //S (yellow) 3
          specralState = 3;
          peakBridge = 0;
       } 
       
       keyMuteOld = keyMute;
    }
    
    // Taster loslassen:
    if (keyMute == LOW && keyMuteOld == HIGH) {
       keyMuteOld = keyMute;
    }
    
    // Schalter wurde geschlossen: Peak
    if(specralState == 1 && peakBridge == 0){
      //Serial.println("MODUS: PEAK");
      Note(4);
 
      specralState = 1;
      peakBridge = 1;
    }
    
    // Schalter wurde geschlossen: SpectralPeaks
    if(specralState == 2 && peakBridge == 0){
      //Serial.println("MODUS: SpectralPeaks");
      Note(5);

      specralState = 2;
      peakBridge = 1;
    }
    
    // Schalter wurde geschlossen: Spectral
    if(specralState == 3 && peakBridge == 0){
      //Serial.println("MODUS: Spectral");
      Note(6);

      specralState = 3;
      peakBridge = 1;
    }

    // Farbdarstellung
    if(specralState == 1) { //P (l.blue) 1 
      neo(0, On, On, LEDMUTE);
    }
    if(specralState == 2){ //SP (pink) 2
      neo(On, 0, On, LEDMUTE);
    } 
    if(specralState == 3){ //S (yellow) 3
      neo(On, On, 0, LEDMUTE);
    }
}

/**
 * MODUS 5 (2)
 * 
 * MIDI: Steuerung
 */
void MoreMidi2() {
    neoGlue("pink", "long", LEDMODUS);
    
    // Standard:
    // specralState = 1
    // peakBridge   = 1
    
    // 3|specralState = Peaks
    // 1|specralState = SpectralPeaks
    // 2|specralState = Spectral
    
    // Taster gedrückt:
    if (keyMute == HIGH && keyMuteOld == LOW) {
       if(specralState == 3 && peakBridge == 1) { //P (l.blue) 1
          specralState = 1;
          peakBridge = 0;
       }
       if(specralState == 1 && peakBridge == 1){ //SP (pink) 2
          specralState = 2;
          peakBridge = 0;
       } 
       if(specralState == 2 && peakBridge == 1){ //S (yellow) 3
          specralState = 3;
          peakBridge = 0;
       } 
       
       keyMuteOld = keyMute;
    }
    
    // Taster loslassen:
    if (keyMute == LOW && keyMuteOld == HIGH) {
       keyMuteOld = keyMute;
    }
    
    // Schalter wurde geschlossen: Peak
    if(specralState == 1 && peakBridge == 0){
      //Serial.println("MODUS: PEAK");
      Note(4);
 
      specralState = 1;
      peakBridge = 1;
    }
    
    // Schalter wurde geschlossen: SpectralPeaks
    if(specralState == 2 && peakBridge == 0){
      //Serial.println("MODUS: SpectralPeaks");
      Note(5);

      specralState = 2;
      peakBridge = 1;
    }
    
    // Schalter wurde geschlossen: Spectral
    if(specralState == 3 && peakBridge == 0){
      //Serial.println("MODUS: Spectral");
      Note(6);

      specralState = 3;
      peakBridge = 1;
    }

    // Farbdarstellung
    if(specralState == 1) { //P (l.blue) 1 
      neo(0, On, On, LEDMUTE);
    }
    if(specralState == 2){ //SP (pink) 2
      neo(On, 0, On, LEDMUTE);
    } 
    if(specralState == 3){ //S (yellow) 3
      neo(On, On, 0, LEDMUTE);
    }
}

