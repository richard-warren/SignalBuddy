// t0 interrupt
// every 1 ms monitor signal duration and update signal power
ISR(TIMER0_COMPA_vect){
  
  if (isSignalOn){

    oldValue = newValue;  // record previous signal value to determine whether it has changed

    switch (presets[currentPreset].type){

      case SINE:
        newValue = pgm_read_byte_near(sineLookup + round(sineIndex));
        sineIndex += deltaIndex;
        if (round(sineIndex)>(sineSmps-1)){sineIndex -= sineSmps;}  // wrap around sine index
        break;
      
      case STEP:
        newValue = 255;
        break;

      case PULSE:
        if (pulseTimer<presets[currentPreset].pulseDuration){newValue = 255;}
        else{newValue = 0;}
        pulseTimer++;
        if (pulseTimer>=interPulseInterval){pulseTimer -= interPulseInterval;}
        break;
    }
  

    // ramp up and down
    if (presets[currentPreset].rampUpTime>0){
      newValue = newValue * constrain(float(signalTimer)/presets[currentPreset].rampUpTime, 0, 1); // ramp up
    }
    if (presets[currentPreset].rampDownTime>0){
      newValue = newValue * constrain(float(presets[currentPreset].duration - signalTimer) / presets[currentPreset].rampDownTime, 0, 1); // ramp down
    }


    // check whether light should be turned off
    signalTimer++;
    if (signalTimer>presets[currentPreset].duration){
      isSignalOn = false;
      newValue = 0;
    }

    // record whether value has been updated
    if (oldValue != newValue){updated = true;}
  }
}




// turns on light when pin goes high, and turns off light when pin goes low
void signalOnOff(){
  
  // triggerPin goes HIGH
  if (digitalRead(triggerPin) & !isSignalOn){
      startSignal();
  
  // triggerPin goes LOW
  } else{
    if (signalTimer<(presets[currentPreset].duration-presets[currentPreset].rampDownTime) && !constantSignalDuration & isSignalOn){ // only do this when triggerPin is low
      signalTimer = presets[currentPreset].duration - presets[currentPreset].rampDownTime; // begin ramp down
    }
  }
}




// begins light delivery
void startSignal(){
  signalTimer = 0;
  sineIndex = 0;
  pulseTimer = 0;
  isSignalOn = true;
}




// updates global variables that depend on the frequency of the current preset
void updateFrequency(){
   deltaIndex = sineSmps*presets[currentPreset].frequency/1000; // how much to advance in the sin lookup table every ms
   interPulseInterval = round(1000.0/presets[currentPreset].frequency);
}
