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

class iridescentBasicSynth
{
public:  
  iridescentBasicSynth(AudioMixer4 *l, AudioMixer4 *r, Bounce *btn0, Bounce *btn1, Bounce *btn2, int rPin, int gPin, int bPin, int lPin, int lPin2);

  void myControlChange(byte channel, byte control, byte value);
  
  void myNoteOn(byte channel, byte note, byte velocity);
  
  void myNoteOff(byte channel, byte note, byte velocity);

  void updateSynth();
  
  void myAfterTouchPoly(byte channel, byte note, byte velocity);
  
  void myProgramChange(byte channel, byte program);
  
  void myAfterTouchChannel(byte channel, byte pressure);
  
  void myPitchChange(byte channel, int pitch);
  
  void mySystemExclusiveChunk(const byte *data, uint16_t length, bool last);
  
  void mySystemExclusive(byte *data, unsigned int length);
  
  void myTimeCodeQuarterFrame(byte data);
  
  void mySongPosition(uint16_t beats);
  
  void mySongSelect(byte songNumber);
  
  void myTuneRequest();
  
  void myClock();
  
  void myStart();
  
  void myContinue();
  
  void myStop();
  
  void myActiveSensing();
  
  void mySystemReset();
  
  void myRealTimeSystem(uint8_t realtimebyte);
  
  void printBytes(const byte *data, unsigned int size);
  
private:  
  bool instrumentSwitch = false;

  int used_voices = 0;
  int stopped_voices = 0;
  int evict_voice = 0;
  int notes_played = 0;

  bool button0Trig = false;
  bool button1Trig = false;
  bool button2Trig = false;
  bool bypass = false;

  int millisSeconds = 0;
  
  float eHoldDelay = 0; //200 GREEN
  float eHoldAttack = 9.2; // 11880 GREEN
  float eHoldHold = 2.1; // 11880 BLUE
  float eHoldDecay = 31.4; //11880 BLUE
  float eHoldSustain = 0.6; //0.0 to 1.0 PURPLE
  float eHoldRelease = 48.5; //11880 PURPLE

  float modulateValue = 0.0;
  float xValue = 127.0;
  float yValue = 100.0;
  bool pwmBypass = false; //start without pulse width modulation
  int filter = 0; //0 for low pass, 1 for band pass, 2 for high pass
  
  int lfoAd = 0; 
  
  int waveform_type = WAVEFORM_SAMPLE_HOLD;
  
  waveform_t                        waveformMod[TOTAL_VOICES];
  //AudioSynthWaveformSine            sineForm[TOTAL_VOICES];
  //AudioSynthWaveformSine            sineFM[TOTAL_VOICES];
  AudioSynthWaveformSine            sineForm;
  AudioSynthWaveformSine            sineFM;
  AudioEffectEnvelope               envelopeMod[TOTAL_VOICES];
  AudioMixer4                       mixer[TOTAL_MIXERS];
  
  AudioMixer4                       rogueMixer1;
  
  AudioMixer4                       filterMixer1;

  AudioMixer4                       pwmMixer1;

  AudioMixer4                       bypassFilter1;

  AudioMixer4                       filterHeaven1;

  AudioMixer4*                      left;
  AudioMixer4*                      right;
  
  AudioSynthWaveformPWM             pwm1;           //xy=438,604

  AudioSynthWaveformSine            sine1;          //xy=551,215
  
  AudioEffectMultiply               multiply1;      //xy=312,667
  
  AudioFilterStateVariable          filter1;        //xy=704,283

