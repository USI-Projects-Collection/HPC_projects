
CC = gcc

# Common optimization flags (no OpenMP qui)
OPT     = -O3 -march=native -funroll-loops -ftree-vectorize
CFLAGS  = -Wall -std=gnu99 $(OPT) -I"${MKLROOT}/include"

# OpenMP flags separati (solo per la versione OMP)
OMPFLAGS = -fopenmp

# MKL libraries
LDLIBS = -L${MKLROOT}/lib/intel64 \
          -lmkl_intel_lp64         \
          -lmkl_sequential         \
          -lmkl_core               \
          -lpthread -lm -ldl

# ============================================================================ #
# Targets / Objects / Results

targets = benchmark-naive benchmark-blocked benchmark-blas benchmark-blocked-omp

objects = benchmark.o \
          dgemm-naive.o \
          dgemm-blocked.o \
          dgemm-blas.o

# Oggetto per la versione OpenMP
objects_omp = dgemm-blocked-omp.o

results = timing_basic_dgemm.data   \
          timing_blas_dgemm.data    \
          timing_blocked_dgemm.data \
          timing_blocked_omp_dgemm.data \
          timing.pdf

# ============================================================================ #
# Default target

.PHONY: default
default: all

.PHONY: all
all: $(targets)

# ============================================================================ #
# Build rules

benchmark-naive: benchmark.o dgemm-naive.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

benchmark-blocked: benchmark.o dgemm-blocked.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

benchmark-blas: benchmark.o dgemm-blas.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

benchmark-blocked-omp: benchmark.o $(objects_omp)
	$(CC) $(CFLAGS) $(OMPFLAGS) -o $@ $^ $(LDLIBS) $(OMPFLAGS)

# ============================================================================ #
# Compilation rules

%.o: %.c
	$(CC) -c $(CFLAGS) $<

# Regola specifica per oggetti OMP
dgemm-blocked-omp.o: dgemm-blocked-omp.c
	$(CC) -c $(CFLAGS) $(OMPFLAGS) $<

# ============================================================================ #
# Cleaning

.PHONY: clean
clean:
	$(RM) $(targets) $(objects) $(objects_omp)

.PHONY: cleanall
cleanall: clean
	$(RM) $(results)
