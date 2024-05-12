# Define the compiler
CXX=g++

# Compiler flags
CXXFLAGS=-Wall -std=c++11

# Linker flags
LDFLAGS=-lrtmidi -lpthread -Wl,-rpath,'/usr/local/lib',-rpath,'./rtmidi/build'

# source files
SOURCES=MidiConverter.cpp MidiHandler.cpp main.cpp

# executables
TARGET=lighting-controller

OBJECTS=$(SOURCES:.cpp=.o)

# Default target
all: clean $(TARGET)

# compile the main program
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

# Rule to install RtMidi (run this if RtMidi is not installed)
install-rtmidi:
	git clone https://github.com/thestk/rtmidi
	cd rtmidi && mkdir build && cd build && cmake .. && make && sudo make install

install-service:
	cp $(TARGET) /usr/local/bin
	cp controller.service /etc/systemd/system/lighting-controller.service
	systemctl daemon-reload
	systemctl enable lighting-controller.service
	systemctl start lighting-controller.service
