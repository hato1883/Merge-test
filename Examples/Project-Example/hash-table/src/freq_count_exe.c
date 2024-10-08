#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "hash_table.h"
#include "iterator.h"
#include "linked_list.h"

#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
    bool found_key = false;
    bool found_value = false;
    elem_t return_value;
    ioopm_hash_table_has_key(&found_key, ht, ptr_elem(word));
    ioopm_hash_table_lookup(&found_value, &return_value, ht, ptr_elem(word));

    int freq = found_key ? (return_value).integer : 0;
    if (freq != 0)
    {
        ioopm_hash_table_insert(ht, ptr_elem(word), int_elem(freq + 1));
    }
    else
    {
        ioopm_hash_table_insert(ht, ptr_elem(strdup(word)), int_elem(freq + 1));
    }
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
    FILE *f = fopen(filename, "r");

    while (true)
    {
        char *buf = NULL;
        size_t len = 0;
        getline(&buf, &len, f);

        if (feof(f))
        {
            free(buf);
            break;
        }
        for (char *word = strtok(buf, Delimiters); word && *word; word = strtok(NULL, Delimiters))
        {
            process_word(word, ht);
        }
        free(buf);
    }
    fclose(f);
}

size_t string_sum_hash(elem_t e)
{
    char *str = e.unknown_pointer;
    size_t result = 0;
    do
    {
        result += *str;
    } while (*++str != '\0');
    return result;
}

bool string_eq(elem_t e1, elem_t e2)
{
    return (strcmp((char *)e1.unknown_pointer, (char *)e2.unknown_pointer) == 0);
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        ioopm_hash_table_t *ht = NULL;
        ioopm_create_hash_table(&ht, 20, string_sum_hash, string_eq, NULL);
        for (int i = 1; i < argc; ++i)
        {
            process_file(argv[i], ht);
        }
        // get keys
        ioopm_list_t *keys = NULL;
        ioopm_hash_table_keys(&keys, ht);

        // convert keys to an array using an iterator
        ioopm_list_iterator_t *key_iter = NULL;
        ioopm_list_iterator(&key_iter, keys);
        size_t size = 0;
        ioopm_hash_table_size(&size, ht);
        char *keys_array[size];
        for (size_t index = 0; index < size; index++)
        {
            elem_t current;
            ioopm_iterator_next(&current, key_iter);
            keys_array[index] = current.unknown_pointer;
        }

        sort_keys(keys_array, size);
        for (size_t i = 0; i < size; ++i)
        {
            bool found = false;
            elem_t return_value;
            ioopm_hash_table_lookup(&found, &return_value, ht, ptr_elem(keys_array[i]));
            int freq = (return_value).integer;
            printf("\"%s\": %d\n", keys_array[i], freq);
        }

        ioopm_iterator_destroy(&key_iter);
        ioopm_linked_list_destroy(&keys);
        ioopm_hash_table_destroy(&ht);
        for (size_t i = 0; i < size; i++)
        {
            free(keys_array[i]);
        }
    }
    else
    {
        puts("Usage: freq-count file1 ... filen");
    }
}