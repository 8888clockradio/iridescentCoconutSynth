/*
        Copyright 2021 George C. Rosar II licensed under:
                 Apache License
           Version 2.0, January 2004
        http://www.apache.org/licenses/
        
        Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MIDI.h>
#include <usb_midi/usb_api.h>
//#include <usb_serial.h>
#include "constsAndStructs.h"
#include "iridescentBasicSynth.h"


//change the synths tuning
//change the line reading '#define A4PITCH 440.0" to set the tuning
//#define CHANGEPITCH       //COMMENT TO LEAVE A4 as CONCERT PITCH FOR A-440 //UNCOMMENT TO DE/RE-TUNE //JUST UNCOMMENTING THIS LINE WILL TUNE TO PHILOSOPHERS PITCH 432.0

#define NOSERIALPORT        //prevent serial port modem from showing

#ifdef CHANGEPITCH
float midiNotes[128];
//#define A4PITCH 440.0     //CONCERT A PITCH

#define A4PITCH 432.0       //PHILOSOPHERS PITCH
//#define A4PITCH 439.33    //ODD DETUNING JUST TO BE WEIRD
#else                       //USES STANDARD 440 CONCERT PITCH IF CHANGEPITCH IS NOT DEFINED
float midiNotes[128] = {8.18, 8.66, 
    9.18, 9.72, 10.30, 10.91, 
    11.56, 12.25, 12.98, 13.75, 
    14.57, 15.43, 16.35, 17.32, 
    18.35, 19.45, 20.60, 21.83, 
    23.12, 24.50, 25.96, 27.50, 
    29.14, 30.87, 32.70, 34.65, 
    36.71, 38.89, 41.20, 43.65, 
    46.25, 49.00, 51.91, 55.00, 
    58.27, 61.74, 65.41, 69.30, 
    73.42, 77.78, 82.41, 87.31, 
    92.50, 98.00, 103.83, 110.00, 
    116.54, 123.47, 130.81, 138.59, 
    146.83, 155.56, 164.81, 174.61, 
    185.00, 196.00, 207.65, 220.00, 
    233.08, 246.94, 261.63, 277.18, 
    293.66, 311.13, 329.63, 349.23, 
    369.99, 392.00, 415.30, 440.00, 
    466.16, 493.88, 523.25, 554.37, 
    587.33, 622.25, 659.26, 698.46, 
    739.99, 783.99, 830.61, 880.00, 
    932.33, 987.77, 1046.50, 1108.73, 
    1174.66, 1244.51, 1318.51, 1396.91, 
    1479.98, 1567.98, 1661.22, 1760.00, 
    1864.65, 1975.53, 2093.00, 2217.46, 
    2349.32, 2489.02, 2637.02, 2793.83, 
    2959.96, 3135.96, 3322.44, 3520.00, 
    3729.31, 3951.07, 4186.01, 4434.92, 
    4698.64, 4978.03, 5274.04, 5587.65, 
    5919.91, 6271.93, 6644.88, 7040.00, 
    7458.62, 7902.13, 8372.02, 8869.84, 
    9397.27, 9956.06, 10548.08, 11175.30, 
    11839.82, 12543.86,
};
#endif

#define CHANGEPITCHTWO       //COMMENT TO LEAVE A4 as CONCERT PITCH FOR A-440 //UNCOMMENT TO DE/RE-TUNE //JUST UNCOMMENTING THIS LINE WILL TUNE TO PHILOSOPHERS PITCH 432.0

#ifdef CHANGEPITCHTWO
float midiNotesTWO[128];
//#define A4PITCH 440.0     //CONCERT A PITCH

#define A4PITCHTWO 432.0       //PHILOSOPHERS PITCH
//#define A4PITCH 439.33    //ODD DETUNING JUST TO BE WEIRD
#else                       //USES STANDARD 440 CONCERT PITCH IF CHANGEPITCH IS NOT DEFINED
float midiNotesTWO[128] = {8.18, 8.66, 
    9.18, 9.72, 10.30, 10.91, 
    11.56, 12.25, 12.98, 13.75, 
    14.57, 15.43, 16.35, 17.32, 
    18.35, 19.45, 20.60, 21.83, 
    23.12, 24.50, 25.96, 27.50, 
    29.14, 30.87, 32.70, 34.65, 
    36.71, 38.89, 41.20, 43.65, 
    46.25, 49.00, 51.91, 55.00, 
    58.27, 61.74, 65.41, 69.30, 
    73.42, 77.78, 82.41, 87.31, 
    92.50, 98.00, 103.83, 110.00, 
    116.54, 123.47, 130.81, 138.59, 
    146.83, 155.56, 164.81, 174.61, 
    185.00, 196.00, 207.65, 220.00, 
    233.08, 246.94, 261.63, 277.18, 
    293.66, 311.13, 329.63, 349.23, 
    369.99, 392.00, 415.30, 440.00, 
    466.16, 493.88, 523.25, 554.37, 
    587.33, 622.25, 659.26, 698.46, 
    739.99, 783.99, 830.61, 880.00, 
    932.33, 987.77, 1046.50, 1108.73, 
    1174.66, 1244.51, 1318.51, 1396.91, 
    1479.98, 1567.98, 1661.22, 1760.00, 
    1864.65, 1975.53, 2093.00, 2217.46, 
    2349.32, 2489.02, 2637.02, 2793.83, 
    2959.96, 3135.96, 3322.44, 3520.00, 
    3729.31, 3951.07, 4186.01, 4434.92, 
    4698.64, 4978.03, 5274.04, 5587.65, 
    5919.91, 6271.93, 6644.88, 7040.00, 
    7458.62, 7902.13, 8372.02, 8869.84, 
    9397.27, 9956.06, 10548.08, 11175.30, 
    11839.82, 12543.86,
};
#endif

//#define DEBUG_ALLOC
#define TEENSY_41_PINS      //use TEENSY 4_1 PINS rather than 3_6
#define POLY_AFTER_TOUCH    //otherwise channel aftertouch
#define LINE_LEVEL          //otherwise MIC
//#define WIFI_ENABLED
//#define LEDS_ENABLED

#ifdef WIFI_ENABLED
#include <WiFi101.h>
#endif //WIFI_ENABLED 

AudioControlSGTL5000     sgtl5000_1;

AudioMixer4              betweenMixer1;
AudioMixer4              betweenMixer2;

AudioMixer4              synth1MasterOut1;
AudioMixer4              synth1MasterOut2;

AudioMixer4              synth2MasterOut1;
AudioMixer4              synth2MasterOut2;

AudioMixer4              MasterOut1;
AudioMixer4              MasterOut2;

AudioOutputI2S           i2s1;
AudioInputI2S            i2s2;           //xy=271,342
AudioInputUSB            usb1;           //xy=222,174
AudioOutputUSB           usb2;           //xy=785,189
//note remember to add '#define MACOSX_ADAPTIVE_LIMIT' in teensy3/usb_audio.cpp and teensy4/usb_audio.cpp
//NOTE!!!!!!!!!!!!!!!! /Applications/Arduino.app/Contents/Java/hardware/teensy/avr/cores/teensy3/usb_audio.cpp and /Applications/Arduino.app/Contents/Java/hardware/teensy/avr/cores/teensy4/usb_audio.cpp

/////
//audio in from computer
AudioConnection          patchCord1(usb1, 0, betweenMixer1, 0);
AudioConnection          patchCord2(usb1, 1, betweenMixer2, 0);
//audio from line-in or mic
AudioConnection          patchCord3(i2s2, 0, betweenMixer1, 1);
AudioConnection          patchCord4(i2s2, 1, betweenMixer2, 1);
//////

/////
//connect audio in to out
AudioConnection          patchCord10(betweenMixer1, 0, MasterOut1, 0);
AudioConnection          patchCord11(betweenMixer2, 0, MasterOut2, 0);
//synth1 to MasterOut
AudioConnection          patchCord14(synth1MasterOut1, 0, MasterOut1, 1);     //might need to make connection in setup
AudioConnection          patchCord15(synth1MasterOut2, 0, MasterOut2, 1);     //might need to make connection in setup
//synth2 to MasterOut
AudioConnection          patchCord16(synth2MasterOut1, 0, MasterOut1, 2);
AudioConnection          patchCord17(synth2MasterOut2, 0, MasterOut2, 2);
//////

//////
//output to computer
AudioConnection         patchCord5(MasterOut1, 0, usb2, 0);
AudioConnection         patchCord7(MasterOut2, 0, usb2, 1);

//output to line out
AudioConnection         patchCord24(MasterOut1, 0, i2s1, 0);
AudioConnection         patchCord25(MasterOut2, 0, i2s1, 1);

//////
//for USB
AudioEffectDigitalCombine      bypassCombine1;      //xy=334,644
AudioEffectDigitalCombine      bypassCombine2;      //xy=340,721

//////
//for LINE-IN/MIC
AudioEffectDigitalCombine      bypassCombine3;      //xy=334,644
AudioEffectDigitalCombine      bypassCombine4;      //xy=340,721

//might create feedback loop
//usb
AudioConnection          byMultCord1(usb1, 0, bypassCombine1, 0);
AudioConnection          byMultCord2(usb1, 1, bypassCombine2, 0);

//line/mic
AudioConnection          byMultCord3(i2s2, 0, bypassCombine3, 0);
AudioConnection          byMultCord4(i2s2, 1, bypassCombine4, 0);

//usb
AudioConnection          MObM1(MasterOut1, 0, bypassCombine1, 1);
AudioConnection          MObM2(MasterOut2, 0, bypassCombine2, 1);
//i2s2
AudioConnection          MObM3(MasterOut1, 0, bypassCombine3, 1);
AudioConnection          MObM4(MasterOut2, 0, bypassCombine4, 1);


//bypassMixer
AudioMixer4              combineMixer1;
AudioMixer4              combineMixer2;


//usb
AudioConnection          byMultMo1(bypassCombine1, 0, combineMixer1, 0);
AudioConnection          byMultMo2(bypassCombine2, 0, combineMixer2, 0);

//line/mic
AudioConnection          byMultMo3(bypassCombine3, 0, combineMixer1, 1);
AudioConnection          byMultMo4(bypassCombine4, 0, combineMixer2, 1);

AudioConnection          combineMultMo1(combineMixer1, 0, MasterOut1, 3);
AudioConnection          combineMultMo2(combineMixer2, 0, MasterOut2, 3);

Bounce button0 = Bounce(28, 15);
Bounce button1 = Bounce(39, 15);  // 15 = 15 ms debounce time
Bounce button2 = Bounce(30, 15);

int combineType = 0;
bool muteBG = true;
bool USB_OR_LINE_IN = true;

bool bypassInstrumentMode = false;
//bool bypassInstrumentModeClone = false;

#ifdef TEENSY_41_PINS
int redPin = A10;
int greenPin = A11;
int bluePin = A12;
int ledPin = 33;
int ledPin2 = 34;
#else
int redPin = A16;
int greenPin = A17;
int bluePin = A18;
int ledPin = 33;
int ledPin2 = 38;
#endif

iridescentBasicSynth  *synth1;
iridescentBasicSynth  *synth2;

bool selectSynth = false; //for synth1

void myControlChange(byte channel, byte control, byte value) {
  /////
  //AudioNoInterrupts();
  ///////////
  synth1->myControlChange(channel, control, value);
  synth2->myControlChange(channel, control, value); 
  /////
  //AudioInterrupts();
  ///////////
}

void myNoteOn(byte channel, byte note, byte velocity) {
  /////
  //AudioNoInterrupts();
  ///////////
  if (channel == 2) {
    //make independent for channel
    if (note == 28) { //channel 2 F0
      if (USB_OR_LINE_IN) {
        USB_OR_LINE_IN = false;
      }
      else {
        USB_OR_LINE_IN = true;
      }
    }
    if (note == 29) { //channel 2 F0
      if (muteBG) {
        muteBG = false;
      }
      else {
        muteBG = true;
      }
    }
    if (note == 31) { //channel 2 G0
      if (combineType == 0) {
        bypassCombine1.setCombineMode(AudioEffectDigitalCombine::XOR);
        bypassCombine2.setCombineMode(AudioEffectDigitalCombine::XOR);
        combineType = 1;
      }
      else if (combineType == 1) {
        bypassCombine1.setCombineMode(AudioEffectDigitalCombine::AND);
        bypassCombine2.setCombineMode(AudioEffectDigitalCombine::AND);
        combineType = 2;
      }
      else if (combineType == 2) {
        bypassCombine1.setCombineMode(AudioEffectDigitalCombine::MODULO);
        bypassCombine2.setCombineMode(AudioEffectDigitalCombine::MODULO);
        combineType = 3;
      }
      else if (combineType == 3) {
        bypassCombine1.setCombineMode(AudioEffectDigitalCombine::OR);
        bypassCombine2.setCombineMode(AudioEffectDigitalCombine::OR);
        combineType = 0;
      }
    }
    if (note == 35) {       //channel 2 B1
      if (!selectSynth) {
        selectSynth = true;
        #ifndef NOSERIALPORT   
        Serial.println("synth 2 selected");
        #endif //NOSERIALPORT
      }
      else {
        selectSynth = false;
        #ifndef NOSERIALPORT   
        Serial.println("synth 1 selected");
        #endif //NOSERIALPORT
      }
    }
    if (!selectSynth) {
      synth1->myNoteOn(channel, note, velocity);
    }
    else {
      synth2->myNoteOn(channel, note, velocity);
    }
   }
   else {
    synth1->myNoteOn(channel, note, velocity);
    synth2->myNoteOn(channel, note, velocity);
   }
  /////
  //AudioInterrupts();
  ///////////
}

void myNoteOff(byte channel, byte note, byte velocity) {
  /////
  //AudioNoInterrupts();
  ///////////
  synth1->myNoteOff(channel, note, velocity);
  synth2->myNoteOff(channel, note, velocity);
  /////
  //AudioInterrupts();
  ///////////
}

void myAfterTouchPoly(byte channel, byte note, byte velocity) {
#ifdef POLY_AFTER_TOUCH
  synth1->myAfterTouchPoly(channel, note, velocity);
  synth2->myAfterTouchPoly(channel, note, velocity);
#endif
}

void myProgramChange(byte channel, byte program) {
  synth1->myProgramChange(channel, program);
  synth2->myProgramChange(channel, program);
}

void myAfterTouchChannel(byte channel, byte pressure) {
#ifndef POLY_AFTER_TOUCH  
  synth1->myAfterTouchChannel(channel, pressure);
  synth2->myAfterTouchChannel(channel, pressure);
#endif
}

void myPitchChange(byte channel, int pitch) {
  synth1->myPitchChange(channel, pitch);
  synth2->myPitchChange(channel, pitch);
}

void mySystemExclusiveChunk(const byte *data, uint16_t length, bool last) {
  synth1->mySystemExclusiveChunk(data, length, last);
  synth2->mySystemExclusiveChunk(data, length, last);
}

void mySystemExclusive(byte *data, unsigned int length) {
  synth1->mySystemExclusive(data, length);
  synth2->mySystemExclusive(data, length);
}

void myTimeCodeQuarterFrame(byte data) {
  synth1->myTimeCodeQuarterFrame(data);
  synth2->myTimeCodeQuarterFrame(data);
}

void mySongPosition(uint16_t beats) {
  synth1->mySongPosition(beats);
  synth2->mySongPosition(beats);
}

void mySongSelect(byte songNumber) {
  synth1->mySongSelect(songNumber);
  synth2->mySongSelect(songNumber);
}

void myTuneRequest() {
  synth1->myTuneRequest();
  synth2->myTuneRequest();
}

void myClock() {
  synth1->myClock();
  synth2->myClock();
}

void myStart() {
  synth1->myStart();
  synth2->myStart();
}

void myContinue() {
  synth1->myContinue();
  synth2->myContinue();
}

void myStop() {
  synth1->myStop();
  synth2->myStop();
}

void myActiveSensing() {
  synth1->myActiveSensing();
  synth2->myActiveSensing();
}

void mySystemReset() {
  synth1->mySystemReset();
  synth2->mySystemReset();
}

void myRealTimeSystem(uint8_t realtimebyte) {
  synth1->myRealTimeSystem(realtimebyte);
  synth2->myRealTimeSystem(realtimebyte);
}

void printBytes(const byte *data, unsigned int size) {
  synth1->printBytes(data, size);
  synth2->printBytes(data, size);
}

void setup() {
  //#ifdef DEBUG_ALLOC
  #ifndef NOSERIALPORT    
  Serial.begin(115200);
  #endif //NOSERIALPORT
  //while (!Serial);
  //#endif //DEBUG_ALLOC 
  delay(2000);

#ifdef CHANGEPITCH
  //calculate pitch
  for (int i = 0; i < 128; i++) {
    midiNotes[i] = (float)(( (float)A4PITCH) * (float)pow((float)2.0, ((float(i) - (float)69.0) / (float)12.0)));
    #ifndef NOSERIALPORT   
    Serial.printf("%f, ", midiNotes[i]);
    #endif //NOSERIALPORT
  }
  #ifndef NOSERIALPORT   
  Serial.printf("\r\n");
  #endif //NOSERIALPORT
  //midiNotes[];
#endif

  pinMode(28, INPUT_PULLUP);
  pinMode(29, INPUT_PULLUP);
  pinMode(30, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  #ifdef WIFI_ENABLED
  pinMode (20, OUTPUT);
  SPI.setCS(20);
  SPI.setMOSI(7);
  SPI.setMISO(8);
  SPI.setSCK(14); // SCK on pin 14
  SPI.begin();
  WiFi.setPins(20, 26, 25); //26 is IRQ, 25 is RST
  #endif //WIFI_ENABLED 
    
  usbMIDI.setHandleNoteOn(myNoteOn);
  usbMIDI.setHandleNoteOff(myNoteOff);
  usbMIDI.setHandleAfterTouchPoly(myAfterTouchPoly);
  usbMIDI.setHandleControlChange(myControlChange);
  usbMIDI.setHandleProgramChange(myProgramChange);
  usbMIDI.setHandleAfterTouchChannel(myAfterTouchChannel);
  usbMIDI.setHandlePitchChange(myPitchChange);
  usbMIDI.setHandleSystemExclusive(mySystemExclusiveChunk);
  //usbMIDI.setHandleSystemExclusive(mySystemExclusive); 
  usbMIDI.setHandleTimeCodeQuarterFrame(myTimeCodeQuarterFrame);
  usbMIDI.setHandleSongPosition(mySongPosition);
  usbMIDI.setHandleSongSelect(mySongSelect);
  usbMIDI.setHandleTuneRequest(myTuneRequest);
  usbMIDI.setHandleClock(myClock);
  usbMIDI.setHandleStart(myStart);
  usbMIDI.setHandleContinue(myContinue);
  usbMIDI.setHandleStop(myStop);
  usbMIDI.setHandleActiveSensing(myActiveSensing);
  usbMIDI.setHandleSystemReset(mySystemReset);
  usbMIDI.setHandleRealTimeSystem(myRealTimeSystem);
  
  AudioMemory(128);  //still overkill 128 was 96
  
  /////
  //AudioNoInterrupts();
  ///////////


  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
#ifdef LINE_LEVEL
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
#endif
#ifndef LINE_LEVEL
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
#endif
  sgtl5000_1.lineInLevel(0.7);
  sgtl5000_1.unmuteHeadphone();
  sgtl5000_1.unmuteLineout();
  sgtl5000_1.audioPreProcessorEnable();
  sgtl5000_1.audioPostProcessorEnable(); // enable the DAP block in SGTL5000

  //////
  //create Synth Object
  synth1 = new iridescentBasicSynth(midiNotes, &synth1MasterOut1, &synth1MasterOut2, &button0, &button1, &button2, redPin, greenPin, bluePin, ledPin, ledPin2, &bypassInstrumentMode);
  synth2 = new iridescentBasicSynth(midiNotesTWO, &synth2MasterOut1, &synth2MasterOut2, &button0, &button1, &button2, redPin, greenPin, bluePin, ledPin, ledPin2, &bypassInstrumentMode);
  //////
  
  /////
  //AudioInterrupts();
  ///////////

  //while (!synth1 || !synth2);
  betweenMixer1.gain(0, 1.0);
  betweenMixer2.gain(0, 1.0);
  betweenMixer1.gain(1, 1.0);
  betweenMixer2.gain(1, 1.0);
  MasterOut1.gain(1, 1.0);
  MasterOut2.gain(1, 1.0);
  MasterOut1.gain(2, 1.0);
  MasterOut2.gain(2, 1.0);
  MasterOut1.gain(3, 0.0);
  MasterOut2.gain(3, 0.0);
  bypassCombine1.setCombineMode(AudioEffectDigitalCombine::OR);
  bypassCombine2.setCombineMode(AudioEffectDigitalCombine::OR);
  
  delay(2000);
  
}

void loop() {
  usbMIDI.read();
  //here are issues with pointers
  if (bypassInstrumentMode) {
    if (muteBG) {
      betweenMixer1.gain(0, 0.0);
      betweenMixer2.gain(0, 0.0);
      betweenMixer1.gain(1, 0.0);
      betweenMixer2.gain(1, 0.0);
    }
    else {
      if (USB_OR_LINE_IN) {
        betweenMixer1.gain(0, 0.65);
        betweenMixer2.gain(0, 0.65);
        betweenMixer1.gain(1, 0.0);
        betweenMixer2.gain(1, 0.0);
      }
      else {
        betweenMixer1.gain(0, 0.0);
        betweenMixer2.gain(0, 0.0);
        betweenMixer1.gain(1, 0.65);
        betweenMixer2.gain(1, 0.65);
      }
    }
    MasterOut1.gain(1, 0.78);
    MasterOut2.gain(1, 0.78);
    MasterOut1.gain(2, 0.78);
    MasterOut2.gain(2, 0.78);
    MasterOut1.gain(3, 0.5);
    MasterOut2.gain(3, 0.5);
  }
  else {
    betweenMixer1.gain(0, 1.0);
    betweenMixer2.gain(0, 1.0);
    betweenMixer1.gain(1, 1.0);
    betweenMixer2.gain(1, 1.0);
    MasterOut1.gain(1, 1.0);
    MasterOut2.gain(1, 1.0);
    MasterOut1.gain(2, 1.0);
    MasterOut2.gain(2, 1.0);
    MasterOut1.gain(3, 0.0);
    MasterOut2.gain(3, 0.0);
  }
  
  //runUpdate on Synth Objects
  /////
  //AudioNoInterrupts();
  ///////////
  synth1->updateSynth();
  synth2->updateSynth();
  /////
  //AudioInterrupts();
  ///////////
}
