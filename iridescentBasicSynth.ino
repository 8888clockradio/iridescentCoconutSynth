/*
        Copyright 2020 George C. Rosar II licensed under:
                 Apache License
           Version 2.0, January 2004
        http://www.apache.org/licenses/
        
        Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

iridescentBasicSynth::iridescentBasicSynth(AudioMixer4 *l, AudioMixer4 *r, Bounce *btn0, Bounce *btn1, Bounce *btn2, int rPin, int gPin, int bPin, int lPin, int lPin2, bool *instrumentBool) {

  left = l;
  right = r;

  mainFilebypassInstrumentMode = instrumentBool;

  fm1l1 = new AudioConnection(filterHeaven1, 0, *left, 0);
  fm1r1 = new AudioConnection(filterHeaven1, 0, *right, 0);
  
  redPin = rPin;
  greenPin = gPin;
  bluePin = bPin;
  ledPin = lPin;
  ledPin2 = lPin2;



  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
    
  setColorIn(255, 0, 0);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPin2, LOW);

  button0 = *btn0;
  button1 = *btn1;  // 15 = 15 ms debounce time
  button2 = *btn2;

  millisSeconds = millis();
  
  /////
  //AudioNoInterrupts();
  ///////////
  
  for (int i = 0; i < TOTAL_VOICES; ++i) {
    waveformMod[i].mod.phaseModulation((float) 0.33 * rad * i);
    waveformMod[i].mod.amplitude(1);
    waveformMod[i].mod.begin(waveform_type);
    waveformMod[i].mod.arbitraryWaveform(SawDerange, 12543.0);
    //waveformMod[i].mod.frequencyModulation(3.5);
    envelopeMod[i].attack(4.2);
    envelopeMod[i].hold(6.1);
    envelopeMod[i].decay(0.4);
    envelopeMod[i].sustain(0.3);
    envelopeMod[i].release(0.2);
    voices[i].waveformMod_id = i;
    voices[i].channel = voices[i].note = 0xFF;
  }
  
  sineForm.amplitude(1.0);
  sineForm.frequency(0.05);
  sineForm.phase(0.85 * rad * 1);
  sineFM.phase(0.67 * rad * 1);
  sineFM.amplitude(1.0);
  //sineFM.phase(0.67 * rad * 9000.0);
  sineFM.frequency(4.5);

  for (int i = 0; i < TOTAL_MIXERS - 1; ++i)
    for (int j = 0; j < 4; ++j)
      mixer[i].gain(j, 0.98);
  for (int i = 0; i < 4; ++i)
    mixer[TOTAL_MIXERS - 1].gain(i, i < SECONDARY_MIXERS ? 1.0 / SECONDARY_MIXERS : 0.0);
    
  filterHeaven1.gain(0, 0.0);       //sets filters off
  filterHeaven1.gain(1, 0.0);
  filterHeaven1.gain(2, 0.0);
  filterHeaven1.gain(3, 1.0);

  sine1.amplitude(1.0);
  sine1.frequency(50);

  filter1.octaveControl(6);
  
  /////
  //AudioInterrupts();
  ///////////
  
}

void iridescentBasicSynth::myControlChange(byte channel, byte control, byte value) {
  #ifdef DEBUG_ALLOC
  Serial.print("Control Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", control=");
  Serial.print(control, DEC);
  Serial.print(", value=");
  Serial.println(value, DEC);
  #endif //DEBUG_ALLOC   


  
  /////
  //AudioNoInterrupts();
  ///////////
  
  /////
  //mod wheel
  if ((int) control == 1)
  {
    modulateValue = (float) value;
  }
  /////
  
  /////
  //x-axis studio logic
  if ((int) control == 74)
  {
    xValue = (float) value;
    if (bypass)
    { 
      filter1.frequency((float) map((float) value, 0.0, 127.0, 50.0, 5000.0));
    }
    if (lfoAd == 0) { //Blue after Red
      eHoldDelay = map((float) value, 0, 127, 0, 20); //200 GREEN
    }
    if (lfoAd == 1) { //Green
      eHoldHold = map((float) value, 0, 127, 0, 11880); // 11880 BLUE
    }
    if (lfoAd == 2) { //Purple
      eHoldSustain = map((float) value, 0, 127, 0.0, 1.0); //0.0 to 1.0 PURPLE
    }
    for (int i = 0; i < TOTAL_VOICES; ++i) {
      waveformMod[i].mod.phaseModulation(map((float) value, 0, 127, 30, 9000));
    }
#ifdef DEBUG_ALLOC  
    Serial.println("frequency");
#endif
    sineFM.frequency((float) map((float) value, 0.0, 127.0,  0.05, 20.9));
    sineForm.frequency((float) map((float) value, 0.0, 127.0, 26.0, 0.05));
  }
  /////

  /////
  //y-axis studio logic
  if ((int) control == 71)
  {
    yValue = (float) value;
    if (bypass)
    {
      //sine2.frequency(map((float) value, 0, 127, 50.0, 5000.0)); //add something here later
      filter1.resonance((float) map((float) value, 0, 127, 0.7, 5.0));
    }
    if (lfoAd == 0) { //GREEN
      eHoldAttack = map((float) value, 0, 127, 0, 1000); // 11880 GREEN
    }
    if (lfoAd == 1) { //BLUE
      eHoldDecay = map((float) value, 0, 127, 0, 500); //11880 BLUE
    }
    if (lfoAd == 2) { //PURPLE
      eHoldRelease = map((float) value, 0, 127, 0, 3800); //11880 PURPLE
    }
    for (int i = 0; i < TOTAL_VOICES; ++i) {
      waveformMod[i].mod.frequencyModulation(map((float) value, 0, 127, 0.1, 2.5));
    }
    sineForm.frequency(map((float) value, 0, 127, 0.05, 200.0));
    sineFM.frequency(map((float) value, 0, 127, 82.41, 8.18));
  }
  
  for (int i = 0; i < TOTAL_VOICES; ++i) {
    if ((envelopeMod[i].isActive()) && waveformMod[i].isPlaying) {  
      envelopeMod[i].delay(eHoldDelay);
      envelopeMod[i].attack(eHoldAttack);
      envelopeMod[i].hold(eHoldHold);
      envelopeMod[i].decay(eHoldDecay);
      envelopeMod[i].sustain(eHoldSustain);
      envelopeMod[i].release(eHoldRelease);            
    }
  }
  /////
  //AudioInterrupts();
  ///////////

}

void iridescentBasicSynth::myNoteOn(byte channel, byte note, byte velocity) {
  // When using MIDIx4 or MIDIx16, usbMIDI.getCable() can be used
  // to read which of the virtual MIDI cables received this message.

  /////
  //AudioNoInterrupts();
  ///////////

  #ifdef DEBUG_ALLOC
  Serial.print("Note On, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);
  #endif //DEBUG_ALLOC  
  if (channel == 2) {
    if (note == 35) { //channel 2 B1
      if (instrumentSwitch)
      {
        instrumentSwitch = false;
        *mainFilebypassInstrumentMode = false;
        Serial.println("instrumentSwitch = false");
      }
      else {
        instrumentSwitch = true;
        *mainFilebypassInstrumentMode = true;
        Serial.println("instrumentSwitch = true");
      }
    }
    if (note == 36) { //channel 2 C1
      //#ifdef DEBUG_ALLOC
      Serial.println("button0Trig - true");
      //#endif //DEBUG_ALLOC
      button0Trig = true;
    }
    if (note == 38) { //channel 2 D1
      //#ifdef DEBUG_ALLOC
      Serial.println("button1Trig - true");
      //#endif //DEBUG_ALLOC
      button1Trig = true;
    }
    if (note == 40) { //channel 2 E1
      //#ifdef DEBUG_ALLOC
      Serial.println("button2Trig - true");
      //#endif //DEBUG_ALLOC
      button2Trig = true;
    }
    if (note == 41) { //channel 2 F1
      if (filter == 0) {
        filter = 1;
        //#ifdef DEBUG_ALLOC
        Serial.println("Band Pass selected");
        //#endif //DEBUG_ALLOC 
      }
      else if (filter == 1) {
        filter = 2;
        //#ifdef DEBUG_ALLOC
        Serial.println("High Pass selected");
        //#endif //DEBUG_ALLOC 
      }
      else if (filter == 2) {
        filter = 0;
        //#ifdef DEBUG_ALLOC
        Serial.println("Low Pass selected");
        //#endif //DEBUG_ALLOC
      }
    }
    if (note == 43) { //channel 2 G1
      if (pwmBypass) {
        //#ifdef DEBUG_ALLOC
        Serial.println("pwmBypass off");
        //#endif //DEBUG_ALLOC
        pwmBypass = false;
      }
      else {
        //#ifdef DEBUG_ALLOC
        Serial.println("pwmBypass on");
        //#endif //DEBUG_ALLOC
        pwmBypass = true;
      }
    }
  }
  else
  {
    notes_played++;
  #ifdef DEBUG_ALLOC
    Serial.printf("**** NoteOn: channel==%hhu,note==%hhu ****\n", channel, note);
    printVoices();
  #endif //DEBUG_ALLOC
    freeVoices();
    int waveformMod_id = allocateVoice(channel, note);
    waveformMod[waveformMod_id].isPlaying = true;
    float val = (float) map((float) velocity, 0.0, 127.0, 0.0, 1.0);
    waveformMod[waveformMod_id].mod.frequency((float) midiNotes[((int) note)]);
    waveformMod[waveformMod_id].freqNote = (float) midiNotes[((int) note)];
    waveformMod[waveformMod_id].midiNote = (int) note;
    waveformMod[waveformMod_id].mod.amplitude(val);
    envelopeMod[waveformMod_id].noteOn();
  #ifdef DEBUG_ALLOC
    printVoices();
  #endif //DEBUG_ALLOC
  }
  /////
  //AudioInterrupts();
  ///////////
  
}

void iridescentBasicSynth::myNoteOff(byte channel, byte note, byte velocity) {
  #ifdef DEBUG_ALLOC
  Serial.print("Note Off, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);
  Serial.printf("\n**** NoteOff: channel==%hhu,note==%hhu ****", channel, note);
  printVoices();
  #endif //DEBUG_ALLOC
  int waveformMod_id = findVoice(channel, note);
  if (waveformMod_id != TOTAL_VOICES) {
    envelopeMod[waveformMod_id].noteOff();
  }
  #ifdef DEBUG_ALLOC
  printVoices();
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::updateSynth() {
  button0.update();
  button1.update();
  button2.update();
  
  //#ifdef DEBUG_ALLOC
  //unsigned long theMillis = millisSeconds + 5000;
  if (millis() > ((unsigned long) millisSeconds + 5000)) {
  //if (millis() > theMillis) {
    millisSeconds = millis();
    Serial.print("Audio Processor Usage: "); Serial.print(AudioProcessorUsage()); Serial.print('/'); 
    Serial.print(AudioProcessorUsageMax()); Serial.print(" ");  Serial.print(AudioProcessorUsage() / AudioProcessorUsageMax()); Serial.println('%');
    Serial.printf("MemUsage:%i\n", AudioMemoryUsageMax());
  }
  //#endif //DEBUG_ALLOC
  
  /////
  //AudioNoInterrupts();
  ///////////
  
  //clear release times
  for (int i = 0; i < TOTAL_VOICES; ++i) {
    if ((!envelopeMod[i].isActive()) && waveformMod[i].isPlaying) {
      waveformMod[i].mod.amplitude(0.0);
      waveformMod[i].isPlaying = false;
      waveformMod[i].freqNote = 0.0;
      waveformMod[i].midiNote = 0;
    }
  }
  
  /////
  //map to good ratio
  float a1a1Aa = 2.0;
  float a1a1Ba = 2.0;
  /////

    
  float modWheel = (float) map((float) modulateValue, 0.0, 127.0, 0.0, 1.0);
  sineForm.amplitude(modWheel);  
  sineFM.amplitude(modWheel);
  a1a1Aa = (float) map((float) xValue, 0.0, 127.0, 2.0, 100.0);
  //a1a1Ab = (float) map(yValue, 0.0, 127.0, 0.0, 1.0);

  a1a1Ba = (float) map((float) yValue, 0.0, 127.0, 0.05, 80.33);
  //a1a1Bb = (float) map(yValue, 0.0, 127.0, 0.0, 1.0);
  
  //start here  
  if (bypass) {                   //filter on
    
    /////
    //filter modulation
    //filter1.resonance(res);
    /////

    if (filter == 0) {
      filterHeaven1.gain(0, 1.0); //low pass
      filterHeaven1.gain(1, 0.0); //band pass note octave range
      filterHeaven1.gain(2, 0.0); //high pass
      filterHeaven1.gain(3, 1.0); //dry on
    }
    if (filter == 1) {
      filterHeaven1.gain(0, 0.0); //low pass
      filterHeaven1.gain(1, 1.0); //band pass note octave range
      filterHeaven1.gain(2, 0.0); //high pass
      filterHeaven1.gain(3, 1.0); //dry on
    }
    if (filter == 2) {
      filterHeaven1.gain(0, 0.0); //low pass
      filterHeaven1.gain(1, 0.0); //band pass note octave range
      filterHeaven1.gain(2, 1.0); //high pass
      filterHeaven1.gain(3, 1.0); //dry on
    }
    
    if (pwmBypass) {              //pwm on
      pwmMixer1.gain(1, 0.0);     //1 is dry audio
      pwmMixer1.gain(0, 1.0);     //0 is multiply + pwm
      
      /////
      //pulsewidth modulation
      pwm1.frequency(a1a1Aa);
      pwm1.amplitude(modWheel);
      /////
    
      /////
      //filter modulation
      //filter1.resonance(res);
      //filter1.octaveControl(octaves);
      /////
      
    }
    else {                        //pwm off
      pwmMixer1.gain(1, 1.0);     //1 is dry audio
      pwmMixer1.gain(0, 0.0);     //0 is multiply + pwm
    }
  }
  else {
    
      /////
      //turn off filter
      filterHeaven1.gain(0, 0.0); //lowpass
      filterHeaven1.gain(1, 0.0); //bandpass
      filterHeaven1.gain(2, 0.0); //highpass
      filterHeaven1.gain(3, 1.0); //from pwmMixer       
      /////                       

    if (pwmBypass) {              //pwm on
      pwmMixer1.gain(1, 0.0);     //1 is dry audio
      pwmMixer1.gain(0, 1.0);     //0 is multiply + pwm
  
      /////
      //pulsewidth modulation
      pwm1.frequency(a1a1Aa);
      pwm1.amplitude(modWheel);
      /////
    
    }
    else {                      //pwm off – just dry
      pwmMixer1.gain(1, 1.0); //1 is dry audio
      pwmMixer1.gain(0, 0.0); //0 is multiply + pwm
    }
  }


  if (used_voices == 0)
  {
    
    /////
    //quiet the pulse width
    pwm1.amplitude(0);
    /////
    
  }
  
  /////
  //AudioInterrupts();
  ///////////

  if (button0.fallingEdge() || button0Trig) {
    //#ifdef DEBUG_ALLOC    
    Serial.print("Control waveform: ");
    //#endif //DEBUG_ALLOC 
    if (waveform_type == WAVEFORM_SAMPLE_HOLD) {
      waveform_type = WAVEFORM_SINE;
      digitalWrite(ledPin2, LOW);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_SINE");
      //#endif //DEBUG_ALLOC 
    } else if (waveform_type == WAVEFORM_SINE) {
      waveform_type = WAVEFORM_SQUARE;  //original value
      //waveform_type = WAVEFORM_TRIANGLE;
      digitalWrite(ledPin2, LOW);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_SQUARE");
      //#endif //DEBUG_ALLOC 
    } else if (waveform_type == WAVEFORM_SQUARE) {
      waveform_type = WAVEFORM_TRIANGLE;
      digitalWrite(ledPin2, LOW);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_TRIANGLE");
      //#endif //DEBUG_ALLOC 
    } else if (waveform_type == WAVEFORM_TRIANGLE) {
      waveform_type = WAVEFORM_PULSE;   //original value
      digitalWrite(ledPin2, HIGH);
      //waveform_type = WAVEFORM_SAWTOOTH;
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_PULSE");
      //#endif //DEBUG_ALLOC 
    } else if (waveform_type == WAVEFORM_PULSE) {
      waveform_type = WAVEFORM_SAWTOOTH;
      digitalWrite(ledPin2, LOW);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_SAWTOOTH");
      //#endif //DEBUG_ALLOC
    } else if (waveform_type == WAVEFORM_SAWTOOTH) {
      waveform_type = WAVEFORM_SAWTOOTH_REVERSE;
      digitalWrite(ledPin2, LOW);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_SAWTOOTH_REVERSE");
      //#endif //DEBUG_ALLOC
    } else if (waveform_type == WAVEFORM_SAWTOOTH_REVERSE) {
      waveform_type = WAVEFORM_TRIANGLE_VARIABLE;   //original value
      //waveform_type = WAVEFORM_ARBITRARY;
      digitalWrite(ledPin2, HIGH);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_TRIANGLE_VARIABLE");
      //#endif //DEBUG_ALLOC
    } else if (waveform_type == WAVEFORM_TRIANGLE_VARIABLE) {
      waveform_type = WAVEFORM_ARBITRARY;
      digitalWrite(ledPin2, LOW);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_ARBITRARY");
      //#endif //DEBUG_ALLOC
    } else if (waveform_type == WAVEFORM_ARBITRARY) {
      waveform_type = WAVEFORM_SAMPLE_HOLD;
      digitalWrite(ledPin2, LOW);
      //#ifdef DEBUG_ALLOC
      Serial.println("WAVEFORM_SAMPLE_HOLD");
      //#endif //DEBUG_ALLOC
    }
    //WAVEFORM_SAWTOOTH_REVERSE, WAVEFORM_TRIANGLE_VARIABLE, WAVEFORM_ARBITRARY, WAVEFORM_SAMPLE_HOLD

    /////
    //AudioNoInterrupts();
    ///////////
    
    for (int i = 0; i < TOTAL_VOICES; i++) {
      waveformMod[i].mod.begin(waveform_type);
    }

    /////
    //AudioInterrupts();
    ///////////
    
    button0Trig = false;
  }

  if (button1.fallingEdge() || button1Trig) {
    if (!bypass) {
      bypass = true;
      if (filter == 0) {
        filterHeaven1.gain(0, 1.0); //low pass
        filterHeaven1.gain(1, 0.0); //band pass note octave range
        filterHeaven1.gain(2, 0.0); //high pass
        filterHeaven1.gain(3, 1.0); //dry on
      }
      if (filter == 1) {
        filterHeaven1.gain(0, 0.0); //low pass
        filterHeaven1.gain(1, 1.0); //band pass note octave range
        filterHeaven1.gain(2, 0.0); //high pass
        filterHeaven1.gain(3, 1.0); //dry on
      }
      if (filter == 2) {
        filterHeaven1.gain(0, 0.0); //low pass
        filterHeaven1.gain(1, 0.0); //band pass note octave range
        filterHeaven1.gain(2, 1.0); //high pass
        filterHeaven1.gain(3, 1.0); //dry on
      }

      
      #ifdef DEBUG_ALLOC
      Serial.println("Bypass turned on - WHITE MODE");
      #endif //DEBUG_ALLOC
      digitalWrite(ledPin, HIGH);
    }
    else if (bypass) {
      #ifdef DEBUG_ALLOC
      Serial.println("Bypass turned off - WHITE MODE");
      #endif //DEBUG_ALLOC
      bypass = false;
      filterHeaven1.gain(0, 0.0);
      filterHeaven1.gain(1, 0.0);
      filterHeaven1.gain(2, 0.0);
      filterHeaven1.gain(3, 1.0);

      digitalWrite(ledPin, LOW);
    }
    /*else {
      if (bypass)
      {
        #ifdef DEBUG_ALLOC
        Serial.println("Bypass turned off - REGULAR MODE");
        #endif //DEBUG_ALLOC
        bypass = false;
        filterHeaven1.gain(0, 0.0);
        filterHeaven1.gain(1, 0.0);
        filterHeaven1.gain(2, 0.0);
        filterHeaven1.gain(3, 1.0);
        
        digitalWrite(ledPin, LOW);
      }
      else {
        #ifdef DEBUG_ALLOC
        Serial.println("Bypass turned on - REGULAR MODE");
        #endif //DEBUG_ALLOC
        bypass = true;
        if (filter == 0) {
          filterHeaven1.gain(0, 1.0); //low pass
          filterHeaven1.gain(1, 0.0); //band pass note octave range
          filterHeaven1.gain(2, 0.0); //high pass
          filterHeaven1.gain(3, 1.0); //dry on
        }
        if (filter == 1) {
          filterHeaven1.gain(0, 0.0); //low pass
          filterHeaven1.gain(1, 1.0); //band pass note octave range
          filterHeaven1.gain(2, 0.0); //high pass
          filterHeaven1.gain(3, 1.0); //dry on
        }
        if (filter == 2) {
          filterHeaven1.gain(0, 0.0); //low pass
          filterHeaven1.gain(1, 0.0); //band pass note octave range
          filterHeaven1.gain(2, 1.0); //high pass
          filterHeaven1.gain(3, 1.0); //dry on
        }
        
        digitalWrite(ledPin, HIGH);
      }
    }*/
    button1Trig = false;
  }

  if (button2.fallingEdge() || button2Trig) {
    if (lfoAd == 0) {
      //greenValue
      #ifdef DEBUG_ALLOC
      Serial.println("greenValue");
      Serial.println("lfo-1");
      #endif //DEBUG_ALLOC     
      lfoAd = 1;  //actual value
      //instrumentSwitch = false;
      setColorIn(0, 255, 0);
    }
    else if (lfoAd == 1) {
      //blue
      #ifdef DEBUG_ALLOC
      Serial.println("blueValue");
      Serial.println("lfo-2");
      #endif //DEBUG_ALLOC      
      lfoAd = 2;
      //instrumentSwitch = false;
      setColorIn(0, 0, 255);
    }
    else if (lfoAd == 2) {
      //purple
      #ifdef DEBUG_ALLOC
      Serial.println("purpleValue");
      Serial.println("lfo-3");
      #endif //DEBUG_ALLOC         
      lfoAd = 3;
      //instrumentSwitch = false;
      setColorIn(255, 255, 0);
    }
    else if (lfoAd == 3) {
      //yellow
      #ifdef DEBUG_ALLOC
      Serial.println("yellowValue");
      Serial.println("lfo-4");
      #endif //DEBUG_ALLOC        
      lfoAd = 4;
      #ifdef DEBUG_ALLOC      
      Serial.println("instrument switch on");
      #endif //DEBUG_ALLOC       
      //instrumentSwitch = false;
      setColorIn(255, 0, 255);
    }
    else if (lfoAd == 4) {
      //white
      #ifdef DEBUG_ALLOC
      Serial.println("whiteValue");
      Serial.println("lfo-5");
      #endif //DEBUG_ALLOC         
      lfoAd = 5;
      #ifdef DEBUG_ALLOC
      Serial.println("instrument switch on");
      #endif //DEBUG_ALLOC 
      //instrumentSwitch = true;
      //add line-in to synth or usb in from computer to synth

      setColorIn(255, 255, 255);
    }
    else if (lfoAd == 5) {
      //red
      #ifdef DEBUG_ALLOC
      Serial.println("redValue");
      Serial.println("lfo-0");
      #endif //DEBUG_ALLOC       
      lfoAd = 0;
      //instrumentSwitch = false;
      setColorIn(255, 0, 0);
    }
    else {
      //red
      #ifdef DEBUG_ALLOC
      Serial.println("redValue");
      Serial.println("lfo default else statement");
      #endif //DEBUG_ALLOC   
      lfoAd = 0;
      //instrumentSwitch = false;
      setColorIn(255, 0, 0);
    }
    button2Trig = false;
  }
}

