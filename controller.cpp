#include <iostream>
#include <fstream>
#include <cstdlib>
#include <csignal>
#include <chrono>
#include <thread>
#include <vector>
#include <iomanip>
#include <rtmidi/RtMidi.h>

bool done = false;
const std::string MIDI_PORT_NAME = "UM-ONE";
std::ofstream logFile("midi_log.txt", std::ios::out | std::ios::app);

static void finish(int sig) {
    done = true;
    logFile.close();
}

void sendCC(RtMidiOut* midiout, unsigned char channel, unsigned char ccNumber, unsigned char value) {
    std::vector<unsigned char> message = {static_cast<unsigned char>(0xB0 + channel), ccNumber, value};
    midiout->sendMessage(&message);
    std::cout << "Sending CC: ";
    logFile << "Sending CC: ";
    for (auto byte : message) {
        std::cout << std::hex << +byte << " ";
        logFile << std::hex << +byte << " ";
    }
    std::cout << std::dec << std::endl;
    logFile << std::dec << std::endl;
}

void handleNote60FadeIn(RtMidiOut* midiout) {
    for (int i = 0; i <= 127; i += 5) {
        sendCC(midiout, 0, 7, i);  // Channel 1, CC#7 (Volume), Increasing Value
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void handleNote61FadeOut(RtMidiOut* midiout) {
    for (int i = 127; i >= 0; i -= 5) {
        sendCC(midiout, 0, 7, i);  // Channel 1, CC#7 (Volume), Decreasing Value
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int findPortByName(RtMidi* midi, const std::string& portName) {
    unsigned int nPorts = midi->getPortCount();
    for (unsigned int i = 0; i < nPorts; ++i) {
        if (midi->getPortName(i).find(portName) != std::string::npos) {
            return i;
        }
    }
    return -1;
}

void waitForDevice(RtMidi* midi, const std::string& portName) {
    int portNumber = -1;
    while ((portNumber = findPortByName(midi, portName)) == -1) {
        std::cout << "Waiting for '" << portName << "' to connect..." << std::endl;
        logFile << "Waiting for '" << portName << "' to connect..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Check every 5 seconds
    }
    midi->openPort(portNumber);
    std::cout << "'" << portName << "' connected." << std::endl;
    logFile << "'" << portName << "' connected." << std::endl;
}

int main() {
    RtMidiIn* midiin = nullptr;
    RtMidiOut* midiout = nullptr;

    try {
        midiin = new RtMidiIn();
        midiout = new RtMidiOut();
    } catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }

    std::signal(SIGINT, finish);

    waitForDevice(midiin, MIDI_PORT_NAME);  // Open UM-ONE port for input
    waitForDevice(midiout, MIDI_PORT_NAME); // Open UM-ONE port for output

    midiin->ignoreTypes(false, false, false);

    while (!done) {
        std::vector<unsigned char> message;
        midiin->getMessage(&message);
        int nBytes = message.size();
        if (nBytes > 0) {
            std::cout << "Received MIDI data: ";
            logFile << "Received MIDI data: ";
            for (int i = 0; i < nBytes; ++i) {
                std::cout << std::hex << +message[i] << " ";
                logFile << std::hex << +message[i] << " ";
            }
            std::cout << std::dec << std::endl;
            logFile << std::dec << std::endl;

            if (message[0] == 0x90) {  // Note On
                if (message[1] == 60) {
                    handleNote60FadeIn(midiout);
                } else if (message[1] == 61) {
                    handleNote61FadeOut(midiout);
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    delete midiin;
    delete midiout;

    return 0;
}
