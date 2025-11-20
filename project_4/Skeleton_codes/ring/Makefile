CC     = mpicc
CFLAGS =

ring_sum: ring_sum.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	$(RM) ring_sum

clean_logs:
	$(RM) *.err *.out