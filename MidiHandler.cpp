#include <thread>
#include "MidiHandler.h"

const unsigned char CH_LIGHT = 15;

// const unsigned char CC_H2O_DIMMER = 16;
// const unsigned char CC_H2O_ROTATION = 17;
// const unsigned char CC_H2O_COLOR = 18;

// const unsigned char CC_ALKALITE_RED = 19;
// const unsigned char CC_ALKALITE_GREEN = 20;
// const unsigned char CC_ALKALITE_BLUE = 21;
// // 0-79: dimming, 80-127: strobe slow to fast
// const unsigned char CC_ALKALITE_DIMMER = 22;


const unsigned char CC_MEGA_HEX_PAR_RED = 16;
const unsigned char CC_MEGA_HEX_PAR_GREEN = 17;
const unsigned char CC_MEGA_HEX_PAR_BLUE = 18;
const unsigned char CC_MEGA_HEX_PAR_WHITE = 19;
const unsigned char CC_MEGA_HEX_PAR_AMBER = 20;
const unsigned char CC_MEGA_HEX_PAR_UV = 21;
const unsigned char CC_MEGA_HEX_PAR_DIMMER = 22;
// 0-15: LED off, 16-31: LED on, 32-47: Strobe slow to fast
const unsigned char CC_MEGA_HEX_PAR_STROBE = 23;
// 0-25: Dimming mode, 26-51: Color Macro mode, 52-76: Color Change mode, 77-102: Color Fade mode, 103-127: Sound Active mode
const unsigned char CC_MEGA_HEX_PAR_PROGRAM_KEY = 24;
const unsigned char CC_MEGA_HEX_PAR_PROGRAM_VALUE = 25;
const unsigned char CC_MEGA_HEX_PAR_PROGRAM_SPEED = 26;
const unsigned char CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE = 27;

const unsigned char VAL_MEGA_HEX_PAR_CURVE_STANDARD = 10;
const unsigned char VAL_MEGA_HEX_PAR_CURVE_STAGE = 20;
const unsigned char VAL_MEGA_HEX_PAR_CURVE_THEATRE = 50;


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

// void fadeIn(RtMidiOut *midiout)
// {
//     for (int i = 0; i <= 127; i += 5)
//     {
//         sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
//         std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     }
// }

// void fadeOut(RtMidiOut *midiout)
// {
//     for (int i = 127; i >= 0; i -= 5)
//     {
//         sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
//         std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     }
// }

// void eFadeOut(RtMidiOut *midiout)
// {
//     int wait = 5000;
//     for (int i = 125; i >= 0; i--)
//     {
//         sendCC(midiout, CH_LIGHT, CC_H2O_DIMMER, i);
//         wait = wait * 1.1;
//         std::cout << "Waiting for " << wait << "ns" << std::endl;
//         std::this_thread::sleep_for(std::chrono::nanoseconds(wait));
//     }
// }

bool hollowOne = false;
int gCounter = 0;

void reset(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STAGE);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_STROBE, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_KEY, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_VALUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_SPEED, 0);


    // sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, 0);
    
    hollowOne = false;
    gCounter = 0;
}

void handleLullaby(RtMidiOut *midiout)
{
    reset(midiout);
}

void handleDeathMarch(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STANDARD);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 80);
}

void handleDeathMarchEnd(RtMidiOut *midiout)
{
    // sendCC(midiout, CH_LIGHT, CC_H2O_COLOR, VAL_H2O_COLOR_ORANGE_GREEN);
}
const unsigned char scdInit = 60;
unsigned char spiralCogwheelDimmer = scdInit;
void handleSpiralCogwheel(RtMidiOut *midiout)
{
    spiralCogwheelDimmer = spiralCogwheelDimmer + 5;
    if (spiralCogwheelDimmer > 127)
    {
        spiralCogwheelDimmer = 127;
    }
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, spiralCogwheelDimmer);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 80);

}

void handleSpiralCogwheelBreak(RtMidiOut *midiout)
{
    spiralCogwheelDimmer = scdInit;

    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 0);

}

void handleSpiralCogwheelHit(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_THEATRE);
    for (int i = 120; i >= 0; i -= 2)
    {
        sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void handleInfiniteCorridorKick(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 127);

    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 5);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (int i = 5; i >= 0; i -= 1)
    {
        sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void handleInfiniteCorridorSnare(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);

    for (int i = 119; i >= 20; i -= 3)
    {
        sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    for (int i = 40; i >= 0; i -= 2)
    {
        sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void handleForcedLabourSong(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 50);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 100);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 30);
}

void handleForcedLabourSongG(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 100);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 30);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 100);
}

void handleForcedLabourSongGKick(RtMidiOut *midiout)
{

    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 100);
    for (int i = 100; i >= 10; i -= 10)
    {
        sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void handleSpirograph(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 50);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 30);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 100);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 30);
}

void handlePrep00000000(RtMidiOut *midiout)
{
    gCounter++;
}

void handle00000000(RtMidiOut *midiout)
{
    if (gCounter <= 12)
    {
        return;
    }

    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 50);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 40);
}

void handleFin(RtMidiOut *midiout)
{
    gCounter = 0;
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STANDARD);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 0);
}

void handleStigma(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 20);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 30);
}

void handleManago(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STAGE);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 0);
}


void handleCradle(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STANDARD);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 30);
}


void handleCradleChorus(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STANDARD);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 40);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 70);
}

void handleUnkown(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_THEATRE);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 0);
}

void handleHollowOne(RtMidiOut *midiout)
{
    if (hollowOne)
    {
        return;
    }
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_THEATRE);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 30);
    hollowOne = true;
}

void handleHollow(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_THEATRE);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 80);
}

void handleBlack(RtMidiOut *midiout)
{
    reset(midiout);
}


void handleWhite(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STANDARD);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 30);
}

void handleSludge(RtMidiOut *midiout)
{
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_PROGRAM_DIMMER_CURVE, VAL_MEGA_HEX_PAR_CURVE_STANDARD);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_RED, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_GREEN, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_BLUE, 127);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_AMBER, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_UV, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_WHITE, 0);
    sendCC(midiout, CH_LIGHT, CC_MEGA_HEX_PAR_DIMMER, 50);
}