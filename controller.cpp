#include <iostream>
#include <cstdlib>
#include <csignal>
#include <chrono>
#include <thread>
#include <map>
#include <vector>

#include <rtmidi/RtMidi.h>

bool done = false;
const std::string MIDI_PORT_NAME = "UM-ONE";
static void finish(int sig) { done = true; }

int findPortByName(RtMidiIn *midiin, const std::string &portName)
{
    unsigned int nPorts = midiin->getPortCount();
    for (unsigned int i = 0; i < nPorts; ++i)
    {
        // std::cout << midiin->getPortName(i) << std::endl;
        if (midiin->getPortName(i).find(portName) != std::string::npos)
        {
            return i;
        }
    }
    return -1;
}

void waitForDevice(RtMidiIn *midiin, const std::string &portName)
{
    int portNumber = -1;
    while ((portNumber = findPortByName(midiin, portName)) == -1)
    {
        std::cout << "Waiting for '" << portName << "' to connect..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    midiin->openPort(portNumber);
    std::cout << "'" << portName << "' connected." << std::endl;
}

int main()
{
    RtMidiIn *midiin = nullptr;
    RtMidiOut *midiout = nullptr;
    int noteNumber = 60; // MIDI note number to trigger fade-in
    int ccValue = 0;     // Starting value for CC
    int triggerCount = 0; // Number of times the trigger note has been caught
    bool passThrough = true; // Configure if input should be passed through

    try
    {
        midiin = new RtMidiIn();
        midiout = new RtMidiOut();
    }
    catch (RtMidiError &error)
    {
        error.printMessage();
        exit(EXIT_FAILURE);
    }

    std::signal(SIGINT, finish);
    waitForDevice(midiin, MIDI_PORT_NAME);

    midiout->openVirtualPort("Controller Output"); // Open a virtual port
    
    midiin->ignoreTypes(false, false, false); // Don't ignore any messages

    // Main loop
    while (!done)
    {
        std::vector<unsigned char> message;
        midiin->getMessage(&message);
        int nBytes = message.size();
        if (nBytes > 0)
        {
            // Example to process received messages
            // Process more complex pattern matching and output generation here
            std::cout << "Received MIDI data: ";
            for (int i = 0; i < nBytes; ++i)
            {
                std::cout << static_cast<int>(message[i]) << " ";
            }
            std::cout << std::endl;
                        if (message[0] == 0x90 && message[1] == noteNumber && message[2] > 0) { // Note On
                if (triggerCount == 0) {
                    // Start sending CC messages for fade-in
                    while (ccValue <= 127) {
                        std::vector<unsigned char> ccMessage;
                        ccMessage.push_back(0xB0); // CC on channel 1
                        ccMessage.push_back(7);    // Volume CC number
                        ccMessage.push_back(ccValue);
                        midiout->sendMessage(&ccMessage);
                        ccValue += 5; // Increment the CC value
                        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 50ms delay between CC messages
                    }
                    triggerCount++;
                }
            }

            // Optionally pass through the original message
            if (passThrough) {
                midiout->sendMessage(&message);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep to reduce CPU usage
    }

    delete midiin;
    delete midiout;

    return 0;
}
