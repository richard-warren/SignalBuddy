// show menu
void showMenu(){
  
  Serial.println(F("\n\nEnter a preset number, (s) to deliver signal, and (m) to print menu..."));
  Serial.println(F("preset | (t)ype  | (d)uration | (a)mplitude | (f)requency | ramp-u(p) | ramp-dow(n) | pulse-(w)idth"));

  for (int i=0; i<9; i++){
    if (presets[i].type != UNSET){
      if (i==currentPreset){
        Serial.print(F("> "));
        Serial.print(i);
        Serial.print(F("   "));
      }else{
        Serial.print(i);
        Serial.print(F("     "));
      }
      presets[i].printPreset(i==currentPreset);
    }
  }
  Serial.println();
}




// get user input
void getUserInput(){
  
  if (Serial.available()){
    
    // get user input
    signalTimer = presets[currentPreset].duration; // this turns off the signal by advancing the timer to the end of the signal
    inputChar = Serial.read();
    Serial.readBytesUntil('\n', inputBuffer, 100);  // throw away all but the first byte, stopping at the newline character
    


    switch (inputChar){

      // deliver signal
      case 's':
        Serial.println(F("Delivering signal..."));
        startSignal();
        break;
      
      
      
      // change signal type
      case 't':
        Serial.println(F("Enter a number for the signal type (0=SINE, 1=STEP, 2=PULSE)..."));
        while (Serial.available() == 0)  {}
        inputInt = Serial.parseInt();
        while (Serial.available()){Serial.read();} // throw away extra bytes
        if (inputInt>=0 && inputInt<3){
          presets[currentPreset].type = static_cast<signalType>(inputInt);
          showMenu();
        }else{
          Serial.println(F("ERROR: must enter an int >=0 and <=2!"));
        }
        break;
      
      
      
      // change signal duration
      case 'd':
        Serial.println(F("Enter signal duration (milliseconds, integer, 1->30000)..."));
        while (Serial.available() == 0)  {}
        inputInt = Serial.parseInt();
        while (Serial.available()){Serial.read();} // throw away extra bytes
        if (inputInt>0 && inputInt<=30000){
          presets[currentPreset].duration = inputInt;
          showMenu();
        }else{
          Serial.println(F("ERROR: must enter an integer >=1 and <=30000!"));
        }
        break;

      
      
      // change signal amplitude
      case 'a':
        Serial.println(F("Enter signal amplitude (fraction of total power, decimal, 0->1)..."));
        while (Serial.available() == 0)  {}
        inputFloat = Serial.parseFloat();
        while (Serial.available()){Serial.read();} // throw away extra bytes
        if (inputFloat>0 && inputFloat<=1.0){
          presets[currentPreset].amplitude = inputFloat;
          showMenu();
        }else{
          Serial.println(F("ERROR: must enter a decimal >=0 and <=1!"));
        }
        break;

      
      
      // change signal frequency
      case 'f':
        Serial.println(F("Enter signal frequency (hz, integer/decimal, 0->500, hz)..."));
        while (Serial.available() == 0)  {}
        inputFloat = Serial.parseFloat();
        while (Serial.available()){Serial.read();} // throw away extra bytes
        if (inputFloat>0 && inputFloat<=500){
          presets[currentPreset].frequency = inputFloat;
          updateFrequency();
          showMenu();
        }else{
          Serial.println(F("ERROR: must be integer/decimal >0 and <=500!"));
        }
        break;

      
      
      // change ramp up duration
      case 'p':
        Serial.println(F("Enter ramp up duration (milliseconds, integer, 0->30000)..."));
        while (Serial.available() == 0)  {}
        inputInt = Serial.parseInt();
        while (Serial.available()){Serial.read();} // throw away extra bytes
        if (inputInt>=0 && inputInt<=presets[currentPreset].duration){
          presets[currentPreset].rampUpTime = inputInt;
          showMenu();
        }else{
          Serial.println(F("ERROR: must enter an integer >=0 and <=30000!"));
        }
        break;

      
      
      // change ramp down duration
      case 'n':
        Serial.println(F("Enter ramp down duration (milliseconds, integer, 0->30000)..."));
        while (Serial.available() == 0)  {}
        inputInt = Serial.parseInt();
        while (Serial.available()){Serial.read();} // throw away extra bytes
        if (inputInt>=0 && inputInt<=presets[currentPreset].duration){
          presets[currentPreset].rampDownTime = inputInt;
          showMenu();
        }else{
          Serial.println(F("ERROR: must enter an integer >=0 and <=30000!"));
        }
        break;

      
      
      // change pulse width
      case 'w':
        if (presets[currentPreset].type==PULSE){
          maxPulseWidth = int(1000.0/presets[currentPreset].frequency);
          Serial.print(F("Enter pulse width (milliseconds, integer, 1->"));
          Serial.print(maxPulseWidth);
          Serial.println("...");
          while (Serial.available() == 0)  {}
          inputInt = Serial.parseInt();
          while (Serial.available()){Serial.read();} // throw away extra bytes
          if (inputInt>=0 && inputInt<(1000.0/presets[currentPreset].frequency)){
            presets[currentPreset].pulseDuration = inputInt;
            showMenu();
          }else{
            Serial.print(F("ERROR: must enter an integer >=0 and <"));
            Serial.print(maxPulseWidth);
            Serial.println("!");
          }  
        }else{
          Serial.println(F("ERROR: pulse width only applies to 'PULSE' signal type!"));
        }
        break;




      // print menu
      case 'm':
        showMenu();

      
      
      default:
        
        // check if preset key was entered
        presetChanged = false;
        inputInt = inputChar - '0';  // convert from char to int
        if (inputInt>=0 && inputInt<9){  // now check if the input was actually an integer
          if (presets[inputInt].type!=UNSET){  // if the number corresponds to a defined preset
            currentPreset = inputInt;
            updateFrequency();
            showMenu();
            presetChanged = true;
          }
        }

        if (!presetChanged && inputChar!='\n'){
          Serial.print(F("Sorry buddy, but '"));
          Serial.print(inputChar);
          Serial.println(F("' is an invalid input!"));
        }
    }
  }
}
