#include "MidiConverter.h"
#include "MidiHandler.h"

const int TRIGGER_KICK = 36;
const int TRIGGER_FOOT_PEDAL = 48;
const int TRIGGER_SNARE_RIM = 40;
const int TRIGGER_FLOOR_TOM = 46;

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
    mc.registerHandler(74, TRIGGER_FOOT_PEDAL, handleForcedLabourSongG);
    mc.registerHandler(77, TRIGGER_KICK, handleSpirograph);
    mc.registerHandler(78, TRIGGER_KICK, handle00000000);

    mc.run();
    return 0;
}
