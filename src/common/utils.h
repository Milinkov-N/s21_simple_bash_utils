#ifndef SRC_COMMON_UTILS_H_
#define SRC_COMMON_UTILS_H_

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vec.h"

// #define malloc(x) NULL
// #define calloc(x, y) NULL
// #define realloc(x, y) NULL

#define TRUE 1
#define FALSE 0

typedef int bool_t;

char *read_file(const char *filename);

char *mstrtok(char *str, const char *delim);
char *str_replace(char *orig, const char *rep, const char *with);
int starts_with(const char *pre, const char *str);
int ends_with(const char *pre, const char *str);
char *tolower_str(char *str);

void *safe_realloc(void *ptr, size_t new_size);
void bubble_sort(int *arr, int len);

#endif // SRC_COMMON_UTILS_H_