  AudioConnection FMToWaveform[TOTAL_VOICES] = {
    {sineFM, 0, waveformMod[0].mod, 0}, 
    {sineFM, 0, waveformMod[1].mod, 0}, 
    {sineFM, 0, waveformMod[2].mod, 0}, 
    {sineFM, 0, waveformMod[3].mod, 0}, 
    {sineFM, 0, waveformMod[4].mod, 0}, 
    {sineFM, 0, waveformMod[5].mod, 0}, 
    {sineFM, 0, waveformMod[6].mod, 0}, 
    {sineFM, 0, waveformMod[7].mod, 0}, 
    {sineFM, 0, waveformMod[8].mod, 0}, 
    {sineFM, 0, waveformMod[9].mod, 0}, 
    {sineFM, 0, waveformMod[10].mod, 0}, 
    {sineFM, 0, waveformMod[11].mod, 0}, 
    {sineFM, 0, waveformMod[12].mod, 0}, 
    {sineFM, 0, waveformMod[13].mod, 0}, 
    {sineFM, 0, waveformMod[14].mod, 0}, 
    {sineFM, 0, waveformMod[15].mod, 0}, 
    {sineFM, 0, waveformMod[16].mod, 0}, 
    {sineFM, 0, waveformMod[17].mod, 0}, 
    {sineFM, 0, waveformMod[18].mod, 0}, 
    {sineFM, 0, waveformMod[19].mod, 0}, 
    {sineFM, 0, waveformMod[20].mod, 0}, 
    {sineFM, 0, waveformMod[21].mod, 0}, 
    {sineFM, 0, waveformMod[22].mod, 0}, 
    {sineFM, 0, waveformMod[23].mod, 0}, 
    {sineFM, 0, waveformMod[24].mod, 0}, 
    {sineFM, 0, waveformMod[25].mod, 0}, 
    {sineFM, 0, waveformMod[26].mod, 0}, 
    {sineFM, 0, waveformMod[27].mod, 0}, 
    {sineFM, 0, waveformMod[28].mod, 0}, 
    {sineFM, 0, waveformMod[29].mod, 0}, 
    {sineFM, 0, waveformMod[30].mod, 0}, 
    {sineFM, 0, waveformMod[31].mod, 0}, 
    {sineFM, 0, waveformMod[32].mod, 0}, 
    {sineFM, 0, waveformMod[33].mod, 0}, 
    {sineFM, 0, waveformMod[34].mod, 0}, 
    {sineFM, 0, waveformMod[35].mod, 0}, 
    {sineFM, 0, waveformMod[36].mod, 0}, 
    {sineFM, 0, waveformMod[37].mod, 0}, 
    {sineFM, 0, waveformMod[38].mod, 0}, 
    {sineFM, 0, waveformMod[39].mod, 0}, 
    {sineFM, 0, waveformMod[40].mod, 0}, 
    {sineFM, 0, waveformMod[41].mod, 0}, 
    {sineFM, 0, waveformMod[42].mod, 0}, 
    {sineFM, 0, waveformMod[43].mod, 0}, 
    {sineFM, 0, waveformMod[44].mod, 0}, 
    {sineFM, 0, waveformMod[45].mod, 0}, 
    {sineFM, 0, waveformMod[46].mod, 0}, 
    {sineFM, 0, waveformMod[47].mod, 0}, 
    {sineFM, 0, waveformMod[48].mod, 0}, 
    {sineFM, 0, waveformMod[49].mod, 0}, 
    {sineFM, 0, waveformMod[50].mod, 0}, 
    {sineFM, 0, waveformMod[51].mod, 0}, 
    {sineFM, 0, waveformMod[52].mod, 0}, 
    {sineFM, 0, waveformMod[53].mod, 0}, 
    {sineFM, 0, waveformMod[54].mod, 0}, 
    {sineFM, 0, waveformMod[55].mod, 0}, 
    {sineFM, 0, waveformMod[56].mod, 0}, 
    {sineFM, 0, waveformMod[57].mod, 0}, 
    {sineFM, 0, waveformMod[58].mod, 0}, 
    {sineFM, 0, waveformMod[59].mod, 0}, 
    {sineFM, 0, waveformMod[60].mod, 0}, 
    {sineFM, 0, waveformMod[61].mod, 0}, 
    {sineFM, 0, waveformMod[62].mod, 0}, 
    {sineFM, 0, waveformMod[63].mod, 0}, 
  };
  
