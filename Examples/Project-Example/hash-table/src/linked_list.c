#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "iterator.h"
#include "linked_list.h"

typedef struct node node_t;
struct node
{
    elem_t entry; // An entry
    node_t *next; // Pointer to the next node in the linked list
};

struct list
{
    node_t *first;              // pointer to head of the linked list
    node_t *last;               // pointer to last link in the linked list
    size_t size;                // the number of links in the list
    ioopm_eq_function *eq_func; // function used to compare stored values with
};

static void initialize_list(ioopm_list_t **list, ioopm_eq_function *eq_func);
static int create_node(node_t **new_node, const elem_t entry, const node_t *next);
static node_t **get_ptr_to_index(node_t **head, const int index);
static node_t **get_ptr_before_index(node_t **head, const int index);

/////////////////////////////////////////////////
///////////     LINKED LIST      ////////////////
/////////////////////////////////////////////////

int ioopm_linked_list_create(ioopm_list_t **return_list, ioopm_eq_function *eq_func)
{
    // Check presumptions
    if (return_list == NULL)
    {
        return INVALID_LIST_POINTER;
    }

    *return_list = malloc(1 * sizeof(ioopm_list_t));

    if (*return_list == NULL)
    {
        return MEMORY_ALLOCATION_FAILURE;
    }

    initialize_list(return_list, eq_func);

    return SUCCESS;
}

int ioopm_linked_list_destroy(ioopm_list_t **list)
{
    // Check presumptions
    if (list == NULL || *list == NULL)
    {
        return INVALID_LIST_POINTER;
    }

    // Iterate through linked list, freeing each node
    ioopm_linked_list_clear(*list);

    // Free linked list
    free(*list);
    *list = NULL;

    return SUCCESS;
}

int ioopm_linked_list_append(ioopm_list_t *list, const elem_t element)
{
    size_t size = 0;
    int status = ioopm_linked_list_size(&size, list);

    if (status == SUCCESS)
    {
        return ioopm_linked_list_insert(list, size, element);
    }

    return status;
}

int ioopm_linked_list_prepend(ioopm_list_t *list, const elem_t element)
{
    return ioopm_linked_list_insert(list, 0, element);
}

int ioopm_linked_list_insert(ioopm_list_t *list, const size_t index, const elem_t element)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }

    size_t size = 0;
    ioopm_linked_list_size(&size, list);
    if (index > size)
    {
        return INDEX_OUT_OF_BOUNDS;
    }

    // Want to insert into index 0 which is the list->first node pointer
    if (index > 0)
    {
        // List is not empty get pointer to node before our index
        node_t **prev = get_ptr_before_index(&(list->first), index);

        // Replace pointers next value with our new node
        // which in turn is updated to point to old next value
        node_t *new_node = NULL;
        if (create_node(&new_node, element, (*prev)->next) != SUCCESS)
        {
            return MEMORY_ALLOCATION_FAILURE;
        }
        (*prev)->next = new_node;

        // Check if we are insterting at the end of the list
        if (size == index)
        {
            // Update our last pointer to point to our new node
            list->last = (*prev)->next;
        }
    }
    else
    {
        // Check if list is empty
        node_t *new_node = NULL;
        if (size > 0)
        {
            // Not empty
            // Only need to insert a new link before first that points to the old link
            if (create_node(&new_node, element, list->first) != SUCCESS)
            {
                return MEMORY_ALLOCATION_FAILURE;
            }
        }
        else
        {
            // in that case set first and last to be equal to our new node
            if (create_node(&new_node, element, NULL) != SUCCESS)
            {
                return MEMORY_ALLOCATION_FAILURE;
            }
            list->last = new_node;
        }
        list->first = new_node;
    }

    list->size += 1;

    return SUCCESS;
}

