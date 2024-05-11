# Define the compiler
CXX=g++

# Compiler flags
CXXFLAGS=-Wall -lrtmidi -lpthread -std=c++11

# Define source files
SRC1=controller.cpp
SRC2=monitor.cpp

# Define the executables output
TARGET1=controller
TARGET2=monitor

# Default target
all: $(TARGET1) $(TARGET2)

# Rule to link the main program
$(TARGET1): $(SRC1)
	$(CXX) $(SRC1) -o $(TARGET1) $(CXXFLAGS)

# Rule to link the monitor program
$(TARGET2): $(SRC2)
	$(CXX) $(SRC2) -o $(TARGET2) $(CXXFLAGS)

# Rule to clean old builds
clean:
	rm -f $(TARGET1) $(TARGET2)

# Rule to install RtMidi (run this if RtMidi is not installed)
install-rtmidi:
	git clone https://github.com/thestk/rtmidi
	cd rtmidi && mkdir build && cd build && cmake .. && make && sudo make install
