#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <string.h>

#include "../common/args.h"
#include "../common/utils.h"
#include "../common/vec.h"
#include "matches.h"

// #define malloc(x) NULL
// #define calloc(x, y) NULL
// #define realloc(x, y) NULL

typedef struct enabled_options {
  intvec_t *pattern_indexes;
  intvec_t *regfiles_indexes;
  bool_t ignore_case;
  bool_t invert_match;
  bool_t count_matched_lines;
  bool_t matched_files;
  bool_t precede_linenum;
  bool_t dont_precede_filenames;
  bool_t suppress_errors;
  bool_t matched_parts;
  bool_t _dump_all;
} options_t;

typedef struct grep {
  strvec_t *patterns;
  strvec_t *filenames;
  options_t *options;
} grep_t;

grep_t *init_grep(int argc, char **argv, options_t *options);
bool_t parse_regfiles(char **argv, options_t *options, strvec_t *patterns);
bool_t read_regfile(char *filename, options_t *options, strvec_t *patterns);
void parse_patterns(int argc, char **argv, options_t *options,
                    strvec_t *patterns);
void parse_filenames(int argc, char **argv, strvec_t *filenames,
                     strvec_t *patterns);

void run_grep(grep_t *grep);

void process_file(char *filename, grep_t *grep);
void process_patterns(grep_t *grep, matches_t *matches, regex_t *regex,
                      regmatch_t *pmatch, char *line, int line_num);
void push_regmatch(regmatch_t *pmatch, regex_t *reg, matches_t *matches,
                   char *str, int line);

void print_matches(char *filename, grep_t *grep, matches_t *matches);
void print_cl_combo(grep_t *grep, char *filename, int filenames_len,
                    int matches_len);
void print_count_matched_lines(grep_t *grep, char *filename, int count);
void print_match(grep_t *grep, match_t *match, char *filename);
char *precede_linenum(options_t *options, char *line, int line_num);

void delete_grep(grep_t *grep);

options_t *get_options(int argc, char **argv);
void delete_options(options_t *options);

void parse_flags(options_t *options, int argc, char *arg, int argi);

#endif // SRC_GREP_S21_GREP_H_
