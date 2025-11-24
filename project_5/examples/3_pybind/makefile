CC=g++ -fopenmp 

CPPFLAGS= -O3 -Wall -std=c++11 -fPIC
LDFLAGS= -shared -Wl,-undefined,dynamic_lookup

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)

OBJECTS=$(SOURCES:.cpp=.o)

INCLD= $(shell python3.11 -m pybind11 --includes)
LIB=

TARGET=MOD_NAME$(shell python3.11-config --extension-suffix)

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(OBJECTS) -o $@  $(INCLD) $(LIB) 

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@  $(INCLD)

