#include "cache.h"
#include <cmath>
#include <cstddef>
#include <string>
#include <fstream>
#include <limits>
#include <iomanip>
using namespace std;

/*
    Useful Info:
        Cache Index = (Memory Address // Blocksize) % Number of Cache Lines
        Block Offset = Memory Address % Blocksize
        Tag = (Memory Address // Blocksize) // Number of Cache Lines
*/

void print_log_entry(const string &cache_name, const string &status, int pc, int addr, int row) {
    cout << left << setw(8) << cache_name + " " + status <<  right <<
        " pc:" << setw(5) << pc <<
        "\taddr:" << setw(5) << addr <<
        "\trow:" << setw(4) << row << endl;
}


// constructor (using ram as next memory)
// will not be used on its own
cache::cache(int assoc, int bksize, int rows) {
    associativity = assoc;
    blocksize = bksize;
    num_rows = rows;

    // start up tables & rows
    for(int i = 0; i < associativity; i++) {
        table.push_back(new vector<int>);
    }

    // start up history collection
    // no need if associativity is 1 (direct mapped)
    if(associativity > 1) {
        for(int i = 0; i < rows; i++) {
            history.push_back(new vector<int>);
        }
    }
    
}


// constructor (using RAM as next memory)
cache::cache(uint16_t* nextRam, int assoc, int bksize, int rows) : cache::cache(assoc, bksize, rows) {
    ram = nextRam;
}


// constructor (using cache as next memory)
cache::cache(cache* nextMem, int assoc, int bksize, int rows) : cache::cache(assoc, bksize, rows){
    next_cache = nextMem;
}



void cache::retrieve(uint16_t memory_address, uint16_t pc) {
    uint16_t output = -1; // this is not a real memory address, and will let us know if we found what was requested

    // calculate identifiers
        // Calculate tag
        int tag = floor(memory_address / this->blocksize);
        tag = floor(tag / num_rows);

        // Calculate Index
        int index = floor(memory_address / this->blocksize);
        index = tag % num_rows;
    // 

    // search for tag within table
    for(size_t i = 0; i < table.size(); i++) {
        // ignore if current table is empty
        if(empty(*table[i])) {
            continue;
        }

        // need to update Cache history
        if(table[i]->at(index) == tag) {
            if(this->next_cache) {
                print_log_entry("L1", "HIT", pc, memory_address, index);
            }

            if(this->ram) {
                print_log_entry("L2", "HIT", pc, memory_address, index);
            }


        }

    }

    // If we didn't find the desired memory Address:

    // if the next in line memory is the ram & we didn't find what we requested
    if(this->ram && output == -1) {
        //print_log_entry(const string &cache_name, const string &status, int pc, int addr, int row)
        print_log_entry("L2", "MISS", pc, memory_address, index);
        
        // update cache tables & history


    }

    // if it is a cache & we didn't find what we requested
    if(this->next_cache && output == -1) {
        // print accordingly
        print_log_entry("L1", "MISS", pc, memory_address, index);
        
        // will also print for the next cache
        this->next_cache->retrieve(memory_address, pc);

        // update cache tables & history

    }

    // update the cache history

}




// adds a value to table & updates history accordingly
void cache::updateTable(uint16_t memory_address) {
    
}

void cache::store(uint16_t memory_address) {

}


// destructor
cache::~cache() {
    for(size_t i = 0; i < table.size(); i++) {
        // delete all vectors within table
        delete table[i];
    }

    for(size_t i = 0; i < history.size(); i++) {
        // delete all vectors within history
        delete history[i];
    }
}

