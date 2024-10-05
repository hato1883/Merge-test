---
name: SIMPLE Module
about: Template for creating a SIMPLE Module
title: "[Module] "
labels: ''
assignees: ''

---

# <File_Name.c>

## Purpose
What will this module do?  
What will it handle



## Public interface   


### `data_struct_t *ioopm_create_some_data_structure(void *some_argument, int or_more, error_t *error)`
##### Is Completed
- [x] issue link here

##### Responsibility
- briefly describe purpose.  
Example:  
_Creates a data_struct_t with size of `or_more` input. Used to abstract allocation for internal linked list structure._

##### Abstraction Level
- Tier:  
Choose on of the following:
  - 5 used in UI to interact with data
  - 4 used when deciding how to store data from UI (hash table, list, stack, etc)
  - 3 used when operating on a given data structure (hash table, list, stack, etc) (Ex: `add_item_to_hash_table(ht, item)`)
  - 2 used to implement / create data structures (`ht->next = new_link`)
  - 1 used to define helper functions that act bewteen system and data structure (Ex: `while(c && c != 'EOF') { str[i] = c; c = getchar() }`)  

  Note: a Tier 3 abstraction level only uses the same level or one bellow.  
  If a function is Tier 5 and uses some thing on Tier 3, 2 or 1, then it is time to rethink implementation.

##### Assumptions
- `x != NULL`
- `y >= 0`

##### Returns
- Returns `data_struct_t *` with size `or_more`
- Reports following error code in `error`
  - `ARGUMENT_IS_NULL` error
  - `SIZE_TO_SMALL` error


### `void ioopm_destroy_some_data_structure(void * some_argument)`
##### Is Completed
- [x] issue link here
##### Responsibility
- Text here

##### Abstraction Level
- Tier: 1, 2, 3, 4, 5

##### Assumptions
- `assumption here`

##### Returns
- Returns `...` with some_attribute_here `...`
- Reports following error code in `some_error_variable_pointer`
  - `Error_1` error
  - `Error_2` error



## Private interface

### `int get_int_data_from_struct(some_struct_t *some_argument)`
##### Is Completed
- [x] issue link here  

##### Responsibility  
- Text here

##### Abstraction Level  
- Tier: 1, 2, 3, 4, 5

##### Assumptions  
- `assumption here`

##### Returns
- Returns `...` with some_attribute_here `...`
- Reports following error code in `some_error_variable_pointer`
  - `Error_1` error
  - `Error_2` error



## Depends on
- [ ] <File_Name.h>

- [ ] <some_data_structure.h>  
  - [ ] `<ioopm_some_public_function>` (from file some_data_structure.c)
  - [ ]  `<ioopm_some_public_other_function>` (from file some_data_structure.c)  

- [ ] <some_utility.h>
  - [ ] `<ioopm_some_public_function>` (from file some_utility.c)  

- [ ] <some_inteface.h>
  - [ ]  `<ioopm_some_public_struct>` (from file some_inteface.c)
