CC=mpic++

CPPFLAGS= -O3
LDFLAGS= 

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)

OBJECTS=$(SOURCES:.cpp=.o)

INCLD=
LIB= 

TARGET=main.exe

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(OBJECTS) -o $@  $(INCLD) $(LIB) 

%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@  $(INCLD)

