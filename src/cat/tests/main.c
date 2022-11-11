#include "../../common/tests.h"

int main(void) {
  bool_t result = TRUE;
  printf("running tests...\n");

  const bool_t tests_results[20] = {
      test(S21_CAT, "no_flags_one_file", "tests/test.file"),
      test(S21_CAT, "no_flags_two_files", "tests/test.file tests/tabs.test"),
      test(S21_CAT, "flag_b_one_file", "-b tests/test.file"),
      test(S21_CAT, "flag_b_two_files", "-b tests/test.file tests/tabs.test"),
      test(S21_CAT, "flag_e_one_file", "-e tests/test.file"),
      test(S21_CAT, "flag_e_two_files", "-e tests/test.file tests/tabs.test"),
      test(S21_CAT, "flag_n_one_file", "-n tests/test.file"),
      test(S21_CAT, "flag_n_two_files", "-n tests/test.file tests/tabs.test"),
      test(S21_CAT, "flag_s_one_file", "-s tests/test.file"),
      test(S21_CAT, "flag_s_two_files", "-s tests/test.file tests/tabs.test"),
      test(S21_CAT, "flag_t_one_file", "-t tests/test.file"),
      test(S21_CAT, "flag_t_two_files", "-t tests/test.file tests/tabs.test"),
      test(S21_CAT, "all_flags_one_file", "-benst tests/test.file"),
      test(S21_CAT, "all_flags_two_files",
           "-benst tests/test.file tests/tabs.test"),
  };

  for (int i = 0; i < 10 && result == TRUE; i++)
    result = tests_results[i];

  return result;
}
