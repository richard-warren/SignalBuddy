/* SIGNAL BUDDY

  Signal generator for Arduino Uno by Richard Warren.
  Documentation at: https://github.com/rwarren2163/SignalBuddy
  
*/



// initializations
enum signalType {SINE, STEP, PULSE, UNSET};
const String signalNames[] = {"SINE", "STEP", "PULSE"};

#include "Adafruit_MCP4725.h"
#include "presets.h"

const int triggerPin = 2; // pin that triggers stimulation (must be an interrupt pin on Arduino Uno)
const int ledPin = 11; // don't change (can't use PWM pins that rely on timer0, which is hijacked in this script) // PWM output pins that delivers signal modulated the same as DAC, but scaled from 0-5V (use this to power a reference LED that can be captured in videos, for example)

const uint8_t PROGMEM sineLookup[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10, 10, 11, 11, 12, 13, 14, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 35, 36, 37, 38, 39, 40, 41, 43, 44, 45, 46, 47, 49, 50, 51, 53, 54, 55, 56, 58, 59, 61, 62, 63, 65, 66, 67, 69, 70, 72, 73, 75, 76, 78, 79, 81, 82, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 99, 100, 102, 104, 105, 107, 108, 110, 112, 113, 115, 116, 118, 119, 121, 123, 124, 126, 127, 129, 131, 132, 134, 136, 137, 139, 140, 142, 143, 145, 147, 148, 150, 151, 153, 155, 156, 158, 159, 161, 162, 164, 165, 167, 168, 170, 171, 173, 174, 176, 177, 179, 180, 182, 183, 185, 186, 188, 189, 190, 192, 193, 194, 196, 197, 199, 200, 201, 202, 204, 205, 206, 208, 209, 210, 211, 212, 214, 215, 216, 217, 218, 219, 220, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 233, 234, 235, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245, 245, 246, 247, 247, 248, 248, 249, 249, 250, 250, 251, 251, 251, 252, 252, 252, 253, 253, 253, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 254, 254, 253, 253, 253, 252, 252, 252, 251, 251, 251, 250, 250, 249, 249, 248, 248, 247, 247, 246, 245, 245, 244, 244, 243, 242, 241, 241, 240, 239, 238, 238, 237, 236, 235, 234, 233, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223, 222, 220, 219, 218, 217, 216, 215, 214, 212, 211, 210, 209, 208, 206, 205, 204, 202, 201, 200, 199, 197, 196, 194, 193, 192, 190, 189, 188, 186, 185, 183, 182, 180, 179, 177, 176, 174, 173, 171, 170, 168, 167, 165, 164, 162, 161, 159, 158, 156, 155, 153, 151, 150, 148, 147, 145, 143, 142, 140, 139, 137, 136, 134, 132, 131, 129, 128, 126, 124, 123, 121, 119, 118, 116, 115, 113, 112, 110, 108, 107, 105, 104, 102, 100, 99, 97, 96, 94, 93, 91, 90, 88, 87, 85, 84, 82, 81, 79, 78, 76, 75, 73, 72, 70, 69, 67, 66, 65, 63, 62, 61, 59, 58, 56, 55, 54, 53, 51, 50, 49, 47, 46, 45, 44, 43, 41, 40, 39, 38, 37, 36, 35, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 22, 21, 20, 19, 18, 17, 17, 16, 15, 14, 14, 13, 12, 11, 11, 10, 10, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0};
const int sineSmps = sizeof(sineLookup);
volatile uint8_t currentPreset = 0;
volatile float sineIndex = 0;
volatile float bitConversion = 4095 / 255;
volatile bool updated = false; // keeps track of whether the desired value for the ADC has been updated
volatile uint8_t oldValue = 0;
volatile uint8_t newValue = 0;
volatile long signalTimer = 0;  // timer for the duration of the signal
volatile long pulseTimer = 0; // keeps track of interstimulus interval in the pulse condition
volatile bool isSignalOn = false;
volatile char inputChar;
volatile int inputInt;
volatile float inputFloat;
int maxPulseWidth;
bool presetChanged;
String inputString;
char inputBuffer[100];
bool externalTrigger;
bool constantSignalDuration;
bool ledAmplitudeScaling;
Adafruit_MCP4725 dac;
Preset presets[9];
volatile float deltaIndex; // how far to advance in the lookup table for every clock tick
volatile float interPulseInterval;




void setup() {

  // load user configuration
#include "config.h"
  updateFrequency();  // sets global variables that depend on current present stimulus frequency

  // initialize pins
  pinMode(triggerPin, INPUT);
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 0);

  // initialize timer0 interrupt
  cli();//disable interrupts
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  OCR0A = 249; // set compare match register for 2khz increments
  TCCR0A |= (1 << WGM01); // turn on CTC mode
  TCCR0B |= (1 << CS01) | (1 << CS00); // Set CS01 and CS00 bits for 64 prescaler
  TIMSK0 |= (1 << OCIE0A); // enable timer compare interrupt
  sei();//enable interrupts

  // initialize DAC
  TWBR = 12; // speed up i2c communication
  dac.begin(0x62); // begin communication with DAC
  dac.setVoltage(0, false);
  if (externalTrigger) {
    attachInterrupt(digitalPinToInterrupt(triggerPin), signalOnOff, CHANGE);
  }

  // initialize serial communication
  Serial.begin(115200);
  while (!Serial) {}; // wait for serial port to connect
  showMenu();
}



void loop() {

  // update LED output if value has changed
  if (updated) {
    if (ledAmplitudeScaling){
      analogWrite(ledPin, newValue * presets[currentPreset].amplitude);
    }else{
      analogWrite(ledPin, newValue);
    }
    dac.setVoltage(constrain(round(newValue * bitConversion * presets[currentPreset].amplitude), 0, 4095), false);
    updated = false;
  }

  getUserInput();
}
