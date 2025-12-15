#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

enum queue_expansion_type {
	QUEUE_EXPANSION_FIXED
};
// TODO!!! reduce memory usage by reusing more than one freed queue bodies !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
typedef struct queue_int {
    enum queue_expansion_type expansion_type;
    struct queue_int_body* first_queue;
    struct queue_int_body* last_queue;

    struct queue_int_body* freed_queue;
} queue_int;

typedef struct queue_int_body {
    unsigned int current_size;
    unsigned int queue_size;
    struct queue_int_body* next_queue;
    int* data;
} queue_int_body;

static inline void set_queue_int_body(queue_int_body* const queue, queue_int_body* const next_queue, const unsigned int queue_size)
{
    queue->next_queue = next_queue;
    queue->current_size = 0;
    queue->queue_size = min(queue_size, 1);
    queue->data = (int*)calloc(queue->queue_size, sizeof(int));
}

static inline queue_int_body* new_queue_int_body(queue_int_body* const next_queue, const unsigned int queue_size)
{
    queue_int_body* queue = (queue_int_body*)calloc(1, sizeof(queue_int_body));
    set_queue_int_body(queue, next_queue, queue_size);
    return queue;
}

static inline void clean_queue_int_body(queue_int_body* const queue)
{
    if (queue->data != NULL) free(queue->data);
}

static inline void set_queue_int(queue_int* const queue, const unsigned int initial_queue_size, const enum queue_expansion_type expansion_type)
{
    queue->expansion_type = expansion_type;
    queue->first_queue = new_queue_int_body(NULL, initial_queue_size);
    queue->last_queue = queue->first_queue;
    queue->freed_queue = NULL;
}

static inline queue_int* new_queue_int(const unsigned int initial_queue_size, const enum queue_expansion_type expansion_type)
{
    queue_int* queue = (queue_int*)calloc(1, sizeof(queue_int));
    set_queue_int(queue, initial_queue_size, expansion_type);
    return queue;
}

static inline void clean_queue_int(queue_int* const queue)
{
    queue_int_body* current_queue = queue->first_queue;
    while (current_queue != NULL)
    {
        queue_int_body* next_queue = current_queue->next_queue;
        clean_queue_int_body(current_queue);
        free(current_queue);
        current_queue = next_queue;
    }
}

// Returns 0 on success
static inline int push_queue_int(queue_int* const queue, const int value)
{
    queue_int_body* current_queue = queue->last_queue;
    if (current_queue->current_size == current_queue->queue_size)
    {
        queue_int_body* new_queue;
        if (queue->freed_queue == NULL)
        {
            int new_queue_size;
            switch (queue->expansion_type)
            {
                case QUEUE_EXPANSION_FIXED:
                default:
                    new_queue_size = current_queue->queue_size;
                    break;
            }
            new_queue = new_queue_int_body(NULL, new_queue_size);
        }
        else
        {
            new_queue = queue->freed_queue;
            new_queue->current_size = 0;
            new_queue->next_queue = NULL;
            queue->freed_queue = NULL;
        }
        current_queue->next_queue = new_queue;
        current_queue = current_queue->next_queue;
        queue->last_queue = current_queue;
    }

    current_queue->data[current_queue->current_size] = value;
    current_queue->current_size++;
    return 0;
}

// Returns 0 if queue is empty
static inline int pop_queue_int(queue_int* const queue)
{
    queue_int_body* current_queue = queue->first_queue;
    if (current_queue->current_size == 0)
    {
        if (queue->first_queue == queue->last_queue) return 0; // queue is empty

        queue->first_queue = current_queue->next_queue;
        if (queue->freed_queue == NULL)
        {
            queue->freed_queue = current_queue;
            current_queue->next_queue = NULL;
        }
        else
        {
            clean_queue_int_body(current_queue);
            free(current_queue);
        }
        current_queue = queue->last_queue;
    }

    current_queue->current_size--;
    return current_queue->data[current_queue->current_size];
}

#endif