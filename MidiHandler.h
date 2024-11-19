#pragma once
#include <rtmidi/RtMidi.h>

void reset(RtMidiOut *midiout);
void handleLullaby(RtMidiOut *midiout);
void handleDeathMarch(RtMidiOut *midiout);
void handleDeathMarchEnd(RtMidiOut *midiout);
void handleSpiralCogwheel(RtMidiOut *midiout);
void handleSpiralCogwheelBreak(RtMidiOut *midiout);
void handleSpiralCogwheelHit(RtMidiOut *midiout);
void handleInfiniteCorridorKick(RtMidiOut *midiout);
void handleInfiniteCorridorSnare(RtMidiOut *midiout);
void handleForcedLabourSong(RtMidiOut *midiout);
void handleForcedLabourSongG(RtMidiOut *midiout);
void handleForcedLabourSongGKick(RtMidiOut *midiout);
void handleSpirograph(RtMidiOut *midiout);
void handlePrep00000000(RtMidiOut *midiout);
void handle00000000(RtMidiOut *midiout);
void handleFin(RtMidiOut *midiout);

void handleStigma(RtMidiOut *midiout);
void handleManago(RtMidiOut *midiout);
void handleCradle(RtMidiOut *midiout);
void handleCradleChorus(RtMidiOut *midiout);
void handleUnkown(RtMidiOut *midiout);
void handleHollowOne(RtMidiOut *midiout);
void handleHollow(RtMidiOut *midiout);