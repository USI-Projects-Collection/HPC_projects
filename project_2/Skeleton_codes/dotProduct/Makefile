CPP ?= g++
CXXFLAGS ?= -O3 -fopenmp
N ?= 100000

all: dotProduct

dotProduct: dotProduct.cpp walltime.h
	$(CPP) $(CXXFLAGS) -DN=$(N) $< -o $@

clean:
	rm -rf dotProduct
