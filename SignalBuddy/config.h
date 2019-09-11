// external trigger settings
externalTrigger = true;            // whether signal can be triggered by pin 2 // otherwise, signal can only be triggered via serial communication
constantSignalDuration = false;    // if true, externally triggered signals obey the current preset duration // if false, signal continues until duration is up or trigger pin goes low, whichever comes first
ledAmplitudeScaling = true;        // whether the LED amplitude should scale with signal amplitude // set to false if using ledPin to drive an LED that serves as a reference in a video, for example


// PRESETS
// add up to 9 signal presets

// preset 0
presets[0].type = SINE;
presets[0].duration = 1000;
presets[0].amplitude = .4;
presets[0].frequency = 5;
presets[0].rampUpTime = 100;
presets[0].rampDownTime = 400;
presets[0].pulseDuration = 0;

// preset 1
presets[1].type = SINE;
presets[1].duration = 500;
presets[1].amplitude = 1;
presets[1].frequency = 20;
presets[1].rampUpTime = 0;
presets[1].rampDownTime = 200;
presets[1].pulseDuration = 0;

// preset 2
presets[2].type = STEP;
presets[2].duration = 200;
presets[2].amplitude = 0.25;
presets[2].rampUpTime = 0;
presets[2].rampDownTime = 0;
presets[2].pulseDuration = 0;

// preset 3
presets[3].type = STEP;
presets[3].duration = 800;
presets[3].amplitude = .7;
presets[3].rampUpTime = 150;
presets[3].rampDownTime = 150;
presets[3].pulseDuration = 50;

// preset 3
presets[4].type = PULSE;
presets[4].duration = 1000;
presets[4].amplitude = .85;
presets[4].frequency = 10;
presets[4].rampUpTime = 0;
presets[4].rampDownTime = 0;
presets[4].pulseDuration = 25;

// preset 3
presets[5].type = PULSE;
presets[5].duration = 600;
presets[5].amplitude = .55;
presets[5].frequency = 15;
presets[5].rampUpTime = 100;
presets[5].rampDownTime = 0;
presets[5].pulseDuration = 30;