int ioopm_linked_list_remove(elem_t *return_removed, ioopm_list_t *list, const size_t index)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }
    if (return_removed == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    size_t size = 0;
    ioopm_linked_list_size(&size, list);
    // Check if index is out of bounds
    if (index >= size)
    {
        return INDEX_OUT_OF_BOUNDS;
    }

    if (index > 0)
    {
        // Removing a link in list at some index
        node_t **prev = get_ptr_before_index(&(list->first), index);

        // *prev is the link before our removal target
        node_t *to_unlink = (*prev)->next;

        // Update return pointer for removed_element
        *return_removed = to_unlink->entry;

        // unlink previous from removal target
        (*prev)->next = to_unlink->next;

        // check if removal target is our last link
        if (index == (size - 1))
        {
            // Relink list->last to the one before
            list->last = *prev;
        }

        // Free memory of removal target
        free(to_unlink);
        list->size -= 1;

        // Points to a pointer to the previous node
        return SUCCESS;
    }
    else
    {
        // Removing first link
        node_t *to_unlink = list->first;
        *return_removed = to_unlink->entry;
        if (size > 1)
        {
            // We have more links,
            // Replace first link with the next link
            list->first = to_unlink->next;
        }
        else
        {
            // No more links in list,
            // Set to NULL
            list->first = NULL;
            list->last = NULL;
        }
        free(to_unlink);
        list->size -= 1;
        return SUCCESS;
    }

    // Can only get here if get_ptr_before_index gives us a pointer to NULL,
    // This can ONLY happen if the index given to the function is larger than number of elements,
    // But due to our if guard if (index >= size) this is impossible.

    // assert(false);
    // return UNSPECIFIED_FAILURE; // Should never get here, return error status
}

int ioopm_linked_list_get(elem_t *return_get, ioopm_list_t *list, const size_t index)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }
    if (return_get == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    size_t size = 0;
    ioopm_linked_list_size(&size, list);
    if (index >= size)
    {
        return INDEX_OUT_OF_BOUNDS;
    }

    // Points to a pointer to the previous node
    node_t **prev = get_ptr_to_index(&(list->first), index);

    *return_get = (*prev)->entry;
    return SUCCESS;

    // Can only get here if get_ptr_before_index gives us a pointer to NULL,
    // This can ONLY happen if the index given to the function is larger than number of elements,
    // But due to our if guard if (index >= size) this is impossible.

    // assert(false);
    // return UNSPECIFIED_FAILURE; // Should never get here, return error status
}

int ioopm_linked_list_contains(bool *return_contains, const ioopm_list_t *list, const elem_t element)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }
    if (return_contains == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    // Linearly search linked list
    node_t *node = list->first;
    while (node != NULL)
    {
        if (list->eq_func(node->entry, element))
        {
            *return_contains = true; // Key found
            return SUCCESS;
        }

        node = node->next; // Go to next node
    }
    *return_contains = false; // Key not found
    return SUCCESS;
}

size_t ioopm_linked_list_size(size_t *return_size, const ioopm_list_t *list)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }
    if (return_size == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    *return_size = list->size;

    return SUCCESS;
}

int ioopm_linked_list_is_empty(bool *return_empty, const ioopm_list_t *list)
{
    // Check if we have valid return adress
    if (return_empty == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    // try to get size
    size_t size = 0;
    int status = ioopm_linked_list_size(&size, list);
    if (status == SUCCESS)
    {
        // We got size, set return value
        *return_empty = size == 0;
        return SUCCESS;
    }
    // Something was wrong
    // Propegate the error:
    // INVALID_LIST_POINTER
    // INVALID_RETURN_POINTER
    return status;
}

int ioopm_linked_list_clear(ioopm_list_t *list)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }

    // Iterate through linked list, freeing each node
    node_t *node = list->first;
    node_t *next_node = NULL;
    while (node != NULL)
    {
        next_node = node->next;
        free(node);
        node = next_node;
    }

    // Reset fields
    list->first = NULL;
    list->last = NULL;
    list->size = 0;

    return SUCCESS;
}

int ioopm_linked_list_all(bool *return_all, const ioopm_list_t *list, ioopm_predicate *prop, const void *extra)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }
    if (return_all == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    node_t *node = list->first;

    bool result = true;
    while (node != NULL && result)
    {
        result = result && prop(node->entry, extra);
        node = node->next;
    }

    *return_all = result;
    return SUCCESS;
}

int ioopm_linked_list_any(bool *return_any, const ioopm_list_t *list, ioopm_predicate *prop, const void *extra)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }
    if (return_any == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    node_t *node = list->first;

    bool result = false;
    while (node != NULL && !result)
    {
        result = result || prop(node->entry, extra);
        node = node->next;
    }

    *return_any = result;
    return SUCCESS;
}

int ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_function *fun, void *extra)
{
    // Check presumptions
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }

    node_t *node_ptr = list->first;
    while (node_ptr != NULL)
    {
        // Save pointer to next before calling function
        // This allows the function destroy node without issue.
        node_t *next = node_ptr->next;
        fun(&(node_ptr->entry), extra);
        // Set next to be the new current
        node_ptr = next;
    }

    return SUCCESS;
}

/**
 * @brief Initializes the fields in a linked list.
 * @param list The list to be initialized.
 * @param list The list's eqivalence function.
 */
static void initialize_list(ioopm_list_t **list, ioopm_eq_function *eq_func)
{
    (*list)->first = NULL;
    (*list)->last = NULL;
    (*list)->size = 0;
    (*list)->eq_func = eq_func;
}

/**
 * @brief Creates a node in a linked list.
 * @param return_new_node A pointer to the newly created node_t * that points to our link is returned via this
 * parameter.
 * @param entry The node entry.
 * @param next The next link to point to.
 * @return The exit status of the function:
 * - SUCCESS:                      node created successfuly
 * - MEMORY_ALLOCATION_FAILURE:    memory allocation failed during creation
 */
static int create_node(node_t **return_new_node, const elem_t entry, const node_t *next)
{
    *return_new_node = malloc(sizeof(node_t));
    if (*return_new_node == NULL)
    {
        return MEMORY_ALLOCATION_FAILURE;
    }

    **return_new_node = (node_t){.entry = entry, .next = (node_t *)next};
    return SUCCESS;
}

/**
 * @brief Return a pointer to the pointer to the node before a given index.
 * @param head A pointer to a pointer to the head of a linked list.
 * @param index The index.
 * @return Returns a pointer to the pointer to the node before a given index.
 */
static node_t **get_ptr_before_index(node_t **head, const int index)
{
    // by calling get_ptr_to_index with (index - 1) we will (given that function works) get the index before the one we
    // want
    return get_ptr_to_index(head, index - 1);
}

/**
 * @brief Return a pointer to the pointer to the node of a given index.
 * @param head A pointer to a pointer to the head of a linked list.
 * @param index The index.
 * @return Returns a pointer to the pointer to the node of a given index.
 */
static node_t **get_ptr_to_index(node_t **head, const int index)
{
    // index of 0 is start of list there by ptr to index is equal to head
    int index_tracker = 0;
    while (*head != NULL && index_tracker != index)
    {
        head = &((*head)->next); // Go to next node
        index_tracker += 1;
    }
    return head;
}

/////////////////////////////////////////////////
///////////       ITERATOR       ////////////////
/////////////////////////////////////////////////

typedef struct list ioopm_list_t;
typedef struct iter iter_t;

typedef struct iter ioopm_list_iterator_t;
struct iter
{
    node_t *current;
    node_t *next;
    ioopm_list_t *list;
    size_t index;
};

static bool is_valid(const ioopm_list_iterator_t *iter);
static void initialize_iterator(ioopm_list_iterator_t **iter, ioopm_list_t *list);

int ioopm_list_iterator(ioopm_list_iterator_t **return_iter, ioopm_list_t *list)
{
    if (return_iter == NULL)
    {
        return INVALID_RETURN_POINTER;
    }
    if (list == NULL)
    {
        return INVALID_LIST_POINTER;
    }

    // Create and initialize iterator
    *return_iter = malloc(sizeof(ioopm_list_iterator_t));
    if (*return_iter == NULL)
    {
        return MEMORY_ALLOCATION_FAILURE;
    }

    initialize_iterator(return_iter, list);

    return SUCCESS;
}

