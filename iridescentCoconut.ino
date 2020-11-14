
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
#include <Bounce.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <log2.h>
#include "constsAndStructs.h"
#include "iridescentBasicSynth.h"

//#define DEBUG_ALLOC
#define TEENSY_41_PINS      //use TEENSY 4_1 PINS rather than 3_6
#define POLY_AFTER_TOUCH    //otherwise channel aftertouch
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


Bounce button0 = Bounce(28, 15);
Bounce button1 = Bounce(39, 15);  // 15 = 15 ms debounce time
Bounce button2 = Bounce(30, 15);


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
    if (note == 35) {       //channel 2 C1
      if (!selectSynth) {
        selectSynth = true;
        Serial.println("synth 2 selected");
      }
      else {
        selectSynth = false;
        Serial.println("synth 1 selected");
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
  Serial.begin(115200);
  while (!Serial);
  //#endif //DEBUG_ALLOC 

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
  
  AudioMemory(96);  //still overkill 128
  
  /////
  //AudioNoInterrupts();
  ///////////


  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(0.7);
  sgtl5000_1.unmuteHeadphone();
  sgtl5000_1.unmuteLineout();
  sgtl5000_1.audioPreProcessorEnable();
  sgtl5000_1.audioPostProcessorEnable(); // enable the DAP block in SGTL5000

  //////
  //create Synth Object
  synth1 = new iridescentBasicSynth(&synth1MasterOut1, &synth1MasterOut2, &button0, &button1, &button2, redPin, greenPin, bluePin, ledPin, ledPin2);
  synth2 = new iridescentBasicSynth(&synth2MasterOut1, &synth2MasterOut2, &button0, &button1, &button2, redPin, greenPin, bluePin, ledPin, ledPin2);
  //////
  
  /////
  //AudioInterrupts();
  ///////////
  
  delay(2000);
  
}

void loop() {
  usbMIDI.read();
  
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
