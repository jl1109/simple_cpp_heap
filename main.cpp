#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    int idx;

    heap_t* memory_pool = (heap_t*)malloc(sizeof(heap_t));  // Cast to heap_t*
    memset(memory_pool, 0, sizeof(heap_t));

    void* memory_region = malloc(HEAP_INIT_SIZE);
    memset(memory_region, 0, HEAP_INIT_SIZE);

    // Initialize the bins for the heap
    for (idx = 0; idx < BIN_COUNT; idx++) {
        memory_pool->bins[idx] = (bin_t*)malloc(sizeof(bin_t));  
        memset(memory_pool->bins[idx], 0, sizeof(bin_t));
    }

    // Initialize the heap with the allocated memory region
    init_heap(memory_pool, (long)memory_region);

    printf("overhead = %d \n", overhead);

    // Allocate memory blocks of various sizes
    void* block_1 = heap_alloc(memory_pool, 8);
    printf("block_1 = %d size: 8 \n", (int)block_1);
    void* block_2 = heap_alloc(memory_pool, 128);
    printf("block_2 = %d size: 128 \n", (int)block_2);
    void* block_3 = heap_alloc(memory_pool, 8);
    printf("block_3 = %d size: 8 \n", (int)block_3);

    // Free some blocks
    printf("\nfreeing block_2 \n");
    heap_free(memory_pool, block_2);

    void* block_4 = heap_alloc(memory_pool, 8);
    printf("block_4 = %d size: 8 \n", (int)block_4);

    void* block_5 = heap_alloc(memory_pool, 16);
    printf("block_5 = %d size: 16 \n", (int)block_5);

    void* block_6 = heap_alloc(memory_pool, 8);
    printf("block_6 = %d size: 8 \n", (int)block_6);

    void* block_7 = heap_alloc(memory_pool, 8);
    printf("block_7 = %d size: 8 \n", (int)block_7);

    // Free more blocks
    printf("\nfreeing block_4 & block_6 \n");
    heap_free(memory_pool, block_4);
    heap_free(memory_pool, block_6);

    printf("\nfreeing block_5\n");
    heap_free(memory_pool, block_5);

    // Allocate more memory
    void* block_8 = heap_alloc(memory_pool, 128);
    printf("block_8 = %d size: 128 \n", (int)block_8);
    printf("\n");

    // Print bin index for different sizes
    for (idx = 1; idx <= 2048; idx += idx) printf("size: %d -> bin: %d \n", idx, get_bin_index(idx));

    // Free allocated bins
    for (idx = 0; idx < BIN_COUNT; idx++) {
        free(memory_pool->bins[idx]);
    }

    free(memory_pool);
    free(memory_region);

    return 0;
}
