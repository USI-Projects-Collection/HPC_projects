"""
HPC Class: Example of NUMBA
Author: Aryan Eftekhari <aryan.eftkehari@gmail.com>

Description:
This program demonstrates the usage of Numba, a Just-In-Time (JIT) compiler that translates
Python functions to optimized machine code at runtime. It showcases how Numba can enhance
performance by allowing for parallelized operations in Python code.

Installation Instructions:
Install Numba with the following command:
$ pip3.11 install numba

Run Instructions:
Allocate the resources: salloc --nodes=1 --exclusive --reservation=hpc-tuesday
To execute the program, use the following command:
$ python3.11 main.py

TODO:
1) Comment on what the program is doing and the degree of parallelization achieved.
"""

from numba import jit,prange
import random
import time

# Base function
def monte_carlo_pi(nsamples):
    acc = 0
    for i in range(nsamples):
        x = random.random()
        y = random.random()
        if (x ** 2 + y ** 2) < 1.0:
            acc += 1
    return 4.0 * acc / nsamples

# Just-in-time compilation of this function
@jit
def jit_monte_carlo_pi(nsamples):
    acc = 0
    for i in range(nsamples):
        x = random.random()
        y = random.random()
        if (x ** 2 + y ** 2) < 1.0:
            acc += 1
    return 4.0 * acc / nsamples

# Just-in-time compilation which is parallelized
@jit(parallel=True)
def prl_jit_monte_carlo_pi(nsamples):
    acc = 0
    for i in prange(nsamples):
        x = random.random()
        y = random.random()
        if (x ** 2 + y ** 2) < 1.0:
            acc += 1
    return 4.0 * acc / nsamples

t=time.time()
val = monte_carlo_pi(50000000)
print('base time=',time.time()-t,' value=',val)

t=time.time()
val = jit_monte_carlo_pi(50000000)
print('jit time=',time.time()-t,' value=',val)

t=time.time()
val = prl_jit_monte_carlo_pi(50000000)
print('prl time=',time.time()-t,' value=',val)