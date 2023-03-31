#include <iostream>
#include <unordered_map>
using namespace std;

// jr


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

void parse_instruc(uint16_t instruc);



int main() {
    uint16_t regs[8];
    regs[0] = 0;
    uint16_t num = 96;

    // 001 000 001 000 1000
    // movi $1,8
    uint16_t an_inst = 8328;



    //func_addi(regs[1], regs[0], num);
    cout << regs[1] << endl;
}

// merge into main 

// will run for every instruction

// need to add break after every condition
void parse_instruc(uint16_t instruc){
    uint16_t regs[8];
    regs[0] = 0;
    uint16_t pc = 0;

    uint16_t opcode = instruc >> 13;
    
    // isolate registers
    uint16_t reg1 = instruc;
    uint16_t reg2 = instruc;
    uint16_t reg3 = instruc;
    
    reg1 = reg1 << 3;
    reg1 = reg1 >> 13;

    reg2 = reg2 << 3;
    reg2 = reg3 >> 13;

    reg3 = reg3 << 3;
    reg3 = reg3 >> 13;




    // 3 reg argument
    if(opcode == 0) {
        // isolate function code
        uint16_t func_code = instruc;
        func_code = func_code << 12;
        func_code = func_code >> 12;

        // the functions inside this should not execute
        // if reg3 is 0, $0 should remain 0 at all times
        if(reg3 != 0) {
            // add (3, 1, 2)
            if(func_code == 0) {
                func_add(regs[reg3], regs[reg1], regs[reg2]);
            }

            // sub
            if(func_code == 1) {
                func_sub(regs[reg3], regs[reg1], regs[reg2]);
            }

            // or 
            if (func_code == 2) {
                func_or(regs[reg3], regs[reg1], regs[reg2]);
            }

            // and
            if (func_code == 3) {
                func_and(regs[reg3], regs[reg1], regs[reg2]);
            }

            // slt
            if (func_code == 4) {
                func_slt(regs[reg3], regs[reg1], regs[reg2]);
            }
        }

        // jr
        if (func_code == 8) {
            regs[7] = pc + 1;
            pc = regs[reg1];
        }


    }

    // 2 reg args

    // isolate immediate (7-bit)
    uint16_t imm_7 = instruc;
    imm_7 = imm_7 << 9;
    imm_7 = imm_7 >> 9;

    // slti
    if (opcode == 7) {
        
    }

    // lw
    if (opcode == 4) {

    }

    // sw
    if (opcode == 5) {

    }

    // jeq
    if (opcode == 6) {

    }

    // addi 

    // [op, src, dst, 7-imm]
    // 001 000 001 000 1000
    if (opcode == 1) {
        func_addi(regs[reg2], regs[reg1], imm_7);
    }

    // no reg args

    // j
    if (opcode == 2) {

    }

    // jal
    if (opcode == 3) {

    }


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



