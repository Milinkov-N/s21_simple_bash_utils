#include "s21_cat.h"

options_t *get_options(int argc, char **argv) {
  options_t *options = {0};

  if ((options = malloc(sizeof(options_t))) != NULL) {
    bool_t result_lflags = TRUE;
    bool_t result_sflags = TRUE;

    for (int i = 1; i < argc && argv; i++) {
      if (starts_with("--", argv[i]))
        result_lflags = parse_long_flags(options, argv[i]);
      else if (starts_with("-", argv[i]))
        result_sflags = parse_short_flags(options, argv[i]);
    }

    if (!result_lflags || !result_sflags) {
      free(options);
      options = NULL;
    }
  }

  return options;
}

bool_t parse_long_flags(options_t *options, char *arg) {
  bool_t result = TRUE;

  if (strcmp(arg, "--number-nonblank") == 0)
    options->num_nonblank = TRUE;
  else if (strcmp(arg, "--number") == 0)
    options->num_all = TRUE;
  else if (strcmp(arg, "--squeeze-blank") == 0)
    options->squeeze_blank = TRUE;
  else
    result = FALSE;

  if (!result) {
    printf(
        "s21_cat: illegal option -- %s\n"
        "usage: s21_cat [-benst] [--number-nonblank --number "
        "--squeeze-blank] [file ...]\n",
        arg);
  }

  return result;
}

bool_t parse_short_flags(options_t *options, char *arg) {
  bool_t result = TRUE;

  for (int i = 1; i < (int)strlen(arg); i++) {
    switch (arg[i]) {
      case 'b':
        options->num_nonblank = TRUE;
        break;
      case 'e':
        options->mark_eol = TRUE;
        break;
      case 'n':
        options->num_all = TRUE;
        break;
      case 's':
        options->squeeze_blank = TRUE;
        break;
      case 't':
        options->mark_tabs = TRUE;
        break;
      case 'v':
        options->mark_nonprintable = TRUE;
        break;
      default:
        fprintf(stderr,
                "s21_cat: illegal option -- %c\n"
                "usage: s21_cat [-benst] [--number-nonblank] [--number] "
                "[--squeeze-blank] [file ...]\n",
                arg[i]);
        result = FALSE;
        break;
    }
  }

  return result;
}

void apply_options(options_t *options, char *str) {
  char *result = strdup(str);

  if (result != NULL) {
    if (options->squeeze_blank) result = squeeze_blank(result);
    if (options->mark_nonprintable) result = mark_nonprintable(result);

    if (options->mark_tabs) {
      result = mark_nonprintable(result);
      result = mark_tabs(result);
    }

    if (options->num_nonblank)
      result = number_lines(result, number_nonblank);
    else if (options->num_all)
      result = number_lines(result, number_all);

    if (options->mark_eol) {
      result = mark_nonprintable(result);
      result = mark_eol(result);
    }

    printf("%s", result);
    free(result);
  }
}

char *number_lines(char *input, void(cb)(char *, char *, int *, char, int *)) {
  char *output = {0};
  char *buf = {0};

  if (input != NULL) {
    output = calloc(strlen(input) * 2, sizeof(char));
    buf = calloc(strlen(input), sizeof(char));
  }

  if (output != NULL && buf != NULL) {
    int line = 1;
    int buf_len = 0;

    for (int i = 0; i <= (int)strlen(input); i++)
      cb(&output[strlen(output)], buf, &buf_len, input[i], &line);

    if (buf_len > 1) sprintf(&output[strlen(output)], "%6d\t%s", line++, buf);

    free(buf);
  }

  free(input);
  return output;
}

void number_nonblank(char *output, char *buf, int *buf_len, char ch,
                     int *line) {
  if (ch == '\n' && *buf_len == 0) {
    sprintf(output, "\n");
  } else if (ch == '\n' && *buf_len != 0) {
    sprintf(output, "%6d\t%s\n", (*line)++, buf);
    for (int j = 0; j < *buf_len; j++) buf[j] = 0;
    *buf_len = 0;
  } else {
    buf[(*buf_len)++] = ch;
  }
}

void number_all(char *output, char *buf, int *buf_len, char ch, int *line) {
  if (ch == '\n') {
    sprintf(output, "%6d\t%s\n", (*line)++, buf);
    for (int j = 0; j < *buf_len; j++) buf[j] = 0;
    *buf_len = 0;
  } else {
    buf[(*buf_len)++] = ch;
  }
}

char *squeeze_blank(char *input) {
  char *output = calloc(strlen(input) * 2, sizeof(char));

  if (output != NULL) {
    int eol_count = 0;

    for (int i = 0; i < (int)strlen(input); i++) {
      if (input[i] == '\n') {
        if (eol_count < 2) {
          sprintf(&output[strlen(output)], "%c", input[i]);
          eol_count++;
        }
      } else {
        sprintf(&output[strlen(output)], "%c", input[i]);
        eol_count = 0;
      }
    }
  }

  free(input);
  return output;
}
char *mark_nonprintable(char *input) {
  char *output = input;

  for (int i = 1; i < 32; i++) {
    if (i == 9 || i == 10 || i == 13) continue;

    char replacing[4] = {0};
    char with[4] = {0};

    sprintf(replacing, "%c", i);
    sprintf(with, "^%c", i + 64);

    output = str_replace(output, replacing, with);
  }

  output = str_replace(output, "\x7f", "^?");
  return output;
}

char *mark_eol(char *input) { return str_replace(input, "\n", "$\n"); }

char *mark_tabs(char *input) { return str_replace(input, "\t", "^I"); }
