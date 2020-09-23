CXX = g++
CXXFLAGS = -std=c++14 -Wall -g
OBJFILES = p2.o Terminal.o Process.o
TARGET = p2

all: $(TARGET)

$(TARGET):  $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJFILES)

$(TARGET).o:  $(TARGET).cpp $(TARGET).h Terminal.o
	$(CXX) $(CXXFLAGS) -c $(TARGET).cpp

Terminal.o: Terminal.cpp Terminal.h Process.o
	$(CXX) $(CXXFLAGS) -c Terminal.cpp

Process.o: Process.cpp Process.h
	$(CXX) $(CXXFLAGS) -c Process.cpp
clean:
	rm -f $(OBJFILES) $(TARGET)

