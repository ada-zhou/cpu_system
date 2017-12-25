#include "circular.h"
#include "malloc.h"
#include "assert.h"

#define CAPACITY 128

struct cir {
    int buf[CAPACITY];
    unsigned head;
    unsigned tail;
};

/* Allocates a new circular buffer, initializes it, and returns a
   pointer to it. */
cir_t *cir_new() {
    cir_t * cirbuf = malloc(sizeof(cir_t));
    cirbuf->head = 0;
    cirbuf->tail = 0;
    return cirbuf;
}

/* Returns 1 if the buffer is empty, 0 otherwise */
int cir_empty(cir_t *cir) {
    if (cir->head == cir->tail){
        return 1;
    }
    return 0;
}

/* Returns 1 if the buffer is full (no more elements could fit), 0 otherwise */
int cir_full(cir_t *cir) {
    if (cir->tail == ((cir->head)+CAPACITY-1)%CAPACITY) {
        return 1;
    }
    return 0;
}

/* Adds an element to the ring buffer
   Asserts an error if the buffer is full */
void cir_enqueue(cir_t *cir, int x) {
    assert((cir_full(cir)) == 0);
    cir->buf[cir->tail] = x;
    cir->tail = ((cir->tail)+1)%CAPACITY;
}

/* Removes an element from the buffer 
   Blocks if the buffer is empty */
int cir_dequeue(cir_t *cir) {
    while (cir_empty(cir));
    int deq = cir->buf[cir->head];
    cir->head = ((cir->head)+1)%CAPACITY;
    return deq;
}

