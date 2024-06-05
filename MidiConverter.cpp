#include "MidiConverter.h"
#include <iostream>
#include <chrono>
#include <thread>

MidiConverter::MidiConverter()
{
    midiin = new RtMidiIn();
    midiout = new RtMidiOut();
}

MidiConverter::~MidiConverter()
{
    delete midiin;
    delete midiout;
}

int findPortByName(RtMidi *midi, const std::string &portName)
{
    unsigned int nPorts = midi->getPortCount();
    for (unsigned int i = 0; i < nPorts; ++i)
    {
        if (midi->getPortName(i).find(portName) != std::string::npos)
        {
            return i;
        }
    }
    return -1;
}

void waitForDevice(RtMidi *midi, const std::string &portName)
{
    int portNumber = -1;
    while ((portNumber = findPortByName(midi, portName)) == -1)
    {
        // std::cout << "Waiting for '" << portName << "' to connect..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Check every 5 seconds
    }
    midi->openPort(portNumber);
    std::cout << "'" << portName << "' connected." << std::endl;
}

void MidiConverter::openPort(const std::string &portName)
{
    waitForDevice(midiin, portName);
    std::cout << "MIDI in OK." << std::endl;
    waitForDevice(midiout, portName);
    std::cout << "MIDI out OK." << std::endl;
}

void MidiConverter::registerHandler(int programNumber, int noteNumber, MidiCallback callback)
{
    std::lock_guard<std::mutex> lock(mutex);
    handlers[{programNumber, noteNumber}].push_back(callback);
}

void MidiConverter::run()
{
    while (true)
    {
        std::vector<unsigned char> message;
        midiin->getMessage(&message);
        int nBytes = message.size();
        if (nBytes > 0)
        {

            std::cout << "Received MIDI data: ";
            for (int i = 0; i < nBytes; ++i)
            {
                std::cout << std::hex << +message[i] << " ";
            }
            std::cout << std::dec << std::endl;

            unsigned char status = message[0];

            // Detect program change messages and store it
            if ((status & 0xF0) == 0xC0)
            {
                int channel = (status & 0x0F) + 1;
                int programNumber = message[1];
                currentProgram[channel] = programNumber;
                std::cout << "Program Change received on channel " << channel
                          << " to program number " << programNumber << std::endl;
            }

            // std::cout << "status: " << std::hex << +status << std::endl;
            // note on
            // todo: consider channels
            if (message[0] == 0x99)
            {

                int channel = (status & 0x0F) + 1;
                int note = message[1];
                int program = currentProgram[channel];

                ProgramNoteKey key{program, note};

                // std::cout << "Program " << program << " Note " << note << std::endl;
                std::lock_guard<std::mutex> lock(mutex);
                if (handlers.find(key) != handlers.end())
                {

                    for (auto &handler : handlers[key])
                    {
                        handler(midiout);
                    }
                }
            }

            // pass through the original message
            if (passThrough)
            {
                midiout->sendMessage(&message);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
