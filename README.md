This is a simple collection of standardised and reusable code blocks found to b useful for mysel across multiple projects in C and Python.

## Currently Supported Objects (C)

### String
Custom implementation of the String object.
- Append, write, insert
- len, getChar
- Copy, compare
- Each function allows for char array (w/ or w/o length) as a second input as an option instead of a second String
- Convert String to integers via various base notations
- Append integers of various base notations to Strings

### Dynamic Array
A custom implementation of a dynamically size adjustng array
- Option between FIXED and DOUBLE resizing strategies
- Append, Pop, Insert functionality
- Various types (both for keys or values)
    - chars, Strings
    - int, unsigned int
    - uint8_t, uint16_t, uint32_t, uint64_t
    - vector2, vector3, vector4
    - matrix_2x2, matrix_3x3, matrix_4x4
    - custom (when given the needed item_size)
- Get item, Get byte given index

### Dictionary
A basic key-value pair dictionary built on adjustable cuckoo hashing parameters and min-load linked-list buckets for overflow.
- Options for Shallow-copying and Deep-copying
- Various types (both for keys or values)
    - Strings
    - int, unsigned int
    - uint8_t, uint16_t, uint32_t, uint64_t
    - vector2, vector3, vector4
    - matrix_2x2, matrix_3x3, matrix_4x4
    - custom (when given the needed size, copying function [for deep-copy], and cleaning function [for deep-copy])
- Capacity for various hash functions
    - XXH3 is currently the only one implemented
- Retrieve all key-value pairs
- Get, update, and delete a value given a key
- Insert a key-value pair
- Clean and Free dictionary functions

### Hashing
Currently supporting various hashing algorithms
- SHA-2 based
    - SHA-256
    - SHA-512
- XXH3
    - with custom seed and secret options

### Primes
- The smallest prime number greater than x
- The first n prime numbers
- is_prime

### Queue & Stack
Basic linked list implementations with primitive memory allocation reduction strategies

### File & Directory Handling
Custom file and directory handling with custom cross-platform architecture
- String splitting
- Value Extraction
- Formatted Read & Write
- Custom formatting

### Vectors & Matrices
Basic implmentation of some vectors and matrices
- vector2, vector3, vector4
- matrix_2x2, matrix_3x3, matrix_4x4
- Set matrix, perspective, scale and transform matrices
- Matrix multiplication
- Dot product

### Type Conversions
Currently supported type conversions
- String to dyn_array (chars)
- dyn_array (chars) to String
- dyn_array (uint [any]) to String using base notation
- uint8_t to String using base notation
- uint16_t to String using base notation
- uint32_t to String using base notation
- uint64_t to String using base notation
- String to uint64_t using base notation
- int to dyn_array [int] using base notation
- unsigned int to dyn_array [unsigned int] using base notation
- uint8_t to dyn_array [uint8_t] using base notation
- uint16_t to dyn_array [uint16_t] using base notation
- uint32_t to dyn_array [uint32_t] using base notation
- uint64_t to dyn_array [uint64_t] using base notation
- ptr to dyn_array [uint64_t] using base notation
- vector2 to dyn_array [vector2] using base notation
- vector3 to dyn_array [vector3] using base notation
- vector4 to dyn_array [vector4] using base notation
- matrix_2x2 to dyn_array [matrix_2x2] using base notation
- matrix_3x3 to dyn_array [matrix_3x3] using base notation
- matrix_4x4 to dyn_array [matrix_4x4] using base notation

