CXX = g++
CXXFLAGS = -std=c++14 -Wall -g
OBJFILES = p1.o Terminal.o
TARGET = p1

all: $(TARGET)

$(TARGET):  $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJFILES)

p1.o:  $(TARGET).cpp $(TARGET).h Terminal.o
	$(CXX) $(CXXFLAGS) -c $(TARGET).cpp

Terminal.o: Terminal.cpp Terminal.h
	$(CXX) $(CXXFLAGS) -c Terminal.cpp
clean:
	rm -f $(OBJFILES) $(TARGET)

