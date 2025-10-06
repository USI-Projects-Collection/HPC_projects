CC=g++

TARGET = main.exe

CPPFLAGS = -g 
LDFLAGS =

INCLD = -I${MKLROOT}/include
LIB   = -L${MKLROOT}/lib -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)
# Linking
# $@ is the output name of this rule, i.e. the final executable (main.exe here).
$(TARGET) : $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIB)

# Compiling 
# Every .cpp file matches with a .o file (see SOURCES and OBJECTS).
# $@ is the object file being built (e.g. test_mkl.o).
# $< is the source file that triggered this rule (e.g. test_mkl.cpp).
%.o : %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ $(INCLD)

clean:
	rm -f $(OBJECTS) $(TARGET)