#pragma once
#include <stdbool.h>

#include "common.h"
#include "linked_list.h"

#define INVALID_ITERATOR_POINTER 31
#define INVALID_ITERATOR_STATE 32
#define ITERATOR_IS_EMPTY 33

/// @brief Check if there are more elements to iterate over.
/// @param return_has_next The result as to whether there is a next element is returned via this parameter.
/// @param iter The iterator.
/// @return The exit status of the function:
/// - SUCCESS:                      function call successful
/// - INVALID_ITERATOR_POINTER:     iter is NULL (invalid pointer)
/// - INVALID_RETURN_POINTER:       return_has_next is NULL (invalid pointer)
int ioopm_iterator_has_next(bool *return_has_next, const ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step.
/// @param return_next The next element in the iterator is returned via this parameter.
/// @param iter The iterator.
/// @return The exit status of the function:
/// - SUCCESS:                      next element retrived successfuly
/// - INVALID_ITERATOR_POINTER:     iter is NULL (invalid pointer)
/// - INVALID_RETURN_POINTER:       return_next is NULL (invalid pointer)
/// - ITERATOR_IS_EMPTY:            iterator is empty
int ioopm_iterator_next(elem_t *return_next, ioopm_list_iterator_t *iter);

/// @brief Remove the current element from the underlying list.
/// @param return_removed The removed element is returned via this parameter.
/// @param iter the iterator
/// @return The exit status of the function:
/// - SUCCESS:                      elem_t removed successfuly
/// - INVALID_ITERATOR_POINTER:     iter is NULL (invalid pointer)
/// - INVALID_RETURN_POINTER:       return_removed is NULL (invalid pointer)
/// - INVALID_ITERATOR_STATE:       iterator is not in a vaild state to remove a elem_t
int ioopm_iterator_remove(elem_t *return_removed, ioopm_list_iterator_t *iter);

/// @brief Insert a new element into the underlying list making the current element it's next.
/// @param iter The iterator.
/// @param element The element to be inserted.
/// @return The exit status of the function:
/// - SUCCESS:                      elem_t inserted successfuly
/// - INVALID_ITERATOR_POINTER:     iter is NULL (invalid pointer)
/// - INVALID_ITERATOR_STATE:       iterator is not in a vaild state to insert a elem_t
/// - MEMORY_ALLOCATION_FAILURE:    memory allocation failed during node creation
int ioopm_iterator_insert(ioopm_list_iterator_t *iter, const elem_t element);

/// @brief Reposition the iterator at the start of the underlying list.
/// @param iter The iterator.
/// @return The exit status of the function:
/// - SUCCESS:                      iter reset successfuly
/// - INVALID_ITERATOR_POINTER:     iter is NULL (invalid pointer)
int ioopm_iterator_reset(ioopm_list_iterator_t *iter);

/// @brief Return the current element from the underlying list.
/// @param return_current The current element is returned via this parameter.
/// @param iter The iterator.
/// @return The exit status of the function:
/// - SUCCESS:                      current elem_t retrived successfuly
/// - INVALID_ITERATOR_POINTER:     iter is NULL (invalid pointer)
/// - INVALID_RETURN_POINTER:       return_current is NULL (invalid pointer)
/// - INVALID_ITERATOR_STATE:       iterator is not in a vaild state to retrive current elem_t from
int ioopm_iterator_current(elem_t *return_current, const ioopm_list_iterator_t *iter);

/// @brief Destroy the iterator and return its resources
/// @param iter pointer to the iterator to destroy. Sets iterator pointer to NULL if successful
/// @return the exit status of the function:
/// - SUCCESS:                      elem_t removed successfuly
/// - INVALID_ITERATOR_POINTER:     iter is NULL (invalid pointer)
int ioopm_iterator_destroy(ioopm_list_iterator_t **iter);