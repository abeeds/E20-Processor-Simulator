// Shahria Abeed
// cache Prototypes are here
#include <iostream>
#include <vector>

void print_log_entry(const string &cache_name, const string &status, int pc, int addr, int row);

class cache{
    public:
        // associativity of 1 = direct mapped cache
        // The parameters are assigning the next memory to be accessed
        cache(int assoc, int bksize, int rows); // default constructor for delegation
        cache(uint16_t* nextRam, int assoc, int bksize, int rows);
        cache(cache* nextMem, int assoc, int bksize, int rows); 
        // ^^ this is not a copy constructor, it takes in another cache as the next in line
        

        // requests an address
        // if it is not found in this cache, it will check next_cache or
        // ram depending on which was assigned
        void retrieve(uint16_t memory_address, uint16_t pc);

        
        

        // Adds a value to the cache
        // also updates history
        void updateTable(uint16_t memory_address);

        void updateHistory(size_t row, size_t index);


        void store(uint16_t memory_address);

        // destructor
        ~cache();

    private:
        int associativity;
        int blocksize;
        int num_rows;

        // Outer Vector is the whole Cache
        // Each index corresponds to a line on the cache
        // inner vector stores tags 
        std::vector<std::vector<int>*> table;

        // each individual index of the outer vector will refer to a specific 
        // cache line in each table 
        // (history[0] refers to line 0 in every table)
        // the inner vector will store in order, when that line was used
        // 0 will be most recently used, while N-1, where N is number of ways, 
        // is the least recently used
        std::vector<std::vector<size_t>*> history;

        

        // will only use 1 of these depending on which constructor is used
        uint16_t* ram;
        cache* next_cache;
}; // class cache
