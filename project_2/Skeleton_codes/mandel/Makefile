CC     ?= gcc
CFLAGS  = -O3 -fopenmp # -I$(CPATH)
LDFLAGS = -O3 -fopenmp # -L$(LIBRARY_PATH)

IMAGE_WIDTH  ?= 4096
IMAGE_HEIGHT ?= 4096

all: mandel_seq

mandel_seq: mandel_seq.o pngwriter.o walltime.o
	$(CC) $(LDFLAGS) $^ -lpng -o $@

mandel_seq.o: mandel_seq.c consts.h pngwriter.h walltime.h
	$(CC) -c $(CFLAGS) -DIMAGE_WIDTH=$(IMAGE_WIDTH) -DIMAGE_HEIGHT=$(IMAGE_HEIGHT) $<

pngwriter.o: pngwriter.c pngwriter.h
	$(CC) -c $(CFLAGS) $<

walltime.o: walltime.c walltime.h
	$(CC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) *.o mandel_seq
