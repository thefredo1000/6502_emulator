#include <stdio.h>
#include <stdlib.h>

// Rodrigo Casale
// http://www.obelisk.me.uk/6502/


using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte data[MAX_MEM];

    void init() {
        for ( u32 i = 0; i < MAX_MEM; i++) {
            data[i] = 0;
        }
    }

};

struct CPU {

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

    void reset( Mem& memory ) {
        PC = 0xFFFC;
        SP = 0x0100;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;

        memory.init();
    }

    void exec( u32 cycles, Mem& memory ) {
        
    }
};

int main() {

    Mem mem;
    CPU cpu;
    cpu.reset(mem);
    cpu.exec( 2, mem );
    return 0;
}
