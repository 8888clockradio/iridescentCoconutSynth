# iridescentCoconutSynth
a [broken] FM synthesizer [that currently has too many voices and crashes] SGTL5000 & Teensy 3.6 – SUPPORTS MIDI VIA USB


Initially Copyright 2020 George C. Rosar II

Licensed Apache 2.0

There is a javascript front-end available at: 
http://georgerosar.com/wavetable/

The project was originally intended to connect via WI-FI and have a web-server to control it.


Here's some python help if you want to use python to link midi devices

```
import rtmidi_python as rtmidi

midi_inSide = rtmidi.MidiIn()

midi_inSide.ignore_types( False, False, False ) #read sysx data

midi_outSide = rtmidi.MidiOut()

def callback(message, time_stamp):
	print message, time_stamp
	midi_outSide.send_message(message)

for port_name in midi_inSide.ports:
	print port_name

for port_name in midi_outSide.ports:
	print port_name

midi_inSide.open_port(1)    #starts with 0
midi_outSide.open_port(8)   #also starts with 0
midi_inSide.callback = callback
```
