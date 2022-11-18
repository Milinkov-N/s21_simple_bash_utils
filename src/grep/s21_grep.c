#include "s21_grep.h"

grep_t *init_grep(int argc, char **argv, options_t *options) {
  grep_t *grep = {0};

  if ((grep = malloc(sizeof(grep_t))) != NULL && options != NULL) {
    strvec_t *patterns = init_strvec(2);
    strvec_t *filenames = init_strvec(4);

    if (patterns != NULL && filenames != NULL) {
      grep->patterns = patterns;
      grep->filenames = filenames;
      grep->options = options;

      if (parse_regfiles(argv, options, patterns)) {
        parse_patterns(argc, argv, options, patterns);
        parse_filenames(argc, argv, filenames, patterns);
      } else {
        delete_grep(grep);
        grep = NULL;
      }
    } else {
      free(grep);
      grep = NULL;
    }
  }

  return grep;
}

bool_t parse_regfiles(char **argv, options_t *options, strvec_t *patterns) {
  bool_t result = TRUE;

  if (options->regfiles_indexes != NULL) {
    for (int i = 0; i < options->regfiles_indexes->len; i++) {
      if (!result) break;
      result = read_regfile(argv[options->regfiles_indexes->data[i]], options,
                            patterns);
    }
  }

  return result;
}

bool_t read_regfile(char *filename, options_t *options, strvec_t *patterns) {
  bool_t result = TRUE;
  char *file_contents = read_file(filename);

  if (file_contents != NULL) {
    char buf[1024] = {0};
    int len = 0;

    for (int i = 0; i < (int)strlen(file_contents); i++) {
      if (file_contents[i] == '\n' && len == 0) {
        options->_dump_all = TRUE;
        i = (int)strlen(file_contents);
      } else if (file_contents[i] == '\n' && len > 0) {
        strvec_push(patterns, strdup(buf));
        for (int j = 0; j < len; j++) buf[j] = 0;
        len = 0;
      } else {
        buf[len++] = file_contents[i];
      }
    }

    if (len != 0 && !options->_dump_all) strvec_push(patterns, strdup(buf));

    free(file_contents);
  } else if (!options->suppress_errors) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
    result = FALSE;
  }

  return result;
}

void parse_patterns(int argc, char **argv, options_t *options,
                    strvec_t *patterns) {
  for (int i = 0; i < argc; i++) {
    int len = (int)strlen(argv[i]);

    if (starts_with("-", argv[i]) && len > 2)
      for (int j = 0; j < len; j++)
        if (argv[i][j] == 'e' && j + 1 < len)
          strvec_push(patterns, strdup(&argv[i][j + 1]));
  }

  if (options->pattern_indexes != NULL)
    for (int i = 0; i < options->pattern_indexes->len; i++)
      strvec_push(patterns, strdup(argv[options->pattern_indexes->data[i]]));
}

void parse_filenames(int argc, char **argv, strvec_t *filenames,
                     strvec_t *patterns) {
  for (int i = 1; i < argc; i++) {
    if (!starts_with("-", argv[i]) && !patterns->len)
      strvec_push(patterns, strdup(argv[i]));
    else if (!starts_with("-", argv[i]) &&
             (!(starts_with("-", argv[i - 1]) &&
                ends_with("f", argv[i - 1]))) &&
             (!(starts_with("-", argv[i - 1]) &&
                ends_with("e", argv[i - 1]))) &&
             patterns->len)
      strvec_push(filenames, argv[i]);
  }
}

void run_grep(grep_t *grep) {
  if (grep != NULL)
    for (int i = 0; i < grep->filenames->len; i++)
      process_file(grep->filenames->data[i], grep);
}

void process_file(char *filename, grep_t *grep) {
  char *file_contents = read_file(filename);

  if (grep->options->_dump_all && file_contents != NULL) {
    printf("%s", file_contents);
    free(file_contents);
    file_contents = NULL;
  }

  if (file_contents != NULL) {
    matches_t *matches = init_matches(48);
    regex_t regex = {0};
    regmatch_t pmatch[2] = {0};
    char buf[1024] = {0};
    int len = 0;
    int line = 1;

    for (int i = 0; i < (int)strlen(file_contents); i++) {
      if (file_contents[i] == '\n') {
        process_patterns(grep, matches, &regex, pmatch, buf, line);
        for (int j = 0; j < len; j++) buf[j] = 0;
        len = 0;
        line++;
      } else {
        buf[len++] = file_contents[i];
      }
    }

    if (len != 0) process_patterns(grep, matches, &regex, pmatch, buf, line);

    matches_sort(matches);
    print_matches(filename, grep, matches);

    delete_matches(matches);
    free(file_contents);
  } else if (!grep->options->suppress_errors && !grep->options->_dump_all) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", filename);
  }
}

void process_patterns(grep_t *grep, matches_t *matches, regex_t *regex,
                      regmatch_t *pmatch, char *line, int line_num) {
  bool_t is_match = FALSE;

  for (int j = 0; j < grep->patterns->len; j++) {
    char *str = grep->options->ignore_case == TRUE ? tolower_str(line) : line;
    char *pat = grep->options->ignore_case == TRUE
                    ? tolower_str(grep->patterns->data[j])
                    : strdup(grep->patterns->data[j]);

    if (regcomp(regex, pat, 0)) {
      fprintf(stderr, "Could not compile pattern: %s\n", pat);
      free(pat);
      continue;
    }

    int match_res = regexec(regex, str, 2, pmatch, 0);

    if (grep->options->matched_parts && !grep->options->invert_match &&
        !match_res)
      push_regmatch(pmatch, regex, matches, line, line_num);
    if (!match_res)
      is_match = TRUE;
    else if (match_res == REG_NOMATCH && (!is_match))
      is_match = FALSE;

    if (grep->options->ignore_case) free(str);
    free(pat);
    regfree(regex);
  }

  if (!grep->options->invert_match && is_match)
    matches_push(matches, new_match(line, line_num));
  else if (grep->options->invert_match && (!is_match) == REG_NOMATCH)
    matches_push(matches, new_match(line, line_num));
}

