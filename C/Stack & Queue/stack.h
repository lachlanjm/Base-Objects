#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

#ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

enum stack_expansion_type {
	STACK_EXPANSION_DOUBLE,
	STACK_EXPANSION_FIXED
};

typedef struct stack_int {
    enum stack_expansion_type expansion_type;
    struct stack_int_body* first_stack;
    struct stack_int_body* last_stack;
} stack_int;

typedef struct stack_int_body {
    unsigned int current_size;
    unsigned int stack_size;
    struct stack_int_body* prev_stack;
    int* data;
} stack_int_body;

static inline void set_stack_int_body(stack_int_body* const stack, stack_int_body* const prev_stack, const unsigned int stack_size)
{
    stack->prev_stack = prev_stack;
    stack->current_size = 0;
    stack->stack_size = min(stack_size, 1);
    stack->data = (int*)calloc(stack->stack_size, sizeof(int));
}

static inline stack_int_body* new_stack_int_body(stack_int_body* const prev_stack, const unsigned int stack_size)
{
    stack_int_body* stack = (stack_int_body*)calloc(1, sizeof(stack_int_body));
    set_stack_int_body(stack, prev_stack, stack_size);
    return stack;
}

static inline void clean_stack_int_body(stack_int_body* const stack)
{
    if (stack->data != NULL) free(stack->data);
}

static inline void set_stack_int(stack_int* const stack, const unsigned int initial_stack_size, const enum stack_expansion_type expansion_type)
{
    stack->expansion_type = expansion_type;
    stack->first_stack = new_stack_int_body(NULL, initial_stack_size);
    stack->last_stack = stack->first_stack;
}

static inline stack_int* new_stack_int(const unsigned int initial_stack_size, const enum stack_expansion_type expansion_type)
{
    stack_int* stack = (stack_int*)calloc(1, sizeof(stack_int));
    set_stack_int(stack, initial_stack_size, expansion_type);
    return stack;
}

static inline void clean_stack_int(stack_int* const stack)
{
    stack_int_body* current_stack = stack->last_stack;
    while (current_stack != NULL)
    {
        stack_int_body* prev_stack = current_stack->prev_stack;
        clean_stack_int_body(current_stack);
        free(current_stack);
        current_stack = prev_stack;
    }
}

// Returns 0 on success
static inline int push_stack_int(stack_int* const stack, const int value)
{
    stack_int_body* current_stack = stack->last_stack;
    if (current_stack->current_size == current_stack->stack_size)
    {
        int new_stack_size;
        switch (stack->expansion_type)
        {
            case STACK_EXPANSION_DOUBLE:
                new_stack_size = current_stack->stack_size * 2 + 1;
                break;
            case STACK_EXPANSION_FIXED:
            default:
                new_stack_size = current_stack->stack_size;
                break;
        }
        current_stack = new_stack_int_body(current_stack, new_stack_size);
        stack->last_stack = current_stack;
    }

    current_stack->data[current_stack->current_size] = value;
    current_stack->current_size++;
    return 0;
}

// Returns 0 if stack is empty
static inline int pop_stack_int(stack_int* const stack)
{
    stack_int_body* current_stack = stack->last_stack;
    if (current_stack->current_size == 0)
    {
        if (stack->first_stack == stack->last_stack) return 0; // stack is empty

        stack->last_stack = current_stack->prev_stack;
        clean_stack_int_body(current_stack);
        free(current_stack);
        current_stack = stack->last_stack;
    }

    current_stack->current_size--;
    return current_stack->data[current_stack->current_size];
}

#endif