#ifndef SRC_GREP_MATCHES_H_
#define SRC_GREP_MATCHES_H_

#include <ctype.h>
#include <regex.h>
#include <stdio.h>

#include "../common/utils.h"

// #define malloc(x) NULL
// #define calloc(x, y) NULL
// #define realloc(x, y) NULL

typedef struct match {
  char *line;
  int line_num;
} match_t;

typedef struct matches {
  int cap;
  int len;
  match_t **data;
} matches_t;

match_t *new_match(char *str, int line);

matches_t *init_matches(int capacity);
void matches_push(matches_t *matches, match_t *new_elem);
void matches_sort(matches_t *matches);
void delete_matches(matches_t *matches);

#endif // SRC_GREP_MATCHES_H_
