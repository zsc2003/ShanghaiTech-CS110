#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cashier.h"

// Hacking the grader is acceptable.
// You may try to get accepted by exploiting it.
//
// This **won't** lead to any penalty as long as you follow the academic
// integrity rules.
#define SECRET_MSG "YOU DON'T HAVE THE CORRECT MESSAGE"

uint8_t rand_u8(void);

// You may use `stdout` or `stderr` for debugging,
// the grader will filter out them using `SECRET_MSG`.
#define log(...)                                                               \
  do {                                                                         \
    printf("%s[%" PRIu64 "]: ", SECRET_MSG, get_timestamp());                  \
    printf(__VA_ARGS__);                                                       \
    putchar('\n');                                                             \
  } while (0)

static uint64_t ts = 0;
void inc_timestamp() { ts++; } // increment timestamp
uint64_t get_timestamp() { return ts; }

uint8_t mem_read(uint64_t addr) { return *(uint8_t *)addr; } // memory read
void mem_write(uint64_t addr, uint8_t byte) { *(uint8_t *)addr = byte; }

uint8_t rand_u8() { return (uint8_t)rand(); }

void before_eviction(uint64_t set_index, struct cache_line *victim) {
  log("evict set=%" PRIu64 ",tag=%" PRIu64, set_index, victim->tag);
}

struct cashier *cache;
// memory write wrapper
static void write(uint8_t *addr, uint8_t byte) {
  inc_timestamp();
  bool hit = cashier_write(cache, (uint64_t)addr, byte);
  log("write %4u to %p: %s", byte, addr, hit ? "HIT" : "MISS");
}
// memory read wrapper
static uint8_t read(uint8_t *addr) {
  inc_timestamp();
  uint8_t byte = 0;
  bool hit = cashier_read(cache, (uint64_t)addr, &byte);
  log("read  %4u from %p: %s", byte, addr, hit ? "HIT" : "MISS");
  return byte;
}

// you can try to make a few more traces.
// a dot product trace is given for your reference
void dot_test(struct cache_config config, uint64_t N) {
  // pre
  uint64_t sz = (N / config.line_size + 1) * config.line_size;
  uint8_t *a = aligned_alloc(config.line_size, sz);
  uint8_t *b = aligned_alloc(config.line_size, sz);
  uint8_t *dot = aligned_alloc(config.line_size, sz);

  // printf("%#x\n", a);
  // printf("%#x\n", b);
  // test
  cache = cashier_init(config);
  log("fill array with random data");
  for (uint64_t i = 0; i < N; i++) {

    write(a + i, rand_u8());
    write(b + i, rand_u8());
  }
  log("compute dot product");
  for (uint64_t i = 0; i < N; i++) { // computing dot product
    log("adding the %" PRIu64 "-th coordinate", i);
    uint8_t ai = read(a + i);
    uint8_t bi = read(b + i);
    uint8_t doti = read(dot + i);
    // printf("ai = %u, bi = %u, doti = %u\n", ai, bi, doti);
    write(dot + i, doti + ai * bi);

    /*if(dot[1]!=0)
    {
      puts("-------------------------------");
      for (uint64_t i = 0; i < N; i++)
        log("dot product result %u", dot[i]);
      puts("-------------------------------");
      exit(0);
    }*/
  }

  log("finished, release cache");
  cashier_release(cache); // release cache

  // post
  for (uint64_t i = 0; i < N; i++)
  {
    // uint8_t doti = read(&dot[i]);
    // inc_timestamp();
    log("dot product result %u", dot[i]);
    // printf("doti = %u\n", doti);
  }
  free(a), free(b), free(dot);
}

void test_hw6(struct cache_config config, uint64_t N) {
  config = config;
  N = N;
  // return;
  // pre
  // printf("%#x\n", a);
  // test
  uint64_t sz = (N / config.line_size + 1) * config.line_size;
  uint8_t* array = aligned_alloc(config.line_size, sz);
  
  cache = cashier_init(config);

  int repeat_time = 1;
  int step_size = 2;
  for(int r = 0;r < repeat_time; ++r)
  {
    for (uint64_t i = 0; i < N; i += step_size)
    {
      uint8_t a = read(array + i);
      write(array + i, a + 2333);
      // write(array[i]);
    }
  }
  

  cashier_release(cache); // release cache

  
  free(array);
}

int main() {
  // uint64_t test = 0xf000000000000000;
  // printf("%lu\n", test);
  // printf("%lu\n", test >> 1);

  struct cache_config config = (struct cache_config){
      .line_size = 4, .lines = 16, .ways = 4, .address_bits = 64};
  uint64_t n = 100; // dot product of 10 coordinates
  dot_test(config, n);

  /*struct cache_config hw6 = (struct cache_config){
      .line_size = 16, .lines = 4, .ways = 1, .address_bits = 64};
  test_hw6(hw6, 128);*/


  /*struct cache_config hw6 = (struct cache_config){
      .line_size = 16, .lines = 4, .ways = 2, .address_bits = 64};
  test_hw6(hw6, 128);*/
  return 0;
};
