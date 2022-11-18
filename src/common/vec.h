#ifndef SRC_COMMON_VEC_H_
#define SRC_COMMON_VEC_H_

#include <stdlib.h>

#include "utils.h"

// #define malloc(x) NULL
// #define calloc(x, y) NULL

#define STRVEC_FULL_DELETE 0xFC

typedef struct str_vector {
  int cap;
  int len;
  char **data;
} strvec_t;

typedef struct int_vector {
  int cap;
  int len;
  int *data;
} intvec_t;

strvec_t *init_strvec(int capacity);
void strvec_push(strvec_t *vec, char *new_elem);
void delete_strvec(strvec_t *vec, int delete_mode);

intvec_t *init_intvec(int capacity);
void intvec_push(intvec_t *vec, int new_elem);
void delete_intvec(intvec_t *vec);

#endif // SRC_COMMON_VEC_H_
