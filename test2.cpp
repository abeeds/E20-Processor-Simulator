#include <iostream>
#include <unordered_map>
using namespace std;

// functions to make
/*
    * flip off first 3 bits of PC

*/

// 3 reg args ---------------------------------------------
void func_add(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_sub(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_or(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_and(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_slt(uint16_t &dst, uint16_t srca, uint16_t srcb);


// 2 reg args 

void func_slti(uint16_t &dst, uint16_t src, uint16_t imm);

uint16_t func_jeq(uint16_t srca, uint16_t srcb, uint16_t imm);

void func_addi(uint16_t &dst, uint16_t src, uint16_t imm);

void parse_instruc(uint16_t mem[], uint16_t registers[], uint16_t &progC);

// used to determine if program should keep running
bool is_halt(uint16_t mem[], uint16_t regs[], uint16_t pc);


int main() {
    uint16_t regs[8];
    regs[0] = 0;
    uint16_t pc = 0;
    uint16_t num = 96;

    // 010 000 000 000 0011
    uint16_t mem[8192];
    


    mem[0] = 8328;
    mem[1] = 8449;
    mem[2] = 8449;
    mem[3] = 16387;

    parse_instruc(mem, regs, pc);
    parse_instruc(mem, regs, pc);
    parse_instruc(mem, regs, pc);
    parse_instruc(mem, regs, pc);
    //parse_instruc(mem[2], regs, pc);


    cout << "reg 1: "<< regs[1] << endl;
    cout << "reg 2: "<< regs[2] << endl;
    cout << "reg 3: "<< regs[3] << endl;
    cout << "pc: " << pc << endl;
}

// merge into main 
// will run for every instruction
// need to add break after every condition
void parse_instruc(uint16_t mem[], uint16_t registers[], uint16_t &progC){
    uint16_t instruc = mem[progC];
    uint16_t opcode = instruc >> 13;
    
    // isolate registers
    uint16_t reg1 = instruc;
    uint16_t reg2 = instruc;
    uint16_t reg3 = instruc;
    
    reg1 = reg1 << 3;
    reg1 = reg1 >> 13;

    reg2 = reg2 << 6;
    reg2 = reg2 >> 13;

    reg3 = reg3 << 9;
    reg3 = reg3 >> 13;




    // 3 reg argument
    if(opcode == 0) {
        // isolate function code
        uint16_t func_code = instruc;
        func_code = func_code << 12;
        func_code = func_code >> 12;

        // the functions inside this should not execute
        // if reg3/regDST is 0, $0 should remain 0 at all times
        if(reg3 != 0) {
            // add (3, 1, 2)
            if(func_code == 0) {
                func_add(registers[reg3], registers[reg1], registers[reg2]);
                progC += 1;
                return;
            }

            // sub
            if(func_code == 1) {
                func_sub(registers[reg3], registers[reg1], registers[reg2]);
                progC += 1;
                return;
            }

            // or 
            if (func_code == 2) {
                func_or(registers[reg3], registers[reg1], registers[reg2]);
                progC += 1;
                return;
            }

            // and
            if (func_code == 3) {
                func_and(registers[reg3], registers[reg1], registers[reg2]);
                progC += 1;
                return;
            }

            // slt
            if (func_code == 4) {
                func_slt(registers[reg3], registers[reg1], registers[reg2]);
                progC += 1;
                return;
            }
        }

        // jr
        if (func_code == 8) {
            registers[7] = progC + 1;
            progC = registers[reg1];
            return;
        }


    }

    // 2 reg args

    // isolate immediate (7-bit)
    uint16_t imm_7 = instruc;
    imm_7 = imm_7 << 9;
    imm_7 = imm_7 >> 9;

    // slti
    if (opcode == 7) {
        func_slti(registers[reg2], registers[reg1], imm_7);
        progC += 1;
        return;
    }

    // lw
    if (opcode == 4) {
        progC += 1;

        // do nothing if regDST is 0
        if(reg2 == 0) {
            return;
        }

        registers[reg2] = mem[imm_7 + registers[reg1]];
        return;
    }

    // sw
    if (opcode == 5) {
        mem[imm_7 + registers[reg1]] = registers[reg2];

        progC += 1;
    }

    // jeq
    if (opcode == 6) {
        if(registers[reg1] == registers[reg2]) {
            progC += 1 + imm_7;
            return;
        }
        progC += 1;
        return;
    }

    // addi 
    if (opcode == 1) {
        progC += 1;

        // do nothing if regDST is 0
        if(reg2 == 0){
            return;
        }
        func_addi(registers[reg2], registers[reg1], imm_7);
        return;
    }

    // no reg args

    // isolate immediate (13-bit)
    uint16_t imm_13 = instruc;
    imm_13 = imm_13 << 3;
    imm_13 = imm_13 >> 3;

    // j
    if (opcode == 2) {
        progC = imm_13;
        return;
    }

    // jal
    // 0101111000000000
    if (opcode == 3) {
        registers[7] = progC + 1;
        progC = imm_13;
        return;
    }
} // parse_instruc



bool is_halt(uint16_t mem[], uint16_t regs[], uint16_t pc) {
    uint16_t instruction = mem[pc];

    uint16_t opcode = instruction;
    opcode = opcode >> 13;
    
    uint16_t func_code = instruction;
    func_code = func_code << 12;
    func_code = func_code >> 12;


    // halt by jr
    if(opcode == 0 && func_code == 8) {
        uint16_t regSRC = instruction;
        regSRC = regSRC << 3;
        regSRC = regSRC >> 13;

        return regs[regSRC] == pc; 
    }


    // halt by jeq
    if(opcode == 6) {
        uint16_t reg1, reg2, imm;
        reg1 = instruction << 3;
        reg1 = reg1 >> 13;

        reg2 = instruction << 6;
        reg2 = reg2 >> 13;

        imm = instruction << 9;
        imm = imm >> 9;


        return regs[reg1] == regs[reg2] && imm == 0;
    }


    // halt by j or jal
    if(opcode == 2 || opcode == 3) {
        uint16_t imm = instruction;
        imm = imm << 3;
        imm = imm >> 3;

        return imm == pc;
    }

    return false; // any remaining cases;
}

void func_add(uint16_t &dst, uint16_t srca, uint16_t srcb) {
    dst = srca + srcb;
}

void func_sub(uint16_t &dst, uint16_t srca, uint16_t srcb){
    dst = srca - srcb;
}

void func_or(uint16_t &dst, uint16_t srca, uint16_t srcb){
    dst = srca ^ srcb;
}

void func_and(uint16_t &dst, uint16_t srca, uint16_t srcb) {
    dst = srca & srcb;
}

void func_slt(uint16_t &dst, uint16_t srca, uint16_t srcb) {
    if(srca < srcb){
        dst = 1;
        return;
    }
    dst = 0;
}

void func_slti(uint16_t &dst, uint16_t src, uint16_t imm) {
    if(src < imm){
        dst = 1;
        return;
    }
    dst = 0;
}   

void func_addi(uint16_t &dst, uint16_t src, uint16_t imm) {
    dst = src + imm;
}