  AudioConnection sineToFM[TOTAL_VOICES] = {
    {sineForm, 0, waveformMod[0].mod, 1}, 
    {sineForm, 0, waveformMod[1].mod, 1}, 
    {sineForm, 0, waveformMod[2].mod, 1}, 
    {sineForm, 0, waveformMod[3].mod, 1}, 
    {sineForm, 0, waveformMod[4].mod, 1}, 
    {sineForm, 0, waveformMod[5].mod, 1}, 
    {sineForm, 0, waveformMod[6].mod, 1}, 
    {sineForm, 0, waveformMod[7].mod, 1}, 
    {sineForm, 0, waveformMod[8].mod, 1}, 
    {sineForm, 0, waveformMod[9].mod, 1}, 
    {sineForm, 0, waveformMod[10].mod, 1}, 
    {sineForm, 0, waveformMod[11].mod, 1}, 
    {sineForm, 0, waveformMod[12].mod, 1}, 
    {sineForm, 0, waveformMod[13].mod, 1}, 
    {sineForm, 0, waveformMod[14].mod, 1}, 
    {sineForm, 0, waveformMod[15].mod, 1}, 
    {sineForm, 0, waveformMod[16].mod, 1}, 
    {sineForm, 0, waveformMod[17].mod, 1}, 
    {sineForm, 0, waveformMod[18].mod, 1}, 
    {sineForm, 0, waveformMod[19].mod, 1}, 
    {sineForm, 0, waveformMod[20].mod, 1}, 
    {sineForm, 0, waveformMod[21].mod, 1}, 
    {sineForm, 0, waveformMod[22].mod, 1}, 
    {sineForm, 0, waveformMod[23].mod, 1}, 
    {sineForm, 0, waveformMod[24].mod, 1}, 
    {sineForm, 0, waveformMod[25].mod, 1}, 
    {sineForm, 0, waveformMod[26].mod, 1}, 
    {sineForm, 0, waveformMod[27].mod, 1}, 
    {sineForm, 0, waveformMod[28].mod, 1}, 
    {sineForm, 0, waveformMod[29].mod, 1}, 
    {sineForm, 0, waveformMod[30].mod, 1}, 
    {sineForm, 0, waveformMod[31].mod, 1}, 
    {sineForm, 0, waveformMod[32].mod, 1}, 
    {sineForm, 0, waveformMod[33].mod, 1}, 
    {sineForm, 0, waveformMod[34].mod, 1}, 
    {sineForm, 0, waveformMod[35].mod, 1}, 
    {sineForm, 0, waveformMod[36].mod, 1}, 
    {sineForm, 0, waveformMod[37].mod, 1}, 
    {sineForm, 0, waveformMod[38].mod, 1}, 
    {sineForm, 0, waveformMod[39].mod, 1}, 
    {sineForm, 0, waveformMod[40].mod, 1}, 
    {sineForm, 0, waveformMod[41].mod, 1}, 
    {sineForm, 0, waveformMod[42].mod, 1}, 
    {sineForm, 0, waveformMod[43].mod, 1}, 
    {sineForm, 0, waveformMod[44].mod, 1}, 
    {sineForm, 0, waveformMod[45].mod, 1}, 
    {sineForm, 0, waveformMod[46].mod, 1}, 
    {sineForm, 0, waveformMod[47].mod, 1}, 
    {sineForm, 0, waveformMod[48].mod, 1}, 
    {sineForm, 0, waveformMod[49].mod, 1}, 
    {sineForm, 0, waveformMod[50].mod, 1}, 
    {sineForm, 0, waveformMod[51].mod, 1}, 
    {sineForm, 0, waveformMod[52].mod, 1}, 
    {sineForm, 0, waveformMod[53].mod, 1}, 
    {sineForm, 0, waveformMod[54].mod, 1}, 
    {sineForm, 0, waveformMod[55].mod, 1}, 
    {sineForm, 0, waveformMod[56].mod, 1}, 
    {sineForm, 0, waveformMod[57].mod, 1}, 
    {sineForm, 0, waveformMod[58].mod, 1}, 
    {sineForm, 0, waveformMod[59].mod, 1}, 
    {sineForm, 0, waveformMod[60].mod, 1}, 
    {sineForm, 0, waveformMod[61].mod, 1}, 
    {sineForm, 0, waveformMod[62].mod, 1}, 
    {sineForm, 0, waveformMod[63].mod, 1},
  };
  
  AudioConnection envelopes1[TOTAL_VOICES] = {
    { waveformMod[0].mod, envelopeMod[0] }, { waveformMod[1].mod, envelopeMod[1] }, 
    { waveformMod[2].mod, envelopeMod[2] }, { waveformMod[3].mod, envelopeMod[3] }, { waveformMod[4].mod, envelopeMod[4] }, { waveformMod[5].mod, envelopeMod[5] }, 
    { waveformMod[6].mod, envelopeMod[6] }, { waveformMod[7].mod, envelopeMod[7] }, { waveformMod[8].mod, envelopeMod[8] }, { waveformMod[9].mod, envelopeMod[9] }, 
    { waveformMod[10].mod, envelopeMod[10] }, { waveformMod[11].mod, envelopeMod[11] }, { waveformMod[12].mod, envelopeMod[12] }, { waveformMod[13].mod, envelopeMod[13] }, 
    { waveformMod[14].mod, envelopeMod[14] }, { waveformMod[15].mod, envelopeMod[15] }, { waveformMod[16].mod, envelopeMod[16] }, { waveformMod[17].mod, envelopeMod[17] }, 
    { waveformMod[18].mod, envelopeMod[18] }, { waveformMod[19].mod, envelopeMod[19] }, { waveformMod[20].mod, envelopeMod[20] }, { waveformMod[21].mod, envelopeMod[21] }, 
    { waveformMod[22].mod, envelopeMod[22] }, { waveformMod[23].mod, envelopeMod[23] }, { waveformMod[24].mod, envelopeMod[24] }, { waveformMod[25].mod, envelopeMod[25] }, 
    { waveformMod[26].mod, envelopeMod[26] }, { waveformMod[27].mod, envelopeMod[27] }, { waveformMod[28].mod, envelopeMod[28] }, { waveformMod[29].mod, envelopeMod[29] }, 
    { waveformMod[30].mod, envelopeMod[30] }, { waveformMod[31].mod, envelopeMod[31] }, { waveformMod[32].mod, envelopeMod[32] }, { waveformMod[33].mod, envelopeMod[33] }, 
    { waveformMod[34].mod, envelopeMod[34] }, { waveformMod[35].mod, envelopeMod[35] }, { waveformMod[36].mod, envelopeMod[36] }, { waveformMod[37].mod, envelopeMod[37] }, 
    { waveformMod[38].mod, envelopeMod[38] }, { waveformMod[39].mod, envelopeMod[39] }, { waveformMod[40].mod, envelopeMod[40] }, { waveformMod[41].mod, envelopeMod[41] }, 
    { waveformMod[42].mod, envelopeMod[42] }, { waveformMod[43].mod, envelopeMod[43] }, { waveformMod[44].mod, envelopeMod[44] }, { waveformMod[45].mod, envelopeMod[45] }, 
    { waveformMod[46].mod, envelopeMod[46] }, { waveformMod[47].mod, envelopeMod[47] }, { waveformMod[48].mod, envelopeMod[48] }, { waveformMod[49].mod, envelopeMod[49] }, 
    { waveformMod[50].mod, envelopeMod[50] }, { waveformMod[51].mod, envelopeMod[51] }, { waveformMod[52].mod, envelopeMod[52] }, { waveformMod[53].mod, envelopeMod[53] }, 
    { waveformMod[54].mod, envelopeMod[54] }, { waveformMod[55].mod, envelopeMod[55] }, { waveformMod[56].mod, envelopeMod[56] }, { waveformMod[57].mod, envelopeMod[57] }, 
    { waveformMod[58].mod, envelopeMod[58] }, { waveformMod[59].mod, envelopeMod[59] }, { waveformMod[60].mod, envelopeMod[60] }, { waveformMod[61].mod, envelopeMod[61] }, 
    { waveformMod[62].mod, envelopeMod[62] }, { waveformMod[63].mod, envelopeMod[63] }, 
  };

  AudioConnection patchCord[ARRAY_SIZE]  = {
    {envelopeMod[0], 0, mixer[0], 0}, {envelopeMod[1], 0, mixer[0], 1}, {envelopeMod[2], 0, mixer[0],  2}, {envelopeMod[3], 0, mixer[0],  3}, {mixer[0], 0, mixer[TOTAL_MIXERS - 2], 0},                    //5
    {envelopeMod[4], 0, mixer[1], 0}, {envelopeMod[5], 0, mixer[1], 1}, {envelopeMod[6], 0, mixer[1],  2}, {envelopeMod[7], 0, mixer[1],  3}, {mixer[1], 0, mixer[TOTAL_MIXERS - 2], 1},                    //5
    {envelopeMod[8], 0, mixer[2], 0}, {envelopeMod[9], 0, mixer[2], 1}, {envelopeMod[10], 0, mixer[2],  2}, {envelopeMod[11], 0, mixer[2],  3}, {mixer[2], 0, mixer[TOTAL_MIXERS - 2], 2},                  //5
    {envelopeMod[12], 0, mixer[3], 0}, {envelopeMod[13], 0, mixer[3], 1}, {envelopeMod[14], 0, mixer[3],  2}, {envelopeMod[15], 0, mixer[3],  3}, {mixer[3], 0, mixer[TOTAL_MIXERS - 2], 3},                //5
    {envelopeMod[16], 0, mixer[4], 0}, {envelopeMod[17], 0, mixer[4], 1}, {envelopeMod[18], 0, mixer[4],  2}, {envelopeMod[19], 0, mixer[4],  3}, {mixer[4], 0, mixer[TOTAL_MIXERS - 3], 0},                //5
    {envelopeMod[20], 0, mixer[5], 0}, {envelopeMod[21], 0, mixer[5], 1}, {envelopeMod[22], 0, mixer[5],  2}, {envelopeMod[23], 0, mixer[5],  3}, {mixer[5], 0, mixer[TOTAL_MIXERS - 3], 1},                //5
    {envelopeMod[24], 0, mixer[6], 0}, {envelopeMod[25], 0, mixer[6], 1}, {envelopeMod[26], 0, mixer[6],  2}, {envelopeMod[27], 0, mixer[6],  3}, {mixer[6], 0, mixer[TOTAL_MIXERS - 3], 2},                //5
    {envelopeMod[28], 0, mixer[7], 0}, {envelopeMod[29], 0, mixer[7], 1}, {envelopeMod[30], 0, mixer[7],  2}, {envelopeMod[31], 0, mixer[7],  3}, {mixer[7], 0, mixer[TOTAL_MIXERS - 3], 3},                //5
    {envelopeMod[32], 0, mixer[8], 0}, {envelopeMod[33], 0, mixer[8], 1}, {envelopeMod[34], 0, mixer[8],  2}, {envelopeMod[35], 0, mixer[8],  3}, {mixer[8], 0, mixer[TOTAL_MIXERS - 4], 0},                //5
    {envelopeMod[36], 0, mixer[9], 0}, {envelopeMod[37], 0, mixer[9], 1}, {envelopeMod[38], 0, mixer[9],  2}, {envelopeMod[39], 0, mixer[9],  3}, {mixer[9], 0, mixer[TOTAL_MIXERS - 4], 1},                //5
    {envelopeMod[40], 0, mixer[10], 0}, {envelopeMod[41], 0, mixer[10], 1}, {envelopeMod[42], 0, mixer[10], 2}, {envelopeMod[43], 0, mixer[10], 3}, {mixer[10], 0, mixer[TOTAL_MIXERS - 4], 2},             //5
    {envelopeMod[44], 0, mixer[11], 0}, {envelopeMod[45], 0, mixer[11], 1}, {envelopeMod[46], 0, mixer[11], 2}, {envelopeMod[47], 0, mixer[11], 3}, {mixer[11], 0, mixer[TOTAL_MIXERS - 4], 3},             //5
    {envelopeMod[48], 0, mixer[12], 0}, {envelopeMod[49], 0, mixer[12], 1}, {envelopeMod[50], 0, mixer[12], 2}, {envelopeMod[51], 0, mixer[12], 3}, {mixer[12], 0, mixer[TOTAL_MIXERS - 5], 0},             //5
    {envelopeMod[52], 0, mixer[13], 0}, {envelopeMod[53], 0, mixer[13], 1}, {envelopeMod[54], 0, mixer[13], 2}, {envelopeMod[55], 0, mixer[13], 3}, {mixer[13], 0, mixer[TOTAL_MIXERS - 5], 1},             //5
    {envelopeMod[56], 0, mixer[14], 0}, {envelopeMod[57], 0, mixer[14], 1}, {envelopeMod[58], 0, mixer[14], 2}, {envelopeMod[59], 0, mixer[14], 3}, {mixer[14], 0, mixer[TOTAL_MIXERS - 5], 2},             //5
    {envelopeMod[60], 0, mixer[15], 0}, {envelopeMod[61], 0, mixer[15], 1}, {envelopeMod[62], 0, mixer[15], 2}, {envelopeMod[63], 0, mixer[15], 3}, {mixer[15], 0, mixer[TOTAL_MIXERS - 5], 3},             //5 * 16ROWS = 90
    {mixer[TOTAL_MIXERS - 2], 0, mixer[TOTAL_MIXERS - 1], 0},                                                                                                                                               //1 + 90 = 91
    {mixer[TOTAL_MIXERS - 3], 0, mixer[TOTAL_MIXERS - 1], 1},                                                                                                                                               //1 + 91 = 92
    {mixer[TOTAL_MIXERS - 4], 0, mixer[TOTAL_MIXERS - 1], 2},                                                                                                                                               //1 + 92 = 93
    {mixer[TOTAL_MIXERS - 5], 0, mixer[TOTAL_MIXERS - 1], 3},                                                                                                                                               //1 + 93 = 94
    {mixer[TOTAL_MIXERS - 1], 0, rogueMixer1, 0},                                                                                                                                                           //1 + 94 = 95
    {rogueMixer1, 0, pwmMixer1, 1}, {rogueMixer1, 0, pwm1, 0},  {pwm1, 0, multiply1, 0},                                                                                                                    //3 + 95 = 98
    {rogueMixer1, 0, multiply1, 1}, {multiply1, 0, pwmMixer1, 0},  {sine1, 0, filter1, 1},                                                                                                                  //3 + 98 = 101
    {pwmMixer1, 0, filterMixer1, 0}, {filterMixer1, 0, filter1, 0}, {pwmMixer1, 0, bypassFilter1, 0},                                                                                                       //3 + 101 = 104
    {filterMixer1, 0, filterHeaven1, 0}, {filterMixer1, 1, filterHeaven1, 1}, {filterMixer1, 2, filterHeaven1, 2}, {bypassFilter1, 0, filterHeaven1, 3},                                                    //4 + 104 = 108                                                                                                                                       //2 + 90 = 92
  };

  AudioConnection *         fm1l1;
  AudioConnection *         fm1r1;

  voice_t voices[TOTAL_VOICES];

  int redPin;
  int greenPin;
  int bluePin;
  int ledPin;
  int ledPin2;

  void setColorIn(int redValue, int greenValue, int blueValue);

  int allocateVoice(byte channel, byte note);
  
  int findVoice(byte channel, byte note); //fix this by using midi note number

  void freeVoices();
  
  void printVoices();
  
};
