# iridescentCoconutSynth
a FM synthesizer SGTL5000 & Teensy 4.1 – SUPPORTS MIDI VIA USB


Copyright 2020 George C. Rosar II

Licensed Apache 2.0

There is a javascript front-end available at: 
https://georgerosar.com/wavetable/

You can hear a quick demo here:
https://www.georgerosar.com/wp-content/uploads/2020/11/synthDemo.mp3

Here's a demo of the latest version, the USB audio from the computer is slightly being messed with. Playing over house music and a key on the piano flips it into mutation mode or computer background music in iTunes.

https://www.georgerosar.com/wp-content/uploads/2020/11/synthTestTwo_01.mp3

The project was originally intended to connect via WI-FI and have a web-server to control it.

You will have to slightly modify two teensyduino files:
```bash
/Applications/Arduino.app/Contents/Java/hardware/teensy/avr/cores/teensy3/usb_audio.cpp
```
AND
```bash
/Applications/Arduino.app/Contents/Java/hardware/teensy/avr/cores/teensy4/usb_audio.cpp
```
Add this line to near top of both files:
```cpp
#define MACOSX_ADAPTIVE_LIMIT
```

Here's some python help if you want to use python to link midi devices.

You may need to, in terminal:

```bash
pip3 install rtmidi
```

OR

```bash
pip install rtmidi
```

python or python3 midi thru code:
```python
#START HIGHLIGHTING
import rtmidi_python as rtmidi

midi_inSide = rtmidi.MidiIn()

midi_inSide.ignore_types( False, False, False ) #read sysx data

midi_outSide = rtmidi.MidiOut()

def callback(message, time_stamp):
	print message, time_stamp
	midi_outSide.send_message(message)

#the two lists
#lists all midi in ports starting with 0
for port_name in midi_inSide.ports:
	print port_name
#lists all midi out ports starting with 0
for port_name in midi_outSide.ports:
	print port_name

#stop highlighting. paste above so far into python interpretter press return twice
#then modify the lines below and paste into python interpretter plus a couple of returns
midi_inSide.open_port(2)    #starts with 0, midi keyboard plugged into computer 	RECEIVES HERE
midi_outSide.open_port(8)   #also starts with 0, the actual iridescentCoconutSynth device  SENDS HERE
midi_inSide.callback = callback #actually assigns the midi thru with function callback
```
Remember control+d once or twice, force quits python. Also note: control NOT Command (Mac OS)
