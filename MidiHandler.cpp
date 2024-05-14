#include <thread>
#include "MidiHandler.h"

const unsigned char CH_LIGHT = 15;

const unsigned char CC_H2O_DIMMER = 16;
const unsigned char CC_H2O_ROTATION = 17;
const unsigned char CC_H2O_COLOR = 18;

const unsigned char CC_ALKALITE_RED = 19;
const unsigned char CC_ALKALITE_GREEN = 20;
const unsigned char CC_ALKALITE_BLUE = 21;
// 0-79: dimming, 80-127: strobe slow to fast
const unsigned char CC_ALKALITE_DIMMER = 22;

const unsigned char VAL_H2O_COLOR_WHITE = 0;
const unsigned char VAL_H2O_COLOR_ORANGE = 15;
const unsigned char VAL_H2O_COLOR_ORANGE_GREEN = 20;
const unsigned char VAL_H2O_COLOR_PURPLE = 60;
const unsigned char VAL_H2O_COLOR_PURPLE_WHITE = 61;

void sendCC(RtMidiOut *midiout, unsigned char channel, unsigned char ccNumber, unsigned char value)
{
    std::vector<unsigned char> message = {static_cast<unsigned char>(0xB0 + channel), ccNumber, value};
    midiout->sendMessage(&message);
    std::cout << "Sending CC: ";
    for (auto byte : message)
    {
        std::cout << std::hex << +byte << " ";
    }
    std::cout << std::dec << std::endl;
}

void fadeIn(RtMidiOut *midiout)
{
    for (int i = 0; i <= 127; i += 5)
    {
        sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void fadeOut(RtMidiOut *midiout)
{
    for (int i = 127; i >= 0; i -= 5)
    {
        sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void eFadeOut(RtMidiOut *midiout)
{
    int wait = 5000;
    for (int i = 125; i >= 0; i--)
    {
        sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
        wait = wait * 1.1;
        std::cout << "Waiting for " << wait << "ns" << std::endl;
        std::this_thread::sleep_for(std::chrono::nanoseconds(wait));
    }
}

void handleLullaby(RtMidiOut *midiout)
{
    // only setup colour, no light
    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 0);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 40);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_ORANGE);

    // reset
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, 0);
}

void handleDeathMarch(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 40);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 40);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_ORANGE);

    sendCC(midiout, CH_LIGHT, CC_ALKALITE_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, 0);
}

void handleDeathMarchEnd(RtMidiOut *midiout)
{
    // sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_ORANGE_GREEN);
}

unsigned char spiralCogwheelDimmer = 60;
void handleSpiralCogwheel(RtMidiOut *midiout)
{
    spiralCogwheelDimmer = spiralCogwheelDimmer + 3;
    if (spiralCogwheelDimmer > 127)
    {
        spiralCogwheelDimmer = 127;
    }
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_RED, spiralCogwheelDimmer);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, 30);

    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 60);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 55);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_ORANGE_GREEN);
}

void handleSpiralCogwheelBreak(RtMidiOut *midiout)
{
    spiralCogwheelDimmer = 60;
    
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, 0);

    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 5);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 60);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_WHITE);
}

void handleSpiralCogwheelHit(RtMidiOut *midiout)
{

    for (int i = 60; i >= 0; i -= 1)
    {
        sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 0);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_WHITE);
}

void handleInfiniteCorridorKick(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 0);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_ORANGE);

    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (int i = 5; i >= 0; i -= 1)
    {
        sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void handleInfiniteCorridorSnare(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_RED, 127);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_BLUE, 0);

    for (int i = 78; i >= 20; i -= 2)
    {
        sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    for (int i = 20; i >= 0; i -= 1)
    {
        sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void handleForcedLabourSong(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 10);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 10);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_PURPLE_WHITE);
}

void handleForcedLabourSongG(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 2);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 10);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_ORANGE);
}

void handleForcedLabourSongGKick(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_BLUE, 0);

    sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, 10);
    for (int i = 20; i >= 6; i -= 2)
    {
        sendCC(midiout, CH_LIGHT, CC_ALKALITE_RED, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void handleSpirograph(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 5);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 60);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_WHITE);
}

void handle00000000(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, 0);
    sendCC(midiout, CH_LIGHT, CC_H2O_ROTATION, 0);
    sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, 0);

    sendCC(midiout, CH_LIGHT, CC_ALKALITE_RED, 127);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, 127);
    // sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, 40);
}

void handleFin(RtMidiOut *midiout)
{
    for (int i = 40; i >= 0; i -= 5)
    {
        sendCC(midiout, CH_LIGHT, CC_ALKALITE_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}