#pragma once
#include <rtmidi/RtMidi.h>
#include <functional>
#include <map>
#include <mutex>

// Define the callback type, which now also needs to know about the RtMidiOut* to send messages
using MidiCallback = std::function<void(RtMidiOut *)>;

// Define a struct to hold program and note for map keys
struct ProgramNoteKey
{
    int programNumber;
    int noteNumber;
    bool operator<(const ProgramNoteKey &other) const
    {
        return std::tie(programNumber, noteNumber) < std::tie(other.programNumber, other.noteNumber);
    }
};

class MidiConverter
{
private:
    std::mutex mutex;
    RtMidiIn *midiin = nullptr;
    RtMidiOut *midiout = nullptr;
    bool passThrough = true;
    // std::map<int, std::function<void(RtMidiOut*)>> handlers;
    std::map<ProgramNoteKey, std::vector<MidiCallback>> handlers;
    std::map<int, int> currentProgram;

public:
    MidiConverter();
    ~MidiConverter();
    void openPort(const std::string &portName);
    // void registerHandler(int note, std::function<void(RtMidiOut*)> handler);
    void registerHandler(int programNumber, int noteNumber, MidiCallback callback);
    void run();
};
