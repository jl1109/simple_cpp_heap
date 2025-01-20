#include "llist.h"

void add_node(bin_t* bucket, node_t* element) {
    element->next = NULL;
    element->prev = NULL;

    node_t* iter = bucket->head;

    if (bucket->head == NULL) {
        bucket->head = element;
        return;
    }

    node_t* current = bucket->head;
    node_t* previous = NULL;

    while (current != NULL && current->size <= element->size) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        previous->next = element;
        element->prev = previous;
    }
    else {
        if (previous != NULL) {
            element->next = current;
            previous->next = element;

            element->prev = previous;
            current->prev = element;
        }
        else {
            element->next = bucket->head;
            bucket->head->prev = element;
            bucket->head = element;
        }
    }
}

void remove_node(bin_t* bucket, node_t* element) {
    if (bucket->head == NULL) return;
    if (bucket->head == element) {
        bucket->head = bucket->head->next;
        return;
    }

    node_t* iter = bucket->head->next;
    while (iter != NULL) {
        if (iter == element) {
            if (iter->next == NULL) {
                iter->prev->next = NULL;
            }
            else {
                iter->prev->next = iter->next;
                iter->next->prev = iter->prev;
            }
            return;
        }
        iter = iter->next;
    }
}

node_t* get_best_fit(bin_t* bucket, size_t size) {
    if (bucket->head == NULL) return NULL;

    node_t* iter = bucket->head;

    while (iter != NULL) {
        if (iter->size >= size) {
            return iter;
        }
        iter = iter->next;
    }
    return NULL;
}

node_t* get_last_node(bin_t* bucket) {
    node_t* iter = bucket->head;

    while (iter->next != NULL) {
        iter = iter->next;
    }
    return iter;
}
