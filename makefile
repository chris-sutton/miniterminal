CXX = g++
CXXFLAGS = -std=c++14 -Wall -g
OBJFILES = p2.o Terminal.o
TARGET = p2

all: $(TARGET)

$(TARGET):  $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJFILES)

$(TARGET).o:  $(TARGET).cpp $(TARGET).h Terminal.o
	$(CXX) $(CXXFLAGS) -c $(TARGET).cpp

Terminal.o: Terminal.cpp Terminal.h
	$(CXX) $(CXXFLAGS) -c Terminal.cpp
clean:
	rm -f $(OBJFILES) $(TARGET)

