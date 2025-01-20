#include "heap.h"
#include "llist.h"

uint meta_offset = 8;

void init_heap(heap_t* heap, uintptr_t start) {
    node_t* initial_chunk = (node_t*)start;
    initial_chunk->hole = 1;
    initial_chunk->size = (HEAP_INIT_SIZE)-sizeof(node_t) - sizeof(footer_t);

    create_foot(initial_chunk);

    add_node(heap->bins[get_bin_index(initial_chunk->size)], initial_chunk);

    heap->start = start; 
    heap->end = start + HEAP_INIT_SIZE;
}

void* heap_alloc(heap_t* heap, size_t size) {
    uint index = get_bin_index(size);
    bin_t* temp = (bin_t*)heap->bins[index];
    node_t* block = get_best_fit(temp, size);

    while (block == NULL) {
        if (index + 1 >= BIN_COUNT)
            return NULL;

        temp = heap->bins[++index];
        block = get_best_fit(temp, size);
    }

    if ((block->size - size) > (meta_offset + MIN_ALLOC_SZ)) {
        node_t* split = (node_t*)(((char*)block + sizeof(node_t) + sizeof(footer_t)) + size);
        split->size = block->size - size - sizeof(node_t) - sizeof(footer_t);
        split->hole = 1;

        create_foot(split);

        uint new_index = get_bin_index(split->size);

        add_node(heap->bins[new_index], split);

        block->size = size;
        create_foot(block);
    }

    block->hole = 0;
    remove_node(heap->bins[index], block);

    node_t* wilderness = get_wilderness(heap);
    if (wilderness->size < MIN_WILDERNESS) {
        uint success = expand(heap, 0x1000);
        if (success == 0) {
            return NULL;
        }
    }
    else if (wilderness->size > MAX_WILDERNESS) {
        contract(heap, 0x1000);
    }

    block->prev = NULL;
    block->next = NULL;
    return &block->next;
}

void heap_free(heap_t* heap, void* pointer) {
    bin_t* bin;
    footer_t* new_footer, * old_footer;

    node_t* head = (node_t*)((char*)pointer - meta_offset);
    if (head == (node_t*)heap->start) { // Direct pointer comparison
        head->hole = 1;
        add_node(heap->bins[get_bin_index(head->size)], head);
        return;
    }

    node_t* next = (node_t*)((char*)get_foot(head) + sizeof(footer_t));
    footer_t* footer = (footer_t*)((char*)head - sizeof(footer_t));
    node_t* prev = footer->header;

    if (prev->hole) {
        bin = heap->bins[get_bin_index(prev->size)];
        remove_node(bin, prev);

        prev->size += meta_offset + head->size;
        new_footer = get_foot(head);
        new_footer->header = prev;

        head = prev;
    }

    if (next->hole) {
        bin = heap->bins[get_bin_index(next->size)];
        remove_node(bin, next);

        head->size += meta_offset + next->size;

        old_footer = get_foot(next);
        old_footer->header = 0;
        next->size = 0;
        next->hole = 0;

        new_footer = get_foot(head);
        new_footer->header = head;
    }

    head->hole = 1;
    add_node(heap->bins[get_bin_index(head->size)], head);
}

uint expand(heap_t* heap, size_t sz) {
    return 0;
}

void contract(heap_t* heap, size_t sz) {
    return;
}

uint get_bin_index(size_t sz) {
    uint index = 0;
    sz = sz < 4 ? 4 : sz;

    while (sz >>= 1) index++;
    index -= 2;

    if (index > BIN_MAX_IDX) index = BIN_MAX_IDX;
    return index;
}

void create_foot(node_t* block) {
    footer_t* foot = get_foot(block);
    foot->header = block;
}

footer_t* get_foot(node_t* node) {
    return (footer_t*)((char*)node + sizeof(node_t) + node->size);
}

node_t* get_wilderness(heap_t* heap) {
    footer_t* wilderness_footer = (footer_t*)((char*)heap->end - sizeof(footer_t));
    return wilderness_footer->header;
}