void push_regmatch(regmatch_t *pmatch, regex_t *regex, matches_t *matches,
                   char *str, int line) {
  char *start_pos = str;
  int match_res = 0;
  int matches_count = 0;

  char matched_part[1024] = {0};

  while (!match_res && pmatch[0].rm_eo != pmatch[0].rm_so) {
    int matched_part_len = (int)(pmatch[0].rm_eo - pmatch[0].rm_so);
    if (matches_count == 0)
      sprintf(&matched_part[strlen(matched_part)], "%.*s", matched_part_len,
              start_pos + pmatch[0].rm_so);
    else
      sprintf(&matched_part[strlen(matched_part)], "\n%.*s", matched_part_len,
              start_pos + pmatch[0].rm_so);

    start_pos += pmatch[0].rm_eo;
    match_res = regexec(regex, start_pos, 2, pmatch, REG_NOTBOL);

    matches_count++;
  }

  matches_push(matches, new_match(matched_part, line));
}

void print_matches(char *filename, grep_t *grep, matches_t *matches) {
  if (matches != NULL) {
    if (grep->options->count_matched_lines && grep->options->matched_files)
      print_cl_combo(grep, filename, grep->filenames->len, matches->len);
    else if (grep->options->count_matched_lines)
      print_count_matched_lines(grep, filename, matches->len);
    else if (grep->options->matched_files && matches->len > 0)
      printf("%s\n", filename);
    else
      for (int m = 0; m < matches->len; m++)
        print_match(grep, matches->data[m], filename);
  }
}

void print_cl_combo(grep_t *grep, char *filename, int filenames_len,
                    int matches_len) {
  if (grep->options->dont_precede_filenames || filenames_len == 1) {
    if (matches_len > 0)
      printf("1\n");
    else
      printf("0\n");
  } else if (filenames_len > 1) {
    if (matches_len > 0)
      printf("%s:1\n", filename);
    else
      printf("%s:0\n", filename);
  }

  if (matches_len > 0) printf("%s\n", filename);
}

void print_count_matched_lines(grep_t *grep, char *filename, int count) {
  if (grep->options->dont_precede_filenames || grep->filenames->len == 1)
    printf("%d\n", count);
  else if (grep->filenames->len > 1)
    printf("%s:%d\n", filename, count);
}

void print_match(grep_t *grep, match_t *match, char *filename) {
  char *line = precede_linenum(grep->options, match->line, match->line_num);

  if (grep->filenames->len == 1) {
    printf("%s\n", line);
  } else if (grep->filenames->len > 1) {
    if (grep->options->dont_precede_filenames)
      printf("%s\n", line);
    else
      printf("%s:%s\n", filename, line);
  }

  free(line);
}

char *precede_linenum(options_t *options, char *line, int line_num) {
  char *new_line = malloc((strlen(line) + 32) * sizeof(char));

  if (new_line != NULL) {
    if (options->precede_linenum)
      sprintf(new_line, "%d:%s", line_num, line);
    else
      sprintf(new_line, "%s", line);
  }

  return new_line;
}

void delete_grep(grep_t *grep) {
  if (grep != NULL) {
    delete_strvec(grep->patterns, STRVEC_FULL_DELETE);
    delete_strvec(grep->filenames, 0);
    delete_options(grep->options);
    free(grep);
  }
}

options_t *get_options(int argc, char **argv) {
  options_t *options = {0};

  if ((options = calloc(1, sizeof(options_t))) != NULL) {
    intvec_t *pattern_indexes = init_intvec(4);
    intvec_t *regfiles_indexes = init_intvec(4);

    if (pattern_indexes != NULL && regfiles_indexes != NULL) {
      options->pattern_indexes = pattern_indexes;
      options->regfiles_indexes = regfiles_indexes;

      for (int i = 1; i < argc; i++)
        if (starts_with("-", argv[i])) parse_flags(options, argc, argv[i], i);
    } else {
      free(options);
      options = NULL;
    }
  }

  return options;
}

void delete_options(options_t *options) {
  if (options != NULL) {
    if (options->pattern_indexes != NULL)
      delete_intvec(options->pattern_indexes);
    if (options->regfiles_indexes != NULL)
      delete_intvec(options->regfiles_indexes);
    free(options);
  }
}

void parse_flags(options_t *options, int argc, char *arg, int argi) {
  for (int j = 0; j < (int)strlen(arg); j++) {
    switch (arg[j]) {
      case 'e':
        if (j + 1 != (int)strlen(arg))
          j = (int)strlen(arg);
        else if (options->pattern_indexes != NULL && argi < argc - 1)
          intvec_push(options->pattern_indexes, argi + 1);
        break;
      case 'f':
        if (options->regfiles_indexes != NULL && argi < argc - 1)
          intvec_push(options->regfiles_indexes, argi + 1);
        break;
      case 'i':
        options->ignore_case = TRUE;
        break;
      case 'v':
        options->invert_match = TRUE;
        break;
      case 'c':
        options->count_matched_lines = TRUE;
        break;
      case 'l':
        options->matched_files = TRUE;
        break;
      case 'n':
        options->precede_linenum = TRUE;
        break;
      case 'h':
        options->dont_precede_filenames = TRUE;
        break;
      case 's':
        options->suppress_errors = TRUE;
        break;
      case 'o':
        options->matched_parts = TRUE;
        break;
    }
  }
}
