#ifndef SRC_COMMON_TESTS_H_
#define SRC_COMMON_TESTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define S21_CAT 0xCA1
#define S21_GREP 0xCE2

bool_t test(int utility, const char *test_name, const char *args);

#endif  // SRC_COMMON_TESTS_H_
