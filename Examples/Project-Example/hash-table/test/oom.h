#pragma once
#include <stdbool.h>
#include <stddef.h>

#define malloc(X) my_malloc(X)
#define calloc(X, Y) my_calloc(X, Y)
#define free(X) my_free(X)

void *my_malloc(size_t size);
void *my_calloc(size_t count, size_t size);
void my_free(void *ptr);

/// @brief Makes next call to malloc return NULL
/// @param toggle if calls after faliure should should return NULL as well
void oom_next_malloc(bool toggle);

/// @brief Makes n:th call to malloc return NULL
/// @param num how many calls to malloc before it returns NULL
/// @param toggle if calls after faliure should should return NULL as well
void oom_nth_malloc_call(size_t num, bool toggle);

/// @brief Sets malloc to behave normaly
void set_normal_malloc(void);

/// @brief Makes next call to calloc return NULL
/// @param toggle if calls after faliure should should return NULL as well
void oom_next_calloc(bool toggle);

/// @brief Makes n:th call to calloc return NULL
/// @param num how many calls to calloc before it returns NULL
/// @param toggle if calls after faliure should should return NULL as well
void oom_nth_calloc_call(size_t num, bool toggle);

/// @brief Sets calloc to behave normaly
void set_normal_calloc(void);