int ioopm_iterator_has_next(bool *return_has_next, const ioopm_list_iterator_t *iter)
{
    if (iter == NULL)
    {
        return INVALID_ITERATOR_POINTER;
    }
    if (return_has_next == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    *return_has_next = iter->next != NULL;

    return SUCCESS;
}

int ioopm_iterator_next(elem_t *return_next, ioopm_list_iterator_t *iter)
{
    if (iter == NULL)
    {
        return INVALID_ITERATOR_POINTER;
    }
    if (return_next == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    bool has_next = false;
    if (ioopm_iterator_has_next(&has_next, iter) == SUCCESS && has_next)
    {
        iter->current = iter->next;
        iter->next = iter->next->next;

        // increment current index
        iter->index++;

        // return new current element
        *return_next = iter->current->entry;
    }
    else
    {
        // Error handeling when calling next on a iterator that is empty
        return ITERATOR_IS_EMPTY;
    }
    return SUCCESS;
}

int ioopm_iterator_remove(elem_t *return_removed, ioopm_list_iterator_t *iter)
{
    if (iter == NULL)
    {
        return INVALID_ITERATOR_POINTER;
    }
    if (return_removed == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    // Remove previous element recived by ioopm_iterator_next / ioopm_iterator_current
    // dose not update current iterator and therfore any opperations on iterator that is not has_next or next is
    // invalid.
    if (is_valid(iter))
    {
        // Remove the current link
        ioopm_linked_list_remove(return_removed, iter->list, iter->index);

        // set next to be pointer after this one,
        // set current to be null after removal to mark it as invalid
        iter->current = NULL;

        return SUCCESS;
    }
    else
    {
        // TODO: Error Can not remove NULL ptr
        //    User has not called ioopm_iterator_next after using ioopm_list_iterator (Creation)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_reset (Reset)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_remove (Removal of element)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_insert (Insertion of element)
        return INVALID_ITERATOR_STATE;
    }
}

int ioopm_iterator_insert(ioopm_list_iterator_t *iter, elem_t element)
{
    if (iter == NULL)
    {
        return INVALID_ITERATOR_POINTER;
    }

    if (is_valid(iter))
    {
        // insert item on our index and step forward to the current
        int status = ioopm_linked_list_insert(iter->list, iter->index, element);
        if (status == SUCCESS)
        {
            iter->current = NULL;
            return SUCCESS;
        }
        else
        {
            // insert failed, propegate errors.
            // Possible errors not caught by our guards:
            // MEMORY_ALLOCATION_FAILURE
            return status;
        }
    }
    else
    {
        // TODO: Error Can not insert item before a NULL ptr
        //    User has not called ioopm_iterator_next after using ioopm_list_iterator (Creation)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_reset (Reset)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_remove (Removal of element)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_insert (Insertion of element)
        return INVALID_ITERATOR_STATE;
    }
}

int ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    if (iter == NULL)
    {
        return INVALID_ITERATOR_POINTER;
    }

    iter->current = NULL;
    iter->next = iter->list->first;
    iter->index = -1;

    return SUCCESS;
}

int ioopm_iterator_current(elem_t *return_current, const ioopm_list_iterator_t *iter)
{
    if (iter == NULL)
    {
        return INVALID_ITERATOR_POINTER;
    }
    if (return_current == NULL)
    {
        return INVALID_RETURN_POINTER;
    }

    if (is_valid(iter))
    {
        // Return the current item of the iterator
        *return_current = iter->current->entry;
        return SUCCESS;
    }
    else
    {
        // TODO: Error Can not retrive a elem_t from a NULL ptr
        //    User has not called ioopm_iterator_next after using ioopm_list_iterator (Creation)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_reset (Reset)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_remove (Removal of element)
        // OR User has not called ioopm_iterator_next after using ioopm_iterator_insert (Insertion of element)
        return INVALID_ITERATOR_STATE;
    }
}

int ioopm_iterator_destroy(ioopm_list_iterator_t **iter)
{
    if (iter == NULL || *iter == NULL)
    {
        return INVALID_ITERATOR_POINTER;
    }

    free(*iter);
    *iter = NULL;

    return SUCCESS;
}

/// @brief Check if the iterator is in a valid state for retrival or changes.
/// @param iter The iterator to check if it has a elem_t in current.
/// @return true if the current node_t is not a NULL pointer
static bool is_valid(const ioopm_list_iterator_t *iter)
{
    return iter->current != NULL;
}

/**
 * @brief Initializes the fields in an iterator.
 * @param list The iterator to be initialized.
 * @param list The list that the iterator is based upon.
 */
static void initialize_iterator(ioopm_list_iterator_t **iter, ioopm_list_t *list)
{
    (*iter)->current = NULL;
    (*iter)->next = list->first;
    (*iter)->list = list;
    (*iter)->index = -1;
}