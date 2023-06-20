#include "cashier.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct cashier *cashier_init(struct cache_config config)
{
  // int * test_memory = malloc(sizeof(int) * 100);
  // YOUR CODE HERE
  // create a cache simulator with a set of parameters. 

  struct cashier* cache = malloc(sizeof(struct cashier));
  if(!cache) // You should return NULL on error.
    return NULL;
  cache->config = config;

  //  lines : total lines -> ways * lines / way
  size_t lines_per_way = config.lines / config.ways;

  cache->lines = malloc(sizeof(struct cache_line) * lines_per_way * config.ways);
  if(!cache->lines) // You should return NULL on error.
  {
    free(cache);
    return NULL;
  }

  // total data bytes in the cache
  cache->size = config.line_size * lines_per_way * config.ways;
  //  | tag | index | offset  |
  //    T      S       B
  //  line_size = 2^B
  //  lines = 2^S
  //  address_bits = T + S + B

  
  // number of bits in offset segment; bit mask for extracting the offset bits.
  cache->offset_bits = 0;
  if(config.line_size != 0)
  {
    while((config.line_size >> cache->offset_bits) != 1)
      ++cache->offset_bits;  
  }
  cache->offset_mask = ((uint64_t)1 << cache->offset_bits) - 1;

  // number of bits in index segment; bit mask for extracting the index bits.
  cache->index_bits = 0;
  if(lines_per_way != 0)
  {
    while((lines_per_way >> cache->index_bits) != 1)
      ++cache->index_bits;
  }
  cache->index_mask = ((uint64_t)1 << cache->index_bits) - 1;

  // number of bits in tag segment; bit mask for extracting the tag bits.
  cache->tag_bits = config.address_bits - cache->index_bits - cache->offset_bits;
  // cache->tag_mask = (0x7fffffffffffffffLL) - cache->index_mask - cache->offset_mask;
  cache->tag_mask = ((uint64_t)1 << cache->tag_bits) - 1;


  // initialize the cache lines
  for (size_t i = 0; i != lines_per_way * config.ways; ++i)
  {
    cache->lines[i].valid = false; // You don’t need to validate the parameters
    cache->lines[i].dirty = false;
    cache->lines[i].tag = 0;
    cache->lines[i].last_access = 0; // update to `get_timestamp()` on access, initialized to 0
    cache->lines[i].data = calloc(config.line_size, sizeof(uint8_t));

    if(!cache->lines[i].data) // You should return NULL on error.
    {
      for(size_t j = 0; j != i; ++j)
        free(cache->lines[j].data);
      free(cache->lines); // all memory should be freed
      free(cache);
      return NULL;
    }
  }
  // printf("tag_bits = %lu\nindex_bit = %lu\noffset_bit = %lu\n", cache->tag_bits, cache->index_bits, cache->offset_bits);
  // printf("tag_mask = %lu\nindex_mask = %lu\noffset_mask = %lu\n", cache->tag_mask, cache->index_mask, cache->offset_mask);
  // You don’t need to validate the parameters.
  return cache;
}

void cashier_release(struct cashier *cache)
{
  // YOUR CODE HERE

  size_t lines_per_way = cache->config.lines / cache->config.ways;

  // release the resources allocated for the cache simulator
  for (size_t i = 0; i != cache->config.ways; ++i)
  {
    for (size_t j = 0; j != lines_per_way; ++j)
    {
      size_t cache_index = i * lines_per_way + j;

      // All the cache lines are considered evicted on cashier_release
      if(cache->lines[cache_index].valid)
      {
        before_eviction(j, &cache->lines[cache_index]);
        if(cache->lines[cache_index].dirty)
        {
          // printf("line = %lu, way = %lu\n", i, j);
          // write-back dirty cache line on eviction
          uint64_t addr = (cache->lines[cache_index].tag << (cache->index_bits + cache->offset_bits))
                        + (j << cache->offset_bits);
          for(size_t k = 0; k != cache->config.line_size; ++k)
            mem_write(addr + k, cache->lines[cache_index].data[k]);
        }
      }

      free(cache->lines[cache_index].data);
    }
  }
  free(cache->lines); // all memory should be freed
  free(cache);
  return;
}

bool cashier_read(struct cashier *cache, uint64_t addr, uint8_t *byte)
{
  // YOUR CODE HERE
  //  | tag | index | offset  |
  uint64_t tag = (addr >> (cache->index_bits + cache->offset_bits)) & cache->tag_mask;
  uint64_t index = (addr >> cache->offset_bits) & cache->index_mask;
  uint64_t offset = addr & cache->offset_mask;

  size_t lines_per_way = cache->config.lines / cache->config.ways;

  // may have a victim
  size_t victim_index = 0 * lines_per_way + index;
  size_t empty_index = 0xffffffffffffffffLL;

  for(size_t i = 0; i != cache->config.ways; ++i)
  {
    // printf("i = %lu\n", i);
    // the j-th slot in i-th set is `data[i * slots_per_way + j]`
    size_t cache_index = i * lines_per_way + index;

    // LRU replacement policy
    if(cache->lines[cache_index].last_access < cache->lines[victim_index].last_access)
      victim_index = cache_index;

    if(cache->lines[cache_index].valid && cache->lines[cache_index].tag == tag)
    {
      // cache hit
      *byte = cache->lines[cache_index].data[offset];
      cache->lines[cache_index].last_access = get_timestamp();
      return true; // return true on cache hit
    }

    // found an empty slot
    if(cache->lines[cache_index].valid == false && empty_index == 0xffffffffffffffffLL)
      empty_index = cache_index;
  }

  // cache miss
  // load entire cache line from data memory on miss

  // found an empty slot
  if(empty_index != 0xffffffffffffffffLL)
  {
    // Placing the cache line at a previously invalidated slot
    // is not considered as an eviction.
    cache->lines[empty_index].last_access = get_timestamp();
    cache->lines[empty_index].tag = tag;
    cache->lines[empty_index].valid = true;
    cache->lines[empty_index].dirty = false;

    // store the data from memory to the cache line
    for(size_t j = 0; j != cache->config.line_size; ++j)
      cache->lines[empty_index].data[j] = mem_read(addr - offset + j);

    *byte = cache->lines[empty_index].data[offset];
    return false; // return false on cache miss
  }

  // Call before_eviction if you have to evict a cache line.
  before_eviction(index, &cache->lines[victim_index]);
  
  if(cache->lines[victim_index].dirty)
  {
    // write-back dirty cache line on eviction
    uint64_t victim_addr = (cache->lines[victim_index].tag << (cache->index_bits + cache->offset_bits))
                         + (index << cache->offset_bits);
    for(size_t i = 0; i != cache->config.line_size; ++i)
      mem_write(victim_addr + i, cache->lines[victim_index].data[i]);
  }

  // set parameters for the new cache line
  cache->lines[victim_index].last_access = get_timestamp();
  cache->lines[victim_index].tag = tag;
  cache->lines[victim_index].valid = true;
  cache->lines[victim_index].dirty = false;

  // store the data from memory to the cache line
  for(size_t i = 0; i != cache->config.line_size; ++i)
    cache->lines[victim_index].data[i] = mem_read(addr - offset + i);

  *byte = cache->lines[victim_index].data[offset];
  return false; // return false on cache miss
}

bool cashier_write(struct cashier *cache, uint64_t addr, uint8_t byte)
{
  // YOUR CODE HERE
  //  | tag | index | offset  |
  uint64_t tag = (addr >> (cache->index_bits + cache->offset_bits)) & cache->tag_mask;
  uint64_t index = (addr >> cache->offset_bits) & cache->index_mask;
  uint64_t offset = addr & cache->offset_mask;

  // printf("tag = %lu\nindex = %lu\noffset = %lu\n", tag, index, offset);

  size_t lines_per_way = cache->config.lines / cache->config.ways;

  // may have a victim
  // use LRU replacement policy
  size_t victim_index = 0 * lines_per_way + index;
  size_t empty_index = 0xffffffffffffffffLL;

  for(size_t i = 0; i != cache->config.ways; ++i)
  {
    // the j-th slot in i-th set is `data[i * slots_per_way + j]`
    size_t cache_index = i * lines_per_way + index;
    // printf("%lu\n", cache_index);
    
    // LRU replacement policy
    if(cache->lines[cache_index].last_access < cache->lines[victim_index].last_access)
      victim_index = cache_index;

    if(cache->lines[cache_index].valid && cache->lines[cache_index].tag == tag)
    {
      // cache hit
      cache->lines[cache_index].dirty = true;
      cache->lines[cache_index].data[offset] = byte;
      cache->lines[cache_index].last_access = get_timestamp();
      return true; // return true on cache hit
    }

    if(cache->lines[cache_index].valid == false && empty_index == 0xffffffffffffffffLL)
      empty_index = cache_index;
  }

  // cache miss
  // load entire cache line from data memory on miss

  // found an empty slot
  if(empty_index != 0xffffffffffffffffLL)
  {
    // Placing the cache line at a previously invalidated slot
    // is not considered as an eviction.
    cache->lines[empty_index].last_access = get_timestamp();
    cache->lines[empty_index].tag = tag;
    cache->lines[empty_index].valid = true;
    cache->lines[empty_index].dirty = true;

    // modify the memory
    // mem_write(addr, byte);

    // store the data from memory to the cache line
    for(size_t j = 0; j != cache->config.line_size; ++j)
      cache->lines[empty_index].data[j] = mem_read(addr - offset + j);

    cache->lines[empty_index].data[offset] = byte;
    return false; // return false on cache miss
  }
  
  // have a victim
  // Call before_eviction if you have to evict a cache line.
  before_eviction(index, &cache->lines[victim_index]);
  
  if(cache->lines[victim_index].dirty)
  {
    // write-back dirty cache line on eviction
    uint64_t victim_addr = (cache->lines[victim_index].tag << (cache->index_bits + cache->offset_bits))
                         + (index << cache->offset_bits);
    for(size_t i = 0; i != cache->config.line_size; ++i)
      mem_write(victim_addr + i, cache->lines[victim_index].data[i]);
  }

  // set parameters for the new cache line
  cache->lines[victim_index].last_access = get_timestamp();
  cache->lines[victim_index].tag = tag;
  cache->lines[victim_index].valid = true;
  cache->lines[victim_index].dirty = true;

  // modify the memory
  // mem_write(addr, byte);
  
  // store the data from memory to the cache line
  for(size_t i = 0; i < cache->config.line_size; ++i)
    cache->lines[victim_index].data[i] = mem_read(addr - offset + i);
  cache->lines[victim_index].data[offset] = byte;
  return false; // return false on cache miss
}