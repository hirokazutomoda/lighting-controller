#include <iostream>
#include <cstdlib>
#include <rtmidi/RtMidi.h>

bool done;
static void finish(int sig) { done = true; }

int main() {
    RtMidiIn  *midiin = 0;
    RtMidiOut *midiout = 0;
    std::vector<unsigned char> message;
    int noteNumber = 60; // MIDI note number to trigger fade-in
    int ccValue = 0;     // Starting value for CC
    int triggerCount = 0; // Number of times the trigger note has been caught
    bool passThrough = true; // Configure if input should be passed through

    try {
        midiin = new RtMidiIn();
        midiout = new RtMidiOut();
    }
    catch (RtMidiError &error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }

    // Check available ports.
    unsigned int nPorts = midiin->getPortCount();
    if (nPorts == 0) {
        std::cout << "No input ports available!" << std::endl;
        exit(EXIT_FAILURE);
    }

    midiin->openPort(0);
    midiout->openPort(0); // Modify if your output port is different

    // Don't ignore sysex, timing, or active sensing messages.
    midiin->ignoreTypes(false, false, false);

    done = false;
    (void) signal(SIGINT, finish);

    // Main loop
    while (!done) {
        midiin->getMessage(&message);
        int nBytes = message.size();
        if (nBytes > 0) {
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
                        usleep(50000); // 50ms delay between CC messages
                    }
                    triggerCount++;
                }
            }

            // Optionally pass through the original message
            if (passThrough) {
                midiout->sendMessage(&message);
            }
        }

        // Sleep for 10ms before the next loop iteration
        usleep(10000);
    }

    delete midiin;
    delete midiout;

    return 0;
}

