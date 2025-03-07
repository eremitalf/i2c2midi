// -------------------------------------------------------------------------------------------
// MIDI misc
// -------------------------------------------------------------------------------------------


// function for sending MIDI Program Change
void sendMidiProgramChange(int channel, int programNumber) { 
  
  // keep values in range
  if (channel < 0 || channel >= channelsOut) return;
  if (programNumber < 0 || programNumber > 127) return;

  if (isTRS(channel)) {
    MIDI.sendProgramChange(programNumber, channel+1);
    #ifdef USB_DEVICE
      usbMIDI.sendProgramChange(programNumber, channel+1);
    #endif
  } else {
    #ifdef MK2
      midiDevice.sendProgramChange(programNumber, channel+1-16);
    #endif
  }
  blinkLED(1);

  #ifdef DEBUG  
    Serial.print("Sending MIDI Program Change: ");
    Serial.print(programNumber); Serial.print(", ");
    Serial.println(channel+1);
  #endif 
}


// -------------------------------------------------------------------------------------------


// function for sending MIDI Pitch Bend
void sendMidiPitchBend(int channel, int value) {
  
  // keep values in range
  if (channel < 0 || channel >= channelsOut) return;
  if (value < -8192) value = -8192;
  else if (value > 8191) value = 8191;

  if (isTRS(channel)) {
    MIDI.sendPitchBend(value, channel+1);
    #ifdef USB_DEVICE
      usbMIDI.sendPitchBend(value, channel+1);
    #endif
  } else {
    #ifdef MK2
      midiDevice.sendPitchBend(value, channel+1-16);
    #endif
  }
  blinkLED(1);
  
  #ifdef DEBUG  
    Serial.print("Sending MIDI Pitch Bend: ");
    Serial.print(value); Serial.print(", ");
    Serial.println(channel+1);
  #endif 
}


// -------------------------------------------------------------------------------------------


// function for sending MIDI Aftertouch
void sendMidiAftertouch(int channel, int value) {
  
  // keep values in range
  if (channel < 0 || channel >= channelsOut) return;
  if (value < 0)   value = 0;
  if (value > 127) value = 127;

  if (isTRS(channel)) {
    MIDI.sendAfterTouch(value, channel+1);
    #ifdef USB_DEVICE
      usbMIDI.sendAfterTouch(value, channel+1);
    #endif   
  } else {
    #ifdef MK2
      midiDevice.sendAfterTouch(value, channel+1-16);
    #endif
  }        
  blinkLED(1);

  #ifdef DEBUG  
    Serial.print("Sending MIDI Aftertouch: ");
    Serial.print(value); Serial.print(", ");
    Serial.println(channel+1);
  #endif 
}


// -------------------------------------------------------------------------------------------


// function for sending MIDI Clock
void sendMidiClock() {
  MIDI.sendRealTime(midi::Clock);
  #ifdef USB_DEVICE
    usbMIDI.sendRealTime(usbMIDI.Clock);
  #endif   
  #ifdef MK2
    midiDevice.sendRealTime(midiDevice.Clock);
  #endif
}


// -------------------------------------------------------------------------------------------


// function for sending MIDI Clock Start
void sendMidiClockStart() {
  MIDI.sendRealTime(midi::Start);
  #ifdef USB_DEVICE
    usbMIDI.sendRealTime(usbMIDI.Start);
  #endif   
  #ifdef MK2
    midiDevice.sendRealTime(midiDevice.Start);
  #endif
  #ifdef DEBUG
    Serial.println("Sending MIDI Clock Start");
  #endif
}


// -------------------------------------------------------------------------------------------


// function for sending MIDI Clock Stop
void sendMidiClockStop() {
  MIDI.sendRealTime(midi::Stop);
  #ifdef USB_DEVICE
    usbMIDI.sendRealTime(usbMIDI.Stop);
  #endif  
  #ifdef MK2 
    midiDevice.sendRealTime(midiDevice.Stop);
  #endif
  #ifdef DEBUG
    Serial.println("Sending MIDI Clock Stop");
  #endif
}


// -------------------------------------------------------------------------------------------


// function for sending MIDI Clock Continue
void sendMidiClockContinue() {
  MIDI.sendRealTime(midi::Continue);
  #ifdef USB_DEVICE
    usbMIDI.sendRealTime(usbMIDI.Continue);
  #endif 
  #ifdef MK2  
    midiDevice.sendRealTime(midiDevice.Continue);
  #endif
  #ifdef DEBUG
    Serial.println("Sending MIDI Clock Continue");
  #endif
}


// -------------------------------------------------------------------------------------------


// MIDI panic
void panic() {
  
  // reset module 
  for (int i = 0; i < channelsOut; i++) {
    currentNoteDuration[i] = 100;
    currentNoteShift[i] = 0;
    currentRepetition[i] = 1;
    currentRatcheting[i] = 1;
    noteUpperLimit[i] = 127;
    noteLowerLimit[i] = 0;
  } 

  // send note offs and reset ratchets and repeats
  for (int j=0; j < 16; j++) {                           
    for (int i=0; i <= 127; i++) {                            
      midiNoteOff(j, i);
    }
  } 
  for (int j=0; j < channelsOut; j++) {                           
    for (int i=0; i < maxNotes; i++) {                            
      notes[j][i][5] = 0;   // reset ratchet count
      notes[j][i][6] = 0;   // reset repeat count
    }
  } 
  // reset scheduled notes
  for (int i = 0; i < maxNotesScheduled; i++) {
    for (int j = 0; j < 5; j++) {
      scheduledNotes[i][j] = 0;
    }
    scheduledNoteCount -= 1;
  }

  #ifdef DEBUG
    Serial.println("OMG!!!!1");
  #endif
}