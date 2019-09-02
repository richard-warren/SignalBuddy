// external trigger settings
externalTrigger = true;            // whether signal can be triggered by pin 2 // otherwise, signal can only be triggered via serial communication
constantSignalDuration = false;    // if true, externally triggered signals obey the current preset duration // if false, signal continues until duration is up or trigger pin goes low, whichever comes first
ledAmplitudeScaling = true;        // whether the LED amplitude should scale with signal amplitude // set to false if using ledPin to drive an LED that serves as a reference in a video, for example


// PRESETS
// add up to 9 signal presets

// preset 0
presets[0].type = SINE;
presets[0].duration = 1000;
presets[0].amplitude = .1;
presets[0].frequency = 5;
presets[0].rampUpTime = 100;
presets[0].rampDownTime = 100;
presets[0].pulseDuration = 0;

// preset 1
presets[1].type = SINE;
presets[1].duration = 5000;
presets[1].amplitude = .5;
presets[1].frequency = 20;
presets[1].rampUpTime = 200;
presets[1].rampDownTime = 200;
presets[1].pulseDuration = 0;

// preset 2
presets[2].type = STEP;
presets[2].duration = 100;
presets[2].amplitude = 1.0;
presets[2].frequency = 0;
presets[2].rampUpTime = 0;
presets[2].rampDownTime = 200;
presets[2].pulseDuration = 0;

// preset 3
presets[3].type = PULSE;
presets[3].duration = 2000;
presets[3].amplitude = .5;
presets[3].frequency = 5;
presets[3].rampUpTime = 0;
presets[3].rampDownTime = 0;
presets[3].pulseDuration = 50;
