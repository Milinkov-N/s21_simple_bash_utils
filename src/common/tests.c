#include "tests.h"

bool_t test(int utility, const char *test_name, const char *args) {
  bool_t result = TRUE;
  char cat_cmd[256] = {0};
  char s21_cat_cmd[256] = {0};
  char *orig = utility == S21_CAT ? "cat" : "grep";
  char *s21 = utility == S21_CAT ? "./s21_cat" : "./s21_grep";
  char *orig_out_file = utility == S21_CAT ? "tests/cat.out" : "tests/grep.out";
  char *s21_out_file =
      utility == S21_CAT ? "tests/s21_cat.out" : "tests/s21_grep.out";

  sprintf(cat_cmd, "%s %s > %s", orig, args, orig_out_file);
  sprintf(s21_cat_cmd, "%s %s > %s", s21, args, s21_out_file);

  system(cat_cmd);
  system(s21_cat_cmd);

  char *output = read_file(orig_out_file);
  char *s21_output = read_file(s21_out_file);

  if (output != NULL && s21_output != NULL) {
    printf("  \x1b[96mrunning\x1b[0m: %-30s", test_name);
    if (!(result = strcmp(output, s21_output)))
      printf("\x1b[92msuccess\x1b[0m\n");
    else
      printf("\x1b[31mfail\x1b[0m\n");
  }

  return result;
}
