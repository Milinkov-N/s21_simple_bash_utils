#include "vec.h"

strvec_t *init_strvec(int capacity) {
  strvec_t *vec = {0};

  if ((vec = malloc(sizeof(strvec_t))) != NULL) {
    char **temp = {0};
    vec->cap = capacity;
    vec->len = 0;

    if ((temp = calloc(capacity, sizeof(char *))) != NULL) vec->data = temp;
  }

  return vec;
}

void strvec_push(strvec_t *vec, char *new_elem) {
  if (vec != NULL) {
    if (vec->len + 1 == vec->cap && vec->data != NULL)
      vec->data = safe_realloc(vec->data, (vec->cap *= 2) * sizeof(char *));

    if (vec->data != NULL) vec->data[vec->len++] = new_elem;
  }
}

void delete_strvec(strvec_t *vec, int delete_mode) {
  if (vec != NULL) {
    if (vec->data != NULL) {
      for (int i = 0; i < vec->len && delete_mode == STRVEC_FULL_DELETE; i++)
        if (vec->data[i] != NULL) free(vec->data[i]);

      free(vec->data);
    }

    free(vec);
  }
}

intvec_t *init_intvec(int capacity) {
  intvec_t *vec = {0};

  if ((vec = malloc(sizeof(intvec_t))) != NULL) {
    int *temp = {0};
    vec->cap = capacity;
    vec->len = 0;

    if ((temp = calloc(capacity, sizeof(int))) != NULL) vec->data = temp;
  }

  return vec;
}

void intvec_push(intvec_t *vec, int new_elem) {
  if (vec != NULL) {
    if (vec->len + 1 == vec->cap && vec->data != NULL)
      vec->data = safe_realloc(vec->data, (vec->cap *= 2) * sizeof(int));

    if (vec->data != NULL) vec->data[vec->len++] = new_elem;
  }
}

void delete_intvec(intvec_t *vec) {
  if (vec != NULL) {
    if (vec->data != NULL) free(vec->data);
    free(vec);
  }
}
