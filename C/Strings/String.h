#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

#ifndef min
    #define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
    #define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#define MIN_STRING_ARR_LEN 32

#define UNICODE_DIGIT_BASE 48
#define UNICODE_UPPER_ALPHA_BASE 65

typedef struct __String String;

struct __String
{
    int str_length;
    int arr_length;
    char* string;
};

// Returns the lowest power of two greater than or equal to `x`
// Internal use only
static inline int __next_pow_2__(int x) {
    // Can be optimised to fit int size with less instructions
    int y = x;
    while (x &= (x^(~x+1))) 
        y = x << 1;
    return y;
}

// To be used rarely with extreme caution
// Internal use only
static inline void __set_arr_length__(struct __String* const str, const int length)
{
    if (str->arr_length != length)
    {
        str->arr_length = length;
        str->string = (char*) realloc(str->string, str->arr_length * sizeof(char));
    }
}

// Internal use only
static inline void __set_arr_to_min_str_length__(struct __String* const str, const int str_length)
{
    const int new_length = max(MIN_STRING_ARR_LEN, __next_pow_2__(str_length+1));
    if (str->arr_length != new_length)
    {
        str->arr_length = new_length;
        str->string = (char*) realloc(str->string, str->arr_length * sizeof(char));
        str->string[str->arr_length - 1] = '\0'; // FOR SAFETY, is performance neg.ve
    }
}

// Internal use only
static inline void __increase_arr_length__(struct __String* const str)
{
    str->arr_length *= 2;
    str->string = (char*) realloc(str->string, str->arr_length * sizeof(char));
}

// ASSUMES that str_length < arr_length/2
// Internal use only
static inline void __decrease_arr_length__(struct __String* const str)
{
    str->arr_length = (str->arr_length > 1) ? (str->arr_length / 2) : 1;
    str->string = (char*) realloc(str->string, str->arr_length * sizeof(char));
    str->string[str->arr_length - 1] = '\0'; // FOR SAFETY
}

//#######################################################################################

static inline struct __String* newEmptyString(void)
{
    struct __String* new_str = (struct __String*) malloc(sizeof(struct __String));
    new_str->arr_length = 1;
    new_str->str_length = 0;
    new_str->string = (char*) malloc(sizeof(char));
    new_str->string[0] = '\0';
    return new_str;
}

// CAREFUL!!! this MUST be treated as atomic
static inline struct __String* newString(const char* const char_arr)
{
    struct __String* new_str = (struct __String*) malloc(sizeof(struct __String));
    new_str->arr_length = 1;
    new_str->str_length = 0;
    new_str->string = (char*) malloc(sizeof(char)); // needed for realloc

    const char* c = char_arr;
    while (*c)
    {
        if (++new_str->str_length == new_str->arr_length) __increase_arr_length__(new_str);
        new_str->string[new_str->str_length - 1] = *c;
        c += sizeof(char);
    }
    new_str->string[new_str->str_length] = '\0';

    return new_str;
}

// Don't include null terminating character in length '\0'
static inline struct __String* newStringN(const char* const char_arr, const int length)
{
    struct __String* new_str = (struct __String*) malloc(sizeof(struct __String));
    new_str->arr_length = 1; // in case of malloc default value collision used for `set __set_arr_to_min_str_length__()`
    new_str->str_length = length;
    new_str->string = (char*) malloc(sizeof(char)); // needed for realloc
    __set_arr_to_min_str_length__(new_str, length); // could expand this to optimise perf.

    for (int i = 0; i < new_str->str_length; i++) new_str->string[i] = char_arr[i];

    new_str->string[new_str->str_length] = '\0';

    return new_str;
}

// Could technically be optimised
static inline struct __String* copyString(const struct __String* const str)
{
    return newStringN(getCharArr(str), lenString(str));
}

static inline int lenString(const struct __String* const str)
{
    return str->str_length;
}

static inline char getCharIndexed(const struct __String* const str, const int index)
{
    return index < str->str_length ? str->string[index] : -1;
}

// DO NOT MODIFY EXTERNALLY; can cause bugs/security issues
static inline char* getCharArr(const struct __String* const str)
{
    return str->string;
}

static inline void clearString(struct __String* const str)
{
    str->str_length = 0;
    __set_arr_to_min_str_length__(str, str->str_length);
    str->string[0] = '\0';
}

static inline void writeChars(struct __String* const str, const char* const char_arr)
{
    str->str_length = 0;

    const char* c = char_arr;
    while (*c)
    {
        if (++str->str_length == str->arr_length) __increase_arr_length__(str);
        str->string[str->str_length - 1] = *c;
        c += sizeof(char);
    }

    str->string[str->str_length] = '\0';
    __set_arr_to_min_str_length__(str, str->str_length); // in case the new str is smaller
}

static inline void writeCharsN(struct __String* const str, const char* const char_arr, const int length)
{
    __set_arr_to_min_str_length__(str, length);
    str->str_length = length;

    for (int i = 0; i < length; i++) str->string[i] = char_arr[i];

    str->string[str->str_length] = '\0';
}

static inline void writeString(struct __String* const str1, const struct __String* const str2)
{
    __set_arr_to_min_str_length__(str1, str2->str_length);
    str1->str_length = str2->str_length;

    for (int i = 0; i < str2->str_length; i++) str1->string[i] = str2->string[i];

    str1->string[str1->str_length] = '\0';
}

static inline void writeStringN(struct __String* const str1, const struct __String* const str2, const int length)
{
    __set_arr_to_min_str_length__(str1, length);
    str1->str_length = length;

    for (int i = 0; i < length; i++) str1->string[i] = str2->string[i];

    str1->string[str1->str_length] = '\0';
}

static inline void writeStringNOffset(struct __String* const str1, const struct __String* const str2, const int length, const int offset)
{
    __set_arr_to_min_str_length__(str1, length);
    str1->str_length = length;

    for (int i = 0; i < length; i++) str1->string[i] = str2->string[i+offset];

    str1->string[str1->str_length] = '\0';
}

static inline void appendChar(struct __String* const str, const char c)
{
    __set_arr_to_min_str_length__(str, ++str->str_length);

    str->string[str->str_length - 1] = c;
    str->string[str->str_length] = '\0';
}

static inline void appendChars(struct __String* const str, const char* const char_arr)
{
    const char* c = char_arr;
    while (*c)
    {
        if (++str->str_length == str->arr_length) __increase_arr_length__(str);
        str->string[str->str_length - 1] = *c;
        c += sizeof(char);
    }

    str->string[str->str_length] = '\0';
}

static inline void appendCharsN(struct __String* const str, const char* const char_arr, const int length)
{
    __set_arr_to_min_str_length__(str, str->str_length+length);

    for (int i = 0; i < length; i++) str->string[str->str_length++] = char_arr[i];

    str->string[str->str_length] = '\0';
}

static inline void appendString(struct __String* const str1, const struct __String* const str2)
{
    __set_arr_to_min_str_length__(str1, str1->str_length+str2->str_length);

    for (int i = 0; i < str2->str_length; i++) str1->string[str1->str_length++] = str2->string[i];

    str1->string[str1->str_length] = '\0';
}

static inline void appendStringN(struct __String* const str1, const struct __String* const str2, const int length)
{
    __set_arr_to_min_str_length__(str1, str1->str_length+length);

    for (int i = 0; i < length; i++) str1->string[str1->str_length++] = str2->string[i];

    str1->string[str1->str_length] = '\0';
}

static inline void insertChar(struct __String* const str, const int index, const char c)
{
    __set_arr_to_min_str_length__(str, ++str->str_length);

    char buf = str->string[index];
    str->string[index] = c;

    const int max_i = str->str_length;
    for (int i = (index+1); i < max_i; i++)
    {
        const char tmp = str->string[i];
        str->string[i] = buf;
        buf = tmp;
    }

    str->string[str->str_length] = '\0';
}

static inline void insertChars(struct __String* const str, const int index, const char* const char_arr)
{
    int buf_length = 2;
    int buf_str_length = 0;
    char* buf = (char*) malloc(sizeof(char) * buf_length);

    int i = 0;
    const char* c = char_arr;
    while (*c)
    {
        if (++str->str_length == str->arr_length) __increase_arr_length__(str); // may not be needed immediately but at least needed later
        if (++buf_str_length == buf_length) 
        {
            buf_length *= 2;
            buf = (char*) realloc(buf, buf_length * sizeof(char));
        }
        const int x = index + i;
        buf[i] = str->string[x];
        str->string[x] = *c;

        c += sizeof(char);
        i++;
    }

    const int length = i; // AT THIS POINT
    const int max_i = str->str_length - index; // str->str_length is updated as the insert occurs above
    for (; i < max_i; i++)
    {
        const int x = i + index;
        const int y = i % length;

        const char tmp = str->string[x];
        str->string[x] = buf[y];
        buf[y] = tmp;
    }

    str->string[str->str_length] = '\0';
    free(buf);
}

static inline void insertCharsN(struct __String* const str, const int index, const char* const char_arr, const int length)
{
    __set_arr_to_min_str_length__(str, str->str_length+length);
    char* buf = (char*) malloc(sizeof(char) * length);

    int i = 0;
    for (; i < length; i++)
    {
        const int x = i + index;
        buf[i] = str->string[x];
        str->string[x] = char_arr[i];
    }

    str->str_length += length;
    const int max_i = str->str_length - index;
    for (; i < max_i; i++)
    {
        const int x = i + index;
        const int y = i % length;

        const char tmp = str->string[x];
        str->string[x] = buf[y];
        buf[y] = tmp;
    }

    str->string[str->str_length] = '\0';
    free(buf);
}

static inline void insertString(struct __String* const str1, const int index, const struct __String* const str2)
{
    const int length = str2->str_length; // simplifies codes for coder (maybe performance neg.ve?)
    __set_arr_to_min_str_length__(str1, str1->str_length+length);
    char* buf = (char*) malloc(sizeof(char) * length);

    int i = 0;
    for (; i < length; i++)
    {
        const int x = i + index;
        buf[i] = str1->string[x];
        str1->string[x] = str2->string[i];
    }

    str1->str_length += length;
    const int max_i = str1->str_length - index;
    for (; i < max_i; i++)
    {
        const int x = i + index;
        const int y = i % length;

        const char tmp = str1->string[x];
        str1->string[x] = buf[y];
        buf[y] = tmp;
    }

    str1->string[str1->str_length] = '\0';
    free(buf);
}

// Inserts `length` chars from `str2` into `str1` at position `index` inclusive
static inline void insertStringN(struct __String* const str1, const int index, const struct __String* const str2, const int length)
{
    __set_arr_to_min_str_length__(str1, str1->str_length+length);
    char* buf = (char*) malloc(sizeof(char) * length);

    int i = 0;
    for (; i < length; i++)
    {
        const int x = i + index;
        buf[i] = str1->string[x];
        str1->string[x] = str2->string[i];
    }

    str1->str_length += length;
    const int max_i = str1->str_length - index;
    for (; i < max_i; i++)
    {
        const int x = i + index;
        const int y = i % length;

        const char tmp = str1->string[x];
        str1->string[x] = buf[y];
        buf[y] = tmp;
    }

    str1->string[str1->str_length] = '\0';
    free(buf);
}

static inline void deleteString(struct __String* str)
{
    free(str->string);
    str->string = NULL; // Technically not needed but whatev ig?
    free(str);
}

// returns 0 when the strings are equal
static inline int compareChars(const struct __String* const str, const char* const char_arr)
{
    const char* c = char_arr;
    int i = 0;
    while (*c && i < str->str_length)
    {
        if (*c != str->string[i]) return ((int)(str->string[i]) - (int)(*c));
        c += sizeof(char);
        i++;
    }
    
    if (*c == NULL && i == str->str_length) return 0;
    else if (*c == NULL) return 1;
    else return -1;
}

