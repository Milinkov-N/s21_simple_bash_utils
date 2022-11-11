#include "matches.h"

match_t *new_match(char *str, int line) {
  char *new_str = strdup(str);
  match_t *match = malloc(sizeof(match_t));

  if (match != NULL && new_str != NULL) {
    match->line = new_str;
    match->line_num = line;
  }

  return match;
}

matches_t *init_matches(int capacity) {
  matches_t *matches = malloc(sizeof(matches_t));

  if (matches != NULL) {
    match_t **temp = {0};
    matches->cap = capacity;
    matches->len = 0;
    if ((temp = calloc(capacity, sizeof(match_t *))) != NULL)
      matches->data = temp;
  }

  return matches;
}

void matches_push(matches_t *matches, match_t *new_elem) {
  if (matches != NULL) {
    if (matches->data != NULL) {
      bool_t is_dup = FALSE;

      if (matches->len + 1 == matches->cap)
        matches->data = safe_realloc(matches->data,
                                     (matches->cap *= 2) * sizeof(match_t *));

      for (int i = 0; i < matches->len; i++) {
        if (matches->data[i]->line_num == new_elem->line_num) {
          free(new_elem->line);
          free(new_elem);
          is_dup = TRUE;
          break;
        }
      }

      if (!is_dup) matches->data[matches->len++] = new_elem;
    }
  }
}

void matches_sort(matches_t *matches) {
  for (int i = 0; i < matches->len && matches != NULL && matches->data != NULL;
       i++) {
    for (int j = i + 1; j < matches->len; j++) {
      if (matches->data[j]->line_num < matches->data[i]->line_num) {
        match_t *temp = matches->data[i];
        matches->data[i] = matches->data[j];
        matches->data[j] = temp;
      }
    }
  }
}

void delete_matches(matches_t *matches) {
  if (matches != NULL) {
    if (matches->data != NULL) {
      for (int i = 0; i < matches->len; i++) {
        if (matches->data[i] != NULL) {
          if (matches->data[i]->line != NULL) free(matches->data[i]->line);
          free(matches->data[i]);
        }
      }
      free(matches->data);
    }
    free(matches);
  }
}