void iridescentBasicSynth::myAfterTouchPoly(byte channel, byte note, byte velocity) {
  //#ifdef DEBUG_ALLOC
  Serial.print("AfterTouch Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", note=");
  Serial.print(note, DEC);
  Serial.print(", velocity=");
  Serial.println(velocity, DEC);
  //#endif //DEBUG_ALLOC

  /////
  //AudioNoInterrupts();
  ///////////

  /////
  //map to good ratio
  float a1a1Aa = 2.0;
  /////
  
  int waveformMod_id = findVoice(channel, note);
  float val = (float) map((float) velocity, 0.0, 127.0, 0.0, 1.0);
  float res = (float) map((float) velocity, 0.0, 127.0, 0.7, 5.0);
  float octaves = (float) map((float) velocity, 0.0, 127.0, 0.25, 3.5);
  a1a1Aa = (float) map((float) xValue, 0.0, 127.0, 2.0, 600.0);
  
  //sineForm[waveformMod_id].amplitude(val);
  //sineFM[waveformMod_id].amplitude(val);

  /////
  //pulsewidth modulation
  pwm1.frequency(a1a1Aa);
  pwm1.amplitude(val);
  /////

  /////
  //filter modulation
  filter1.resonance(res);
  filter1.octaveControl(octaves);
  /////

  /////
  //AudioInterrupts();
  ///////////
}

void iridescentBasicSynth::myProgramChange(byte channel, byte program) {
  #ifdef DEBUG_ALLOC
  Serial.print("Program Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", program=");
  Serial.println(program, DEC);
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myAfterTouchChannel(byte channel, byte pressure) {
  //#ifdef DEBUG_ALLOC
  Serial.print("After Touch, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pressure=");
  Serial.println(pressure, DEC);
  //#endif //DEBUG_ALLOC

  /////
  //map to good ratio
  float a1a1Aa = 2.0;
  /////
  
  float val = (float) map((float) pressure, 0.0, 127.0, 0.0, 1.0);
  float res = (float) map((float) pressure, 0.0, 127.0, 0.7, 5.0);
  float octaves = (float) map((float) pressure, 0.0, 127.0, 0.25, 3.5);
  a1a1Aa = (float) map(pressure, 0.0, 127.0, 2.0, 600.0);
  
  /////
  //AudioNoInterrupts();
  ///////////
  
  /////
  //pulsewidth modulation
  pwm1.frequency(a1a1Aa);
  pwm1.amplitude(val);
  /////

  /////
  //filter modulation
  filter1.resonance(res);
  filter1.octaveControl(octaves);
  /////

  /////
  //AudioInterrupts();
  ///////////
  
}

void iridescentBasicSynth::myPitchChange(byte channel, int pitch) {
  #ifdef DEBUG_ALLOC
  Serial.print("Pitch Change, ch=");
  Serial.print(channel, DEC);
  Serial.print(", pitch=");
  Serial.println(pitch, DEC);
  #endif //DEBUG_ALLOC
  
  /////
  //AudioNoInterrupts();
  ///////////
  
  for (int i = 0; i < TOTAL_VOICES; ++i) {
    if (waveformMod[i].isPlaying) {
      float minPitch = 0.0;
      float maxPitch = 0.0;
      if (waveformMod[i].midiNote > 1) {
        minPitch = midiNotes[waveformMod[i].midiNote - 2];
      }
      if (waveformMod[i].midiNote < 126) {
        maxPitch = midiNotes[waveformMod[i].midiNote + 2];
      }
      float bendNote = (float) map((float) pitch, -8192, 8192, minPitch, maxPitch);
      waveformMod[i].mod.frequency(bendNote);     
    }
  }
  /////
  //AudioInterrupts();
  ///////////
}

void iridescentBasicSynth::mySystemExclusiveChunk(const byte *data, uint16_t length, bool last) {
  #ifdef DEBUG_ALLOC
  Serial.print("SysEx Message: ");
  printBytes(data, length);
  if (last) {
    Serial.println(" (end)");
  } else {
    Serial.println(" (to be continued)");
  }
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::mySystemExclusive(byte *data, unsigned int length) {
  #ifdef DEBUG_ALLOC
  Serial.print("SysEx Message: ");
  printBytes(data, length);
  Serial.println();
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myTimeCodeQuarterFrame(byte data) { 
  static char SMPTE[8]={'0','0','0','0','0','0','0','0'};
  static byte fps=0;
  byte index = data >> 4;
  byte number = data & 15;
  #ifdef DEBUG_ALLOC
  if (index == 7) {
    fps = (number >> 1) & 3;
    number = number & 1;
  }
  if (index < 8 || number < 10) {
    SMPTE[index] = number + '0';
    Serial.print("TimeCode: ");  // perhaps only print when index == 7
    Serial.print(SMPTE[7]);
    Serial.print(SMPTE[6]);
    Serial.print(':');
    Serial.print(SMPTE[5]);
    Serial.print(SMPTE[4]);
    Serial.print(':');
    Serial.print(SMPTE[3]);
    Serial.print(SMPTE[2]);
    Serial.print('.');
    Serial.print(SMPTE[1]);  // perhaps add 2 to compensate for MIDI latency?
    Serial.print(SMPTE[0]);
    switch (fps) {
      case 0: Serial.println(" 24 fps"); break;
      case 1: Serial.println(" 25 fps"); break;
      case 2: Serial.println(" 29.97 fps"); break;
      case 3: Serial.println(" 30 fps"); break;
    }
  } else {
    Serial.print("TimeCode: invalid data = ");
    Serial.println(data, HEX);
  }
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::mySongPosition(uint16_t beats) {
  #ifdef DEBUG_ALLOC
  Serial.print("Song Position, beat=");
  Serial.println(beats);
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::mySongSelect(byte songNumber) {
  #ifdef DEBUG_ALLOC
  Serial.print("Song Select, song=");
  Serial.println(songNumber, DEC);
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myTuneRequest() {
  #ifdef DEBUG_ALLOC
  Serial.println("Tune Request");
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myClock() {
  #ifdef DEBUG_ALLOC
  Serial.println("Clock");
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myStart() {
  #ifdef DEBUG_ALLOC
  Serial.println("Start");
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myContinue() {
  #ifdef DEBUG_ALLOC
  Serial.println("Continue");
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myStop() {
  #ifdef DEBUG_ALLOC
  Serial.println("Stop");
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myActiveSensing() {
  #ifdef DEBUG_ALLOC
  Serial.println("Active Sensing");
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::mySystemReset() {
  #ifdef DEBUG_ALLOC
  Serial.println("System Reset");
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::myRealTimeSystem(uint8_t realtimebyte) {
  #ifdef DEBUG_ALLOC
  Serial.print("Real Time Message, code=");
  Serial.println(realtimebyte, HEX);
  #endif //DEBUG_ALLOC
}

void iridescentBasicSynth::printBytes(const byte *data, unsigned int size) {
  #ifdef DEBUG_ALLOC
  while (size > 0) {
    byte b = *data++;
    if (b < 16) Serial.print('0');
    Serial.print(b, HEX);
    if (size > 1) Serial.print(' ');
    size = size - 1;
  }
  #endif //DEBUG_ALLOC
}

//private functions

void iridescentBasicSynth::setColorIn(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

int iridescentBasicSynth::allocateVoice(byte channel, byte note) {
  int i;
  int nonfree_voices = stopped_voices + used_voices;
  if (nonfree_voices < TOTAL_VOICES) {
    for (i = nonfree_voices; i < TOTAL_VOICES && voices[i].channel != channel; ++i);
    if (i < TOTAL_VOICES) {
      voice_t temp = voices[i];
      voices[i] = voices[nonfree_voices];
      voices[nonfree_voices] = temp;
    }
    i = nonfree_voices;
    used_voices++;
  }
  else {
    if (stopped_voices) {
      i = evict_voice % stopped_voices;
      voice_t temp = voices[i];
      stopped_voices--;
      voices[i] = voices[stopped_voices];
      voices[stopped_voices] = temp;
      used_voices++;
      i = stopped_voices;
    }
    else
      i = evict_voice;
  }

  voices[i].channel = channel;
  voices[i].note = note;

  evict_voice++;
  evict_voice %= TOTAL_VOICES;

  return voices[i].waveformMod_id;
}

int iridescentBasicSynth::findVoice(byte channel, byte note) {
  int i;
  //find match
  int nonfree_voices = stopped_voices + used_voices;
  for (i = stopped_voices; i < nonfree_voices && !(voices[i].channel == channel && voices[i].note == note); ++i);
  //return TOTAL_VOICES if no match
  if (i == (nonfree_voices)) return TOTAL_VOICES;

  voice_t temp = voices[i];
  voices[i] = voices[stopped_voices];
  voices[stopped_voices] = temp;
  --used_voices;

  return voices[stopped_voices++].waveformMod_id;
}

void iridescentBasicSynth::freeVoices() {
  for (int i = 0; i < stopped_voices; i++)
    if (waveformMod[voices[i].waveformMod_id].isPlaying == false) {
      voice_t temp = voices[i];
      --stopped_voices;
      voices[i] = voices[stopped_voices];
      int nonfree_voices = stopped_voices + used_voices;
      voices[stopped_voices] = voices[nonfree_voices];
      voices[nonfree_voices] = temp;
    }
}

void iridescentBasicSynth::printVoices() {
  #ifdef DEBUG_ALLOC  
  static int last_notes_played = notes_played;
  if (last_notes_played == notes_played)
    return;
  last_notes_played = notes_played;
  int usage = AudioProcessorUsage();
  Serial.printf("\nCPU:%03i voices:%02i CPU/Voice:%02i evict:%02i\n", usage, used_voices, usage / used_voices, evict_voice);
  for (int i = 0; i < used_voices; ++i)
    Serial.printf(" %02hhu %-2s", voices[i].channel, note_map[voices[i].note % 12]);
  #endif //DEBUG_ALLOC
}
