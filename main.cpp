#include <stdio.h>
#include <stdlib.h>

// Rodrigo Casale
// http://www.obelisk.me.uk/6502/


struct CPU {
    using Byte = unsigned char;
    using Word = unsigned short;

    Word PC;    // Program Counter
    Word SP;    // Stack Pointer

    Byte A, X, Y;   // Registers

    Byte C : 1; // Status Flag
    Byte Z : 1; // Status Flag
    Byte I : 1; // Status Flag
    Byte D : 1; // Status Flag
    Byte B : 1; // Status Flag
    Byte V : 1; // Status Flag
    Byte N : 1; // Status Flag

};

int main() {

    CPU cpu;
    return 0;
}
