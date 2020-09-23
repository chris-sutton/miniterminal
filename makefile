CXX = g++
CXXFLAGS = -std=c++14 -Wall -g
OBJFILES = p2.o Terminal.o signalHandlers.o
TARGET = p2

all: $(TARGET)

$(TARGET):  $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJFILES)

$(TARGET).o:  $(TARGET).cpp $(TARGET).h Terminal.o
	$(CXX) $(CXXFLAGS) -c $(TARGET).cpp

Terminal.o: Terminal.cpp Terminal.h signalHandlers.o
	$(CXX) $(CXXFLAGS) -c Terminal.cpp

signalHandlers.o: signalHandlers.cpp signalHandlers.h
	$(CXX) $(CXXFLAGS) -c signalHandlers.cpp

clean:
	rm -f $(OBJFILES) $(TARGET)

