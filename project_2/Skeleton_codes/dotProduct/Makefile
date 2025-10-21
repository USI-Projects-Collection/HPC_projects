CXX ?= g++
CXXFLAGS ?= -O3 -fopenmp
N ?= 100000

all: dotProduct

dotProduct: dotProduct.cpp walltime.h
	$(CXX) $(CXXFLAGS) -DN=$(N) $< -o $@
	chmod +x $@
clean:
	rm -rf dotProduct
