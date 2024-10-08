#include "oom.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#undef malloc
#undef calloc
#undef free

/// @brief Remaing calls before malloc return NULL
size_t remaining_malloc_allocations = 0;

/// @brief Remaing calls before calloc return NULL
size_t remaining_calloc_allocations = 0;

/// @brief if malloc should only fail once
bool should_oom_malloc = false;

/// @brief if calloc should only fail once
bool should_oom_calloc = false;

/// @brief if malloc should fail more than once
bool oom_toggled_malloc = false;

/// @brief if calloc should fail more than once
bool oom_toggled_calloc = false;

extern void *__real_malloc(size_t size);
extern void *__real_calloc(size_t count, size_t size);
extern void __real_free(void *ptr);

/// @brief Simulate failure of malloc every other call
/// @param size number of bytes to allcoate space for in the heap
/// @return pointer to heap allocated space for `size` bytes
void *__wrap_malloc(size_t size)
{
    // returns NULL if should_oom_malloc or oom_toggled_malloc is true
    if (should_oom_malloc || oom_toggled_malloc)
    {
        // And there are no calls left allowed
        if (remaining_malloc_allocations == 0)
        {
            // reset one time trigger
            // If toggle is true next call will still fail
            should_oom_malloc = false;
            return NULL;
        }
        else
        {

            // Reduce allowed calls by 1
            remaining_malloc_allocations--;
        }
    }
    // malloc was allowed to run normaly
    return __real_malloc(size);
}

/// @brief Simulate failure of calloc every other call
/// @param count number of elements
/// @param size size of each element in bytes
/// @return pointer to heap allocated space for `count * size` bytes which all have been cleared
void *__wrap_calloc(size_t count, size_t size)
{
    // returns NULL if should_oom_calloc or oom_toggled_calloc is true
    if (should_oom_calloc || oom_toggled_calloc)
    {
        // And there are no calls left allowed
        if (remaining_calloc_allocations == 0)
        {
            // reset one time trigger
            // If toggle is true next call will still fail
            should_oom_calloc = false;
            return NULL;
        }
        else
        {
            // Reduce allowed calls by 1
            remaining_calloc_allocations--;
        }
    }
    // calloc was allowed to run normaly
    return __real_calloc(count, size);
}

/// @brief Normal free function, no changes made
/// @param ptr to location in the heap to free
void __wrap_free(void *ptr)
{
    __real_free(ptr);
}

void oom_next_malloc(bool toggle)
{
    oom_nth_malloc_call(0, toggle);
}

void oom_nth_malloc_call(size_t num, bool toggle)
{
    should_oom_malloc = true;
    oom_toggled_malloc = toggle;
    remaining_malloc_allocations = num;
}

void set_normal_malloc(void)
{
    oom_toggled_malloc = false;
    should_oom_malloc = false;
    remaining_malloc_allocations = 0;
}

void oom_next_calloc(bool toggle)
{
    oom_nth_calloc_call(0, toggle);
}

void oom_nth_calloc_call(size_t num, bool toggle)
{
    should_oom_calloc = true;
    oom_toggled_calloc = toggle;
    remaining_calloc_allocations = num;
}

void set_normal_calloc(void)
{
    oom_toggled_calloc = false;
    should_oom_calloc = false;
    remaining_calloc_allocations = 0;
}
