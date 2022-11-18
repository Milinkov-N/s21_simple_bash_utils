#include "../../common/tests.h"

int main(void) {
  bool_t result = TRUE;
  printf("running tests...\n");

  const bool_t tests_results[40] = {
      test(S21_GREP, "e_flag_separated", "-e main tests/test.file"),
      test(S21_GREP, "ne_flags_separated", "-ne main tests/test.file"),
      test(S21_GREP, "e_flag_separated_two_patterns",
           "-e main -e ^int tests/test.file"),
      test(S21_GREP, "e_flag_glued", "-emain tests/test.file"),
      test(S21_GREP, "ne_flags_glued", "-nemain tests/test.file"),
      test(S21_GREP, "e_flag_glued_two_patterns",
           "-emain -e^int tests/test.file"),
      test(S21_GREP, "no_flags_one_file", "main tests/test.file"),
      test(S21_GREP, "no_flags_two_files",
           "main tests/test.file tests/test2.file"),
      test(S21_GREP, "n_flag_one_file", "main -n tests/test.file"),
      test(S21_GREP, "n_flag_two_files",
           "main -n tests/test.file tests/test2.file"),
      test(S21_GREP, "regex_one_file", "'[0-9];$' tests/test.file"),
      test(S21_GREP, "regex_two_files",
           "'[0-9];$' tests/test.file tests/test2.file"),
      test(S21_GREP, "i_flag_one_file", "-i null tests/test2.file"),
      test(S21_GREP, "v_flag_one_file", "-v NULL tests/test2.file"),
      test(S21_GREP, "c_flag_one_file", "-c NULL tests/test2.file"),
      test(S21_GREP, "c_flag_two_files",
           "-c NULL tests/test.file tests/test2.file"),
      test(S21_GREP, "l_flag_one_file", "-l NULL tests/test2.file"),
      test(S21_GREP, "l_flag_two_files",
           "-l NULL tests/test.file tests/test2.file"),
      test(S21_GREP, "n_flag_one_file", "-n NULL tests/test2.file"),
      test(S21_GREP, "n_flag_two_files",
           "-n NULL tests/test.file tests/test2.file"),
      test(S21_GREP, "h_flag_two_files",
           "-h NULL tests/test.file tests/test2.file"),
      test(S21_GREP, "s_flag_one_unknown_file", "-s NULL unknown.file"),
      test(S21_GREP, "s_flag_unknown_file", "-s NULL unknown.file"),
      test(S21_GREP, "s_flag_unknown_and_known_file",
           "-s NULL unknown.file tests/test.file"),
      test(S21_GREP, "f_flag_one_file", "-f tests/regexes.txt tests/test.file"),
      test(S21_GREP, "o_flag_one_file", "-o tests/regexes.txt tests/test.file"),
      test(S21_GREP, "o_flag_two_files",
           "-o tests/regexes.txt tests/test.file tests/test2.file"),
      test(S21_GREP, "iv_flag_combo_one_file", "-iv grep tests/test2.file"),
      test(S21_GREP, "iv_flag_combo_two_files",
           "-iv grep tests/test.file tests/test2.file"),
      test(S21_GREP, "in_flag_combo_one_file", "-in grep tests/test2.file"),
      test(S21_GREP, "in_flag_combo_two_files",
           "-in grep tests/test.file tests/test2.file"),
      test(S21_GREP, "ic_flag_combo_one_file", "-ic grep tests/test2.file"),
      test(S21_GREP, "ic_flag_combo_two_files",
           "-ic grep tests/test.file tests/test2.file"),
      test(S21_GREP, "hl_flag_combo_one_file", "-hl grep tests/test2.file"),
      test(S21_GREP, "hl_flag_combo_two_files",
           "-hl grep tests/test.file tests/test2.file"),
      test(S21_GREP, "cl_flag_combo_one_file", "-cl grep tests/test2.file"),
      test(S21_GREP, "cl_flag_combo_two_files",
           "-cl grep tests/test.file tests/test2.file"),
  };

  for (int i = 0; i < 40 && result == TRUE; i++)
    result = tests_results[i];

  return result;
}
