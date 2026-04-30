# MorseDecoder

### YouTube Demo
<a href="https://www.youtube.com/watch?v=Axz3j_8UlqE">
  <img src="https://img.youtube.com/vi/Axz3j_8UlqE/hqdefault.jpg" width="200">
</a>

### Summary
This project implements a real-time embedded system that captures analog audio signals and decodes them into Morse code using an Arduino Uno R3. The system processes live input from an analog sensor, performs basic signal conditioning, and interprets tone durations to reconstruct alphanumeric text.

### How it works

The system continuously samples an analog audio input and computes a smoothed signal using an exponential moving average filter. A baseline calibration step is performed at startup to adapt to ambient noise conditions.

The processed signal is converted into a binary tone state (ON/OFF) using a simple threshold detector. A lightweight state machine tracks transitions between sound and silence to measure:

- Dot vs dash durations
- Intra-character gaps
- Letter and word spacing

These timing intervals are used to build Morse code sequences in real time.

### Decoding logic

A fixed lookup table maps Morse patterns (e.g., ".-", "-...") to their corresponding ASCII characters (A–Z). Once a full character or word boundary is detected, the system decodes and outputs the result over serial communication.

Unrecognized patterns are safely handled by returning a placeholder character (?).

### Key features
- Real-time signal processing on constrained embedded hardware
- Baseline calibration for adaptive noise handling
- Exponential smoothing filter for signal stability
- State machine-based Morse decoding logic
- Static memory design (no dynamic allocation)
- Lightweight lookup table for fast decoding
- Serial output for live transcription

### Technical stack
- C/C++
- Arduino Uno R3
- Analog signal processing
- Time-domain analysis
- Embedded state machines
