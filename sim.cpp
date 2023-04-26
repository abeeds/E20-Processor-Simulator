#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <regex>
#include <cstdlib>

using namespace std;

// Some helpful constant values that we'll be using.
size_t const static NUM_REGS = 8; 
size_t const static MEM_SIZE = 1<<13;
size_t const static REG_SIZE = 1<<16;


void load_machine_code(ifstream &f, uint16_t mem[]);

void print_state(uint16_t pc, uint16_t regs[], uint16_t memory[], size_t memquantity);

// 3 reg args ---------------------------------------------
void func_add(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_sub(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_or(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_and(uint16_t &dst, uint16_t srca, uint16_t srcb);

void func_slt(uint16_t &dst, uint16_t srca, uint16_t srcb);


// 2 reg args ---------------------------------------------

void func_slti(uint16_t &dst, uint16_t src, uint16_t imm);

void func_addi(uint16_t &dst, uint16_t src, uint16_t imm);


// reads each instruction and executes the correct command
void parse_instruc(uint16_t mem[], uint16_t registers[], uint16_t &progC);

// used to determine if program should keep running
bool is_halt(uint16_t mem[], uint16_t regs[], uint16_t pc);

uint16_t sign_extend(uint16_t num);


int main(int argc, char *argv[]) {
    /*
        Parse the command-line arguments
    */
   /**/
    char *filename = nullptr;
    bool do_help = false;
    bool arg_error = false;
    for (int i=1; i<argc; i++) {
        string arg(argv[i]);
        if (arg.rfind("-",0)==0) {
            if (arg== "-h" || arg == "--help")
                do_help = true;
            else
                arg_error = true;
        } else {
            if (filename == nullptr)
                filename = argv[i];
            else
                arg_error = true;
        }
    }
    // Display error message if appropriate 
    if (arg_error || do_help || filename == nullptr) {
        cerr << "usage " << argv[0] << " [-h] filename" << endl << endl; 
        cerr << "Simulate E20 machine" << endl << endl;
        cerr << "positional arguments:" << endl;
        cerr << "  filename    The file containing machine code, typically with .bin suffix" << endl<<endl;
        cerr << "optional arguments:"<<endl;
        cerr << "  -h, --help  show this help message and exit"<<endl;
        return 1;
    }

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Can't open file "<<filename<<endl;
        return 1;
    }

    // -------------------------------------------------
    //              Variable Assignments
    uint16_t regs[8] = {0};
    uint16_t memory[8192] = {0};
    uint16_t pc = 0;
    bool keepGoing = true;
    

    // -------------------------------------------------
    //          Store Instructions in Memory

    load_machine_code(file, memory);


    // -------------------------------------------------
    //                   Simulation
    while(keepGoing){
        keepGoing = !is_halt(memory, regs, (pc & 8191));
        parse_instruc(memory, regs, pc);
    }

    // -------------------------------------------------
    // TODO: your code here. print the final state of the simulator before ending, using print_state
    print_state(pc, regs, memory, 128);
    return 0;
}
// 

// All prototype implementations below:


void load_machine_code(ifstream &f, uint16_t mem[]) {
    regex machine_code_re("^ram\\[(\\d+)\\] = 16'b(\\d+);.*$");
    size_t expectedaddr = 0;
    string line;
    while (getline(f, line)) {
        smatch sm;

        // checks if matches format
        if (!regex_match(line, sm, machine_code_re)) {
            cerr << "Can't parse line: " << line << endl;
            exit(1);
        }

        // ensures instructions increment properly
        size_t addr = stoi(sm[1], nullptr, 10);
        uint16_t instr = stoi(sm[2], nullptr, 2);
        if (addr != expectedaddr) {
            cerr << "Memory addresses encountered out of sequence: " << addr << endl;
            exit(1);
        }

        // ensures program doesn't load too many instructions
        if (addr >= MEM_SIZE) {
            cerr << "Program too big for memory" << endl;
            exit(1);
        }
        expectedaddr ++;
        mem[addr] = instr; // store instruction in array
    }
}

void print_state(uint16_t pc, uint16_t regs[], uint16_t memory[], size_t memquantity) {
    cout << setfill(' ');
    cout << "Final state:" << endl;
    cout << "\tpc=" <<setw(5)<< pc << endl;

    for (size_t reg=0; reg<NUM_REGS; reg++)
        cout << "\t$" << reg << "="<<setw(5)<<regs[reg]<<endl;

    cout << setfill('0');
    bool cr = false;
    for (size_t count=0; count < memquantity; count++) {
        cout << hex << setw(4) << memory[count] << " ";
        cr = true;
        if (count % 8 == 7) {
            cout << endl;
            cr = false;
        }
    }
    if (cr)
        cout << endl;
}


void parse_instruc(uint16_t mem[], uint16_t registers[], uint16_t &progC){
    uint16_t instruc = mem[progC & 8191]; // 8191 is all 1s except first 3 bits, so this will ignore them
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

        if(reg3 == 0 && func_code != 8) {
            return;
        }

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
            progC = registers[reg1];
            return;
        }
        return; // incase of error
    }

    // 2 reg args

    // isolate immediate (7-bit)
    uint16_t imm_7 = instruc;
    imm_7 = imm_7 << 9;
    imm_7 = imm_7 >> 9;

    // slti
    if (opcode == 7) {
        imm_7 = sign_extend(imm_7);
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

        imm_7 = sign_extend(imm_7);

        registers[reg2] = mem[imm_7 + registers[reg1]];
        return;
    }

    // sw
    if (opcode == 5) {
        imm_7 = sign_extend(imm_7);

        mem[imm_7 + registers[reg1]] = registers[reg2];

        progC += 1;
    }

    // jeq
    if (opcode == 6) {
        if(registers[reg1] == registers[reg2]) {
            imm_7 = sign_extend(imm_7);
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

        imm_7 = sign_extend(imm_7);
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

    // if nothing happens go to next instruction
    progC += 1;
} // parse_instruc



uint16_t sign_extend(uint16_t num) {
    uint16_t msb = num << 9;
    msb = msb >> 15;
    uint16_t temp = 0b1111111110000000;

    if(msb == 0){
        return num;
    }
    return (num | temp);
}

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
        imm = sign_extend(imm);

        // this is where the jump will lead to
        imm += pc + 1;
        imm = imm & 8191; 

        return (regs[reg1] == regs[reg2]) && imm == pc;
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
    dst = srca | srcb;
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




