#include "cache.h"
#include <cmath>
#include <cstddef>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <bits/stdc++.h>
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
            history.push_back(new vector<size_t>);
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
    bool found = false;
    // calculate identifiers
        // Calculate tag
        int tag = floor(memory_address / blocksize);
        tag = floor(tag / num_rows);

        // Calculate Index
        int index = floor(memory_address / blocksize);
        index = tag % num_rows;
    // 

    // search for tag within table
    for(size_t i = 0; i < table.size(); i++) {
        // ignore if current table is empty
        if(empty(*table[i])) {
            continue;
        }

        // tag found
        if(table[i]->at(index) == tag) {

            // identify which cache is being used & print message
            if(next_cache) {
                print_log_entry("L1", "HIT", pc, memory_address, index);
            }
            if(ram) {
                print_log_entry("L2", "HIT", pc, memory_address, index);
            }
            found = true;
            updateHistory(index, i);
            return;
        }

    }

    // If we didn't find the desired memory Address:

    

    // if the next in line memory is the ram & we didn't find what we requested
    if(ram && !found) {
        //print_log_entry(const string &cache_name, const string &status, int pc, int addr, int row)
        print_log_entry("L2", "MISS", pc, memory_address, index);

        // store it in cache
        updateTable(memory_address);


    }

    // if it is a cache & we didn't find what we requested
    if(next_cache && !found) {
        print_log_entry("L1", "MISS", pc, memory_address, index);
        // will perform this for next cache
        next_cache->retrieve(memory_address, pc);

        // store in cache
        updateTable(memory_address);

    }

}




// adds a value to table & updates history accordingly
void cache::updateTable(uint16_t memory_address) {
    // calculate identifiers
        // Calculate tag
        int tag = floor(memory_address / blocksize);
        tag = floor(tag / num_rows);

        // Calculate Index or row
        int index = floor(memory_address / blocksize);
        index = tag % num_rows;
    // 

    // identify which set to add the new value to
    size_t LRU; // least recently used

    // incase certain ways have not yet been accessed
    if(history[index]->size() < associativity) {
        LRU = history[index]->size();
    }
    if(history[index]->size() == associativity) {
        // if associativity is 4, highest index is 3
        // which is also the least recently accessed
        LRU = history[index]->at(associativity - 1);
    }

    table[LRU]->at(index) = tag;
    updateHistory(index, LRU);
}

// will move the accessed row up in the history
// to show that it was used most recently
void cache::updateHistory(size_t row, size_t way) {
    // locate it in the history
    bool found = false;
    size_t ind;
    for(size_t i = 0; i < history[row]->size(); i++) {
        if(history[row]->at(i) == way) {
            found = true;
            ind = i;
        }
    }

    // if it is not in the history
    if(!found) {
        ind = history[row]->size();
        history[row]->push_back(way);  
    }
     
    // move it up to [0]
    int temp = history[row]->at(ind); // store the original value to be moved
    for(size_t i = way; i > 0; i--) {
        history[row]->at(i) = history[row]->at(i - 1);
    }
    history[row]->at(0) = temp;
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

