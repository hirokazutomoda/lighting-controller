#include "MidiConverter.h"
#include "MidiHandler.h"

const int TRIGGER_KICK = 36; // 0x24
const int TRIGGER_SNARE = 38; 
const int TRIGGER_SNARE_RIM = 40; // 0x28
const int TRIGGER_FOOT_PEDAL = 48; 
const int TRIGGER_PAD = 45; //
const int TRIGGER_CHANNEL_SWITCH = 49;
const int TRIGGER_FLOOR_TOM = 46; // 0x2E

int main()
{
    MidiConverter mc;
    mc.openPort("UM-ONE");

    mc.registerHandler(67, TRIGGER_FOOT_PEDAL, handleLullaby);
    // mc.registerHandler(0, 60, handleDawn);
    mc.registerHandler(68, TRIGGER_KICK, handleDeathMarch);
    mc.registerHandler(69, TRIGGER_KICK, handleDeathMarchEnd);
    mc.registerHandler(69, TRIGGER_FLOOR_TOM, handleSpiralCogwheel);
    mc.registerHandler(72, TRIGGER_FOOT_PEDAL, handleSpiralCogwheelBreak);
    mc.registerHandler(72, TRIGGER_KICK, handleSpiralCogwheelHit);
    mc.registerHandler(73, TRIGGER_KICK, handleInfiniteCorridorKick);
    mc.registerHandler(73, TRIGGER_SNARE_RIM, handleInfiniteCorridorSnare);
    mc.registerHandler(74, TRIGGER_KICK, handleForcedLabourSong);
    mc.registerHandler(76, TRIGGER_FOOT_PEDAL, handleForcedLabourSongG);
    mc.registerHandler(76, TRIGGER_KICK, handleForcedLabourSongGKick);
    mc.registerHandler(77, TRIGGER_KICK, handleSpirograph);
    mc.registerHandler(78, TRIGGER_FLOOR_TOM, handle00000000);
    mc.registerHandler(78, TRIGGER_FOOT_PEDAL, handleFin);

    mc.run();
    return 0;
}
