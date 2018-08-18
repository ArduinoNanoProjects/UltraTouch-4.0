/**
 * Copyright 2017 (c) by Michael Kaufmann 
 * 
 * Lizenz:
 * ==========================================
 * Diese Software steht unter Lizenz: Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen 
 * Bedingungen 4.0 International CC BY-NC-SA 4.0)
 * Dies ist eine allgemeinverständliche Zusammenfassung der Lizenz (die diese nicht ersetzt). 
 * 
 * Haftungsbeschränkung
 * 
 * Sie dürfen:
 * ----------------------------
 * 
 * - Teilen — das Material in jedwedem Format oder Medium vervielfältigen und weiterverbreiten
 * - Bearbeiten — das Material remixen, verändern und darauf aufbauen
 * 
 * Der Lizenzgeber kann diese Freiheiten nicht widerrufen solange Sie sich an die Lizenzbedingungen halten.
 * 
 * 
 * Unter folgenden Bedingungen:
 * ----------------------------
 * 
 * - Namensnennung — Sie müssen angemessene Urheber- und Rechteangaben machen, einen Link zur Lizenz 
 *                   beifügen und angeben, ob Änderungen vorgenommen wurden. Diese Angaben dürfen in jeder 
 *                   angemessenen Art und Weise gemacht werden, allerdings nicht so, dass der Eindruck entsteht, 
 *                   der Lizenzgeber unterstütze gerade Sie oder Ihre Nutzung besonders.
 *                   
 * - Nicht kommerziell — Sie dürfen das Material nicht für kommerzielle Zwecke nutzen.
 * 
 * - Weitergabe unter gleichen Bedingungen — Wenn Sie das Material remixen, verändern oder anderweitig 
 *                   direkt darauf aufbauen, dürfen Sie Ihre Beiträge nur unter derselben Lizenz wie das Original 
 *                   verbreiten.
 *                   
 * - Keine weiteren Einschränkungen — Sie dürfen keine zusätzlichen Klauseln oder technische Verfahren einsetzen, 
 *                   die anderen rechtlich irgendetwas untersagen, was die Lizenz erlaubt.
 * 
 * Hinweise:
 * ----------------------------
 * Sie müssen sich nicht an diese Lizenz halten hinsichtlich solcher Teile des Materials, die gemeinfrei sind, oder soweit Ihre Nutzungshandlungen durch Ausnahmen und Schranken des Urheberrechts gedeckt sind.
 * Es werden keine Garantien gegeben und auch keine Gewähr geleistet. Die Lizenz verschafft Ihnen möglicherweise nicht alle Erlaubnisse, die Sie für die jeweilige Nutzung brauchen. Es können beispielsweise andere Rechte wie Persönlichkeits- und Datenschutzrechte zu beachten sein, die Ihre Nutzung des Materials entsprechend beschränken.
 * 
 * https://creativecommons.org/licenses/by-nc-sa/4.0/deed.de
 */
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define FPIN                12   // Pin der Signal Pixel
#define NUMPIXELS            6   // Anzahl der NeoPixel

//KopfPixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, FPIN, NEO_GRB + NEO_KHZ800);

/**
 * IR SENSOR
 */
#include <IRremote.h>
int RECV_PIN               = 11; // Pin des IR-Diode
IRrecv irrecv(RECV_PIN);
decode_results results;

/**
 * INCLUDE TASTATUREN
 */
#include "MiniTastatur.h"

//-------------------- Taster --------------------------

const int keyMutePIN       = 2;  // Taster 1 (Mute)
const int keyModusPIN      = 4;  // Taster 2 (Modus)
const int keyMarkerPIN     = 7;  // Taster 3 (Marker)

#define LEDMARKER            5   // NeoPixel für Marker
#define LEDMODUS             6   // NeoPixel für Modus
#define LEDMUTE              4   // NeoPixel für Mute

//-------------------- Variablen -----------------------

//Status Taster
int keyModus              = LOW;  // Status Taster 2 (Modus)
int keyMute               = LOW;  // aktuelles Signal vom Eingangspin
int keyMuteOld            = LOW;  // Backup speicher für Taster 2 (Modus)
int keyChapter            = LOW;  // Status Taster 3 (Shownotes/Chapters)
int keyChapOld            = LOW;  // Backup speicher für Taster 3 (Chapters)
int keyIR                 = 999;

//Blink- und Prellzeiten
int pulse                 = 22;   // Impuls der Blinkzeit (0-10 = nicht sichbar, 11-29 = Impuls, 29 - 500 = Blink)
int prellTime             = 200;  // Zeit für Entprellung
int periode               = 500;  // Zeit für Blinken
unsigned long timer       = 0;    // Zeit beim drücken des Tasters
unsigned long ledMillis   = 0;    // Zeit für den Rythmus der LED

//Modi Values
int modeType              = 0;    // festlegen der verschiedenen Modis
int keyModusValue         = 0;    // festlegen des aktuellen Schalter Modus

//Brücken
int glue;                         // LED Startus der Farbwerte
int rainbowTime           = 0;    // Zeit bis zum Umschalten in neuen Modus
int blinkStatus           = 1;    // Brücke für Blinken
int switchBridge          = 0;    // Brücke für Schalter Modus 3
int specralState          = 1;    // Brücke für Schalter Modus 4

//Status für Mute Schalter
int switcherMute          = 0;
int switcherUnmute        = 0;
int peakBridge            = 1;    // Brücke für Schalter Modus 4

//Helligkeit
int On                    = 30;   // Helligkeit der Signal Pixel (oben)
int coloring              = 6;    // Farbe für Boden (Start mit Farbe 6/grün)

/**
 * Settings
 */
void setup() {
  // Load Serial:
  Serial.begin(9600);
  
  // Taster PINs auf Eingang setzten:
  pinMode(keyMutePIN,  INPUT);    // Taster 1: Mute
  pinMode(keyModusPIN, INPUT);    // Taster 2: Modus
  pinMode(keyMarkerPIN,  INPUT);  // Taster 3: Marker

  // NeoPixel library.
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();   // init NeoPixel
  neoOFF();         // alle NeoPixel aus
  
  //IR LED
  irrecv.enableIRIn();

  delay(100);
}


/**
 * Program
 */
void loop() {
  Modus();
}