// returns 0 when the strings are equal
static inline int compareCharsN(const struct __String* const str, const char* const char_arr, const int length)
{
    const int max_len = min(length, str->str_length);
    
    for (int i = 0; i < max_len; i++)
        if (char_arr[i] != str->string[i]) 
            return ((int)(str->string[i]) - (int)(char_arr[i]));
    
    return str->str_length - length;
}

// returns 0 when the strings are equal
static inline int compareString(const struct __String* const str1, const struct __String* const str2)
{
    const int max_len = min(str1->str_length, str2->str_length);

    for (int i = 0; i < max_len; i++)
        if (str1->string[i] != str2->string[i]) 
            return ((int)(str1->string[i]) - (int)(str2->string[i]));
    
    return str1->str_length - str2->str_length;
}

// TODO make an optimised digit only func
// Internal use only
static inline void __append_int_base_func__(struct __String* const str, const int number, const int base)
{
    int rem;
    if (number < 0) 
    {
        appendChar(str, '-');
        rem = -1 * number;
    }
    else 
    {
        rem = number;
    }

    int pow = 1;
    int i = 0;
    while (pow <= rem)
    {
        pow *= base;
        i++;
    }
    __set_arr_to_min_str_length__(str, str->str_length+i);

    i--;
    pow /= base;
    for (; i > 0; i--)
    {
        const int dig = rem / pow;
        rem %= pow;
        str->string[str->str_length++] = (char) (dig >= 10 
            ? (UNICODE_DIGIT_BASE + dig) 
            : ((UNICODE_UPPER_ALPHA_BASE - 10) + dig));
    }

    str->string[str->str_length] = '\0';
}

// Presumes >= 0; TODO make an optimised digit only func
// Internal use only
static inline int __convert_string_to_int_base_func__(const struct __String* const str, const int str_offset, const int base)
{
    int result = 0;

    int i = str_offset;
    while (i < str->str_length)
    {
        const int c_val = (int)str->string[i];
        if (c_val < UNICODE_DIGIT_BASE 
            || (c_val > (UNICODE_DIGIT_BASE + 9) 
                && (c_val < UNICODE_UPPER_ALPHA_BASE))
            || c_val > (UNICODE_UPPER_ALPHA_BASE + 25)
        ) break; // safety catch

        result = (result*base) + (
            c_val <= (UNICODE_DIGIT_BASE + 9)
            ? c_val - UNICODE_DIGIT_BASE
            : c_val - (UNICODE_UPPER_ALPHA_BASE - 11)
        );
        i++;
    }

    return result;
}

static inline void appendIntOct(struct __String* const str, const int number)
{
    __append_int_base_func__(str, number, 8);
}

static inline void appendIntDec(struct __String* const str, const int number)
{
    __append_int_base_func__(str, number, 10);
}

static inline void appendIntHex(struct __String* const str, const int number)
{
    __append_int_base_func__(str, number, 16);
}

static inline void appendIntB32(struct __String* const str, const int number)
{
    __append_int_base_func__(str, number, 32);
}

// Presumes >= 0
static inline int convertStringToIntOct(const struct __String* const str, const int str_offset)
{
    return __convert_string_to_int_base_func__(str, str_offset, 8);
}

// Presumes >= 0
static inline int convertStringToIntDec(const struct __String* const str, const int str_offset)
{
    return __convert_string_to_int_base_func__(str, str_offset, 10);
}

// Presumes >= 0
static inline int convertStringToIntHex(const struct __String* const str, const int str_offset)
{
    return __convert_string_to_int_base_func__(str, str_offset, 16);
}

// Presumes >= 0
static inline int convertStringToIntB32(const struct __String* const str, const int str_offset)
{
    return __convert_string_to_int_base_func__(str, str_offset, 32);
}

#endif