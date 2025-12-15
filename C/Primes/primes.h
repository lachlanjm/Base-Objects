#ifndef PRIMES_H
#define PRIMES_H

#include "../Dynamic Array/dyn_array.h"
#include "../Stack & Queue/stack.h"
#include "../Stack & Queue/queue.h"

// Returns 1 if prime, 0 if not prime
static inline int is_prime(const unsigned int number)
{
    if (number < 2) return 0;
    if (number == 2) return 1;
    if (number % 2 == 0) return 0;

    for (unsigned int i = 3; i * i <= number; i += 2)
    {
        if (number % i == 0) return 0;
    }

    return 1;
}

// Returns the next prime number greater than or equal to 'start'
static inline unsigned int next_prime(unsigned int start)
{
    if (start <= 2) return 2;
    if (start % 2 == 0) start++; // make it odd

    while (!is_prime(start))
    {
        start += 2; // check only odd numbers
    }

    return start;
}

/** Fills a dynamic array with prime numbers up to \p count primes
 * @param primes_array Pointer to a dynamic array of int (not int*); will be initialized within the function
 * @param count Number of prime numbers to generate
 * @return int 0 on success, 1 on failure
 * @warning All contents in \p primes_array will be cleared and the caller is responsible for cleaning up the dynamic array using clean_dyn_array() when done
 * */
static inline int generate_primes(dyn_array* const primes_array, const unsigned int count)
{
    if (count == 0) return 1;
    if (primes_array == NULL) return 1;
    
    clean_dyn_array(primes_array);
    set_dyn_array(primes_array, DYN_ARRAY_INT_TYPE, DYN_ARRAY_EXPANSION_FIXED);

    add_slot_dyn_array(primes_array);
    dyn_get_int(primes_array->data, 0) = 2;
    unsigned int num_found = 1;
    unsigned int candidate = 3;

    while (num_found < count)
    {
        int is_prime_b = 1;
        for (int i = 0; i < primes_array->current_size; i++)
        {
            const unsigned int prime = dyn_get_int(primes_array->data, i);
            if (candidate % prime == 0) 
            {
                is_prime_b = 0;
                break;
            }
        }
        if (is_prime_b)
        {
            add_slot_dyn_array(primes_array);
            dyn_get_last_int(primes_array) = candidate;
            num_found++;
        }
        candidate += 2; // check only odd numbers
    }
    return 0;
}

#endif