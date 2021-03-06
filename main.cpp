#include <stdio.h>
#include <stdlib.h>

// Rodrigo Casale
// http://www.obelisk.me.uk/6502/


using Byte = unsigned char;     //  8 Bit
using Word = unsigned short;    // 16 Bit

using u32 = unsigned int;       // 32 Bit

// Memory Structure
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

    /** Write 2 bytes */
    void writeWord( Word value, u32 addr, u32& cycles) {
        data[addr]   = value & 0xFF;
        data[addr+1] = (value >> 8);
        cycles -= 2;
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

    Byte fetchWord (u32& cycles, Mem& memory) {
        // 6502 is little endian
        Word data = memory[PC];
        PC++;

        data |= memory[PC] << 8;
        PC++;

        cycles += 2;


        return data;
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
    static constexpr Byte INS_LDA_ZPX = 0xB5;
    static constexpr Byte INS_JSR = 0x20;
    

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
                    printf("Intruction LDA Immediate \n");
                    printf("A = %x \n", A);
                } 
                break;
                case INS_LDA_ZP :{
                    Byte zeroPageAddr = fetchByte(cycles, memory);
                    A = readByte( cycles, zeroPageAddr, memory);
                    LDASetStatus();
                    printf("Intruction LDA Zero Page \n");
                    printf("A = %x \n", A);
                } break;
                case INS_LDA_ZPX :{
                    Byte zeroPageAddr = fetchByte(cycles, memory);
                    zeroPageAddr += X;
                    cycles--;
                    A = readByte( cycles, zeroPageAddr, memory);
                    LDASetStatus();
                    printf("Intruction LDA Zero Page X \n");
                    printf("A = %x \n", A);
                } 
                break;
                case INS_JSR :{
                    Word subAddr = fetchWord(cycles, memory);
                    memory.writeWord(PC - 1, SP, cycles);
                    PC = subAddr;
                    cycles--;
                    printf("Intruction Jump to Subroutine \n");
                } break;
                default: {
                    cycles--;
                    // printf("Instruction not handled %x \n", instr);
                } break;
            }
        }
    }
};

int main() {

    Mem mem;
    CPU cpu;
    cpu.reset( mem );

    // Hard coded test
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    cpu.exec( 9, mem );
    return 0;
}
