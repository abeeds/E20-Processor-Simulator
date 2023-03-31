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

/*
    Loads an E20 machine code file into the list
    provided by mem. We assume that mem is
    large enough to hold the values in the machine
    code file.

    @param f Open file to read from
    @param mem Array representing memory into which to read program
*/
void load_machine_code(ifstream &f, unsigned mem[]);

/*
    Prints the current state of the simulator, including
    the current program counter, the current register values,
    and the first memquantity elements of memory.

    @param pc The final value of the program counter
    @param regs Final value of all registers
    @param memory Final value of memory
    @param memquantity How many words of memory to dump
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




int main(int argc, char *argv[]) {
    /*
        Parse the command-line arguments
    */
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
    /* Display error message if appropriate */
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
    //               Variable Assignments

    // registers
    uint16_t regs[8];
    regs[0] = 0;

    // memory
    unsigned memory[8192];

    // program counter
    uint16_t pc = 0;
    
    // -------------------------------------------------
    //          Store Instructions in Memory

    load_machine_code(file, memory);
    
    // -------------------------------------------------
    // TODO: your code here. Do simulation.

    // TODO: your code here. print the final state of the simulator before ending, using print_state
    
    return 0;
}
// 

// All prototype implementations below:



void load_machine_code(ifstream &f, unsigned mem[]) {
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
        unsigned instr = stoi(sm[2], nullptr, 2);
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




void add(uint16_t &dst, uint16_t srca, uint16_t srcb) {
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

