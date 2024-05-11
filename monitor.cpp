#include <iostream>
#include <sstream>
#include <rtmidi/RtMidi.h>

bool chooseMidiPort(RtMidi *rtmidi) {
    std::cout << "Available MIDI ports:\n";
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if (nPorts == 0) {
        std::cout << "No ports available!\n";
        return false;
    }

    for (i = 0; i < nPorts; i++) {
        std::cout << i << ": " << rtmidi->getPortName(i) << '\n';
    }

    std::cout << "Choose port number: ";
    unsigned int portNum;
    std::cin >> portNum;
    if (portNum >= nPorts) {
        std::cout << "Invalid port number\n";
        return false;
    }

    rtmidi->openPort(portNum);
    return true;
}

int main() {
    RtMidiIn *midiin = new RtMidiIn();
    RtMidiOut *midiout = new RtMidiOut();

    if (!chooseMidiPort(midiin) || !chooseMidiPort(midiout)) {
        delete midiin;
        delete midiout;
        return 1;
    }

    midiin->ignoreTypes(false, false, false);

    std::vector<unsigned char> message;
    std::string input;

    while (true) {
        std::cout << "Enter hex message to send, or 'wait' to display incoming messages: ";
        std::cin >> input;
        if (input == "wait") {
            message.clear();
            midiin->getMessage(&message);
            int nBytes = message.size();
            if (nBytes > 0) {
                std::cout << "Received message: ";
                for (int i = 0; i < nBytes; i++)
                    std::cout << std::hex << +message[i] << " ";
                std::cout << std::dec << '\n';
            }
        } else {
            message.clear();
            for (size_t i = 0; i < input.length(); i += 2) {
                unsigned int byte;
                std::stringstream ss;
                ss << std::hex << input.substr(i, 2);
                ss >> byte;
                message.push_back(static_cast<unsigned char>(byte));
            }
            midiout->sendMessage(&message);
        }
    }

    delete midiin;
    delete midiout;
    return 0;
}

