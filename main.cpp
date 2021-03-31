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

    /** Read 1 byte */
    Byte operator[] (u32 address ) const {
        // assert here address is < MAX_MEM
        return data[address];
    } 

    /** Write 1 byte */
    Byte& operator[] (u32 address )  {
        // assert here address is < MAX_MEM
        return data[address];
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

    Byte fetchByte (u32& cycles, Mem& memory) {
        Byte data = memory[PC];
        PC++;
        cycles--;
        return data;
    }

    Byte readByte (u32& cycles, Byte address, Mem& memory) {
        Byte data = memory[address];
        cycles--;
        return data;
    }
    // Opcodes
    static constexpr Byte INS_LDA_IM = 0xA9;
    static constexpr Byte INS_LDA_ZP = 0xA5;

    void LDASetStatus() {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    void exec( u32 cycles, Mem& memory ) {
        while (cycles > 0) {
            Byte instr = fetchByte(cycles, memory);

            switch (instr){
                case INS_LDA_IM :{
                    Byte val = fetchByte (cycles, memory);
                    A = val;
                    LDASetStatus();
                    printf("RAN \n");
                } 
                break;
                case INS_LDA_ZP :{
                    Byte zeroPageAddr = fetchByte (cycles, memory);
                    A = readByte( cycles, zeroPageAddr, memory);

                } 
                break;
                default: {
                    cycles--;
                    printf("Instruction not handled %x \n", instr);
                } break;
            }
        }
    }
};

int main() {

    Mem mem;
    CPU cpu;
    cpu.reset( mem );
    mem[0xFFFC] = CPU::INS_LDA_ZP;
    mem[0xFFFD] = 0x42;
    mem[0x0042] = 0x84;
    cpu.exec( 3, mem );
    return 0;
}
