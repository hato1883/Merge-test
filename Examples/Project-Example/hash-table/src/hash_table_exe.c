#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

bool integer_eq(elem_t a, elem_t b)
{
    return a.integer == b.integer;
}

bool string_eq(elem_t a, elem_t b)
{
    return strcmp((char *)a.unknown_pointer, (char *)b.unknown_pointer) == 0;
}

size_t integer_hash(const elem_t key)
{
    return (size_t)key.integer;
}

/// The starting point of any C program
int main(void)
{

    /// Create an array of 10 elements, of which we are going to use 3
    size_t map_size = 17;
    ioopm_hash_table_t *ht = NULL;
    ioopm_create_hash_table(&ht, map_size, integer_hash, integer_eq, string_eq);
    const int iterations = 100000;
    for (int i = 0; i < iterations; i++)
    {
        ioopm_hash_table_insert(ht, int_elem(i), ptr_elem("Profiling..."));
    }
    elem_t ret;
    bool found = false;
    for (int i = 0; i < iterations; i++)
    {
        ioopm_hash_table_lookup(&found, &ret, ht, int_elem(i));
    }

    ioopm_hash_table_destroy(&ht);
    return 0;
}