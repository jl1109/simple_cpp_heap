#ifndef HEAP_H
#define HEAP_H

#include <cstdint>
#include <cstddef>

#define HEAP_INIT_SIZE 0x10000
#define HEAP_MAX_SIZE 0xF0000
#define HEAP_MIN_SIZE 0x10000

#define MIN_ALLOC_SZ 4

#define MIN_WILDERNESS 0x2000
#define MAX_WILDERNESS 0x1000000

#define BIN_COUNT 9
#define BIN_MAX_IDX (BIN_COUNT - 1)

using uint = unsigned int;

struct node_t {
    uint hole;
    uint size;
    node_t* next;
    node_t* prev;
};

struct footer_t {
    node_t* header;
};

struct bin_t {
    node_t* head;
};

struct heap_t {
    uintptr_t start;  
    uintptr_t end;  
    bin_t* bins[BIN_COUNT];
};

static uint overhead = sizeof(footer_t) + sizeof(node_t);

void init_heap(heap_t* heap, uintptr_t start);

void* heap_alloc(heap_t* heap, size_t size);
void heap_free(heap_t* heap, void* p);
uint expand(heap_t* heap, size_t sz);
void contract(heap_t* heap, size_t sz);

uint get_bin_index(size_t sz);
void create_foot(node_t* head);
footer_t* get_foot(node_t* head);

node_t* get_wilderness(heap_t* heap);

#endif
