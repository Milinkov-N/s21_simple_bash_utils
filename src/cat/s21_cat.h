#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/utils.h"
#include "../common/vec.h"

// #define malloc(x) NULL
// #define calloc(x, y) NULL
// #define realloc(x, y) NULL

typedef struct enabled_options {
  bool_t num_nonblank;
  bool_t mark_eol;
  bool_t num_all;
  bool_t squeeze_blank;
  bool_t mark_tabs;
  bool_t mark_nonprintable;
} options_t;

options_t *get_options(int argc, char **argv);

bool_t parse_long_flags(options_t *enopt, char *arg);
bool_t parse_short_flags(options_t *enopt, char *arg);
void apply_options(options_t *enopt, char *str);
char *number_lines(char *input, void(cb)(char *, char *, int *, char, int *));

void number_nonblank(char *output, char *buf, int *buf_len, char ch, int *line);
void number_all(char *output, char *buf, int *buf_len, char ch, int *line);
char *squeeze_blank(char *input);
char *mark_nonprintable(char *input);
char *mark_eol(char *input);
char *mark_tabs(char *input);

#endif  // SRC_CAT_S21_CAT_H_
