class Preset {
  
  public:
    
    signalType type = UNSET;
    int duration; // ms (includes ramp up and ramp down times) // also serves as the max signal time when signal is externally triggered
    float amplitude; // fraction of light power
    float frequency; // frequency of sine wave
    int rampUpTime; // ms
    int rampDownTime; // ms
    int pulseDuration; // ms
    
    void printPreset(bool isRowSelected){

      if (isRowSelected){
        seperator = "> ";
      }else{
        seperator = "| ";
      }
      
      Serial.print(seperator);
      printWithLength(signalNames[type], 10);
      Serial.print(seperator);
      printWithLength(String(duration), 11);
      Serial.print(seperator);
      printWithLength(String(amplitude, 2), 12);
      Serial.print(seperator);
      printWithLength(String(frequency, 2), 12);
      Serial.print(seperator);
      printWithLength(String(rampUpTime), 10);
      Serial.print(seperator);
      printWithLength(String(rampDownTime), 12);
      Serial.print(seperator);
      if (type==PULSE){
        printWithLength(String(pulseDuration), 14);
      }else{
          printWithLength("N/A", 14);
      }
      Serial.println();
    }

  private:

    String seperator;  // this string seperates columns in the serial output
    
    // prints text with appended spaces such that total printed statement is of length totalCharacters
    void printWithLength(String text, int totalCharacters){
      Serial.print(text);
      for (int i=0; i<(totalCharacters-text.length()); i++){
        Serial.print(" ");
      }
    }

};
