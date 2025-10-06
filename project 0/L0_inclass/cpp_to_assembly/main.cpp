/*
HPC Class: C++/C to Assembly Example
Author: Aryan Eftekhari <aryan.eftekhari@gmail.com>

INFO:
- Illustrates the translation between C++ code and assembly.
- Compile C++ to executable: g++ -o main.exe main.cpp
- Compile C++ to assembly: g++ -S main.cpp
- Compile assembly to executable: g++ main.s -o main_as.exe

TODO:
1. In the assembly code, change "addl" to "imull", compile, and run. Observe the result.
2. Keep "addl" in the assembly code but set a = 2147483647 and b = 1, compile, and run. Analyze the output.

NOTE:
- On most systems "int" is 4 bytes (32 bits), allowing representation of 2^32 distinct values.
- Range for unsigned int: [0, 2^32 - 1]
- Range for signed int: [-2^31, 2^31 - 1]
*/

#include <iostream>
int main() {

    int a = 7;
    int b = 93;
    int c = a + b;

    std::cout << "c=" << c << std::endl;

    return c;
}