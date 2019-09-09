#include "FastLED.h"
#include <math.h>
#define LED_PIN     9
#define NUM_LEDS    50
#define BRIGHTNESS  160
#define LED_TYPE    WS2811
#define COLOR_ORDER RGB

CRGB leds[NUM_LEDS];
int bright = 0;

void setup() {
  //int bright = 100;
  // put your setup code here, to run once:
  delay( 1000 ); // power-up safety delay
  pinMode(7,INPUT); //SIG of the Parallax Sound Impact Sensor connected to Digital Pin 7
  FastLED.addLeds<LED_TYPE, LED_PIN>(leds, NUM_LEDS); //Initialize LEDs
  FastLED.setBrightness(BRIGHTNESS); //Set LED Brightness
  FastLED.clear();
  Serial.begin(9600); //Set Baud Rate
}

int current = 0; // Keeps track of how many LEDs are currently on
int hue = 0;    //Value of RGB Hue


/*
 * Display sound on all Sensor Levels
 * Parameters 
 * sound : int32    (Raw sensor value from microphone)
 * 
 * Return: Void
 */
void pulse_all(int sound){

  //Print status of Sound Sensors
  Serial.print("Start");
  Serial.println();
  int colors = analogRead(0);
  Serial.print("Colors: ");
  Serial.print(colors/15);
  colors = colors/15;
  Serial.println();
  Serial.print("Sound: ");
  Serial.print(sound);
  Serial.println();

  //Counts number of loop cycles
  int count = 0;
  
  //Set Maximum and Minimum LED thresholds
  if(sound <= 0 || sound > 40) return;
  while(current != sound && count < 100){
    if(current < sound){ leds[current] = CRGB((128+current*4+(colors))%255,(255-current*5)-(colors),(current*5)+(colors)); current++;}
    else{ leds[current] = CRGB(0,0,0); current--;}
    count++;
    delay(11);
    FastLED.show();
    hue++;
  }
  Serial.print("End");
  Serial.println();
  return;
}



/*
 * Display sound on LEDs only within a certain threshold
 * Parameters 
 * sound : int32    (Raw sensor value from microphone)
 * 
 * Return: Void
 */
 
void pulse_bass(int sound){
  int count = 0;
  if(sound < 10 || sound > 100){    //Only show if sensor value is between 20 and 100
    while(current > 5 && count < 5){
      leds[current] = CRGB(0,0,0); 
      current--;
      FastLED.show();
      delay(12);
      count++;
    }
    return;
  }
  
  Serial.print(sound);
  Serial.println();
  
  while(current != sound && count < 35){  //Increase leds to correct level, but only for so long (35 iterations)
    if(current < sound){ 
      leds[current] = CRGB((128+current*4+(hue%255))%255,(255-current*5)+(hue%255),(current*5)+(hue%255));  //Move leds up and controls RGB based on loudness
      current++;
      }
    else{ leds[current] = CRGB(0,0,0); current--;}  //Move leds down
    count++;

    delay(12);
    FastLED.show();
  }
  count = 0;
  while(current > 15 && count < 5){
      leds[current] = CRGB(0,0,0); 
      current--;
      FastLED.show();
      delay(12);
      count++;
    }
  return;
}

/*
 * Main:
 * Reads analog signals on Arduino Analog Pin
 * and inputs the raw value into the pulse_bass functions
 * to display sound on LED Strip
 */
void loop() {
  
  int sound = analogRead(1);
  
  pulse_bass(sound-25);
  delay(30);
}
