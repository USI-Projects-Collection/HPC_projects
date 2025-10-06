CC = gcc
CFLAGS = -O3 -march=native -DSTREAM_TYPE=double -DNTIMES=20 

all: stream_c_1

stream_c_1: stream.c
	${CC} ${CFLAGS} -DSTREAM_ARRAY_SIZE=128000000 stream.c -o stream_c_1

.PHONY: clean
clean:
	rm -f stream_c_1
