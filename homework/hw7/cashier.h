#ifndef CASHIER
#define CASHIER

#include <stdbool.h>
#include <stdint.h>

// get current timestamp: monotonic increasing positive integer value
uint64_t get_timestamp(void);

// read a byte from the data memory
uint8_t mem_read(uint64_t addr);
// write a byte into the data memory
void mem_write(uint64_t addr, uint8_t byte);

// Configuration of the cache
typedef struct cache_config {
  // address space size in bits: A=T+I+O
  uint64_t address_bits;
  // size of each cache line in bytes: guaranteed to be power of 2
  uint64_t line_size;
  // number of lines in the cache: guaranteed to be power of 2
  uint64_t lines;
  // N-way associative cache
  uint64_t ways;
}cache_config;

// Cache line data structure
typedef struct cache_line {
  // the valid bit and dirty bit
  bool valid, dirty;
  // tag bits of this cache line, initialize to 0
  uint64_t tag;
  // last access timestamp
  // update to `get_timestamp()` on access, initialized to 0
  uint64_t last_access;
  // the data bytes in this cache line
  uint8_t *data;
}cache_line;

// call this function before evicting one cache line
void before_eviction(uint64_t set_index, struct cache_line *victim);

// A CASHIER simple cache simulator
//
// - never evict a line when a invalid slot is available
// - use LRU replacement policy
// - load entire cache line from data memory on miss
// - write-back dirty cache line on eviction
//
typedef struct cashier {
  // cache simulator configuration
  struct cache_config config;
  // total data bytes in the cache
  uint64_t size;

  // number of bits in tag segment; bit mask for extracting the tag bits.
  uint64_t tag_bits, tag_mask;
  // number of bits in index segment; bit mask for extracting the index bits.
  uint64_t index_bits, index_mask;
  // number of bits in offset segment; bit mask for extracting the offset bits.
  uint64_t offset_bits, offset_mask;

  // the cache lines in this
  //
  // for (i,j) in [0,associative-1] x [0,sets-1]
  // the j-th slot in i-th set is `data[i * slots_per_way + j]`
  struct cache_line *lines;
}cashier;

// initialize a cache simulator for a configuration
// return NULL on resource allocation failure
struct cashier *cashier_init(struct cache_config config);
// release the resources allocated for the cache simulator
// also writeback dirty lines
//
// The order in which lines are evicted is:
// set0-slot0, set1-slot0, set2-slot0,
// set0-slot1, set1-slot1, set2-slot1,
// and so on.
void cashier_release(struct cashier *cache);
// read one byte at a specific address. return hit=true/miss=false
bool cashier_read(struct cashier *cache, uint64_t addr, uint8_t *byte);
// write one byte into a specific address. return hit=true/miss=false
bool cashier_write(struct cashier *cache, uint64_t addr, uint8_t byte);

#endif
