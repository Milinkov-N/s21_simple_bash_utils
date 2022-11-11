#ifndef SRC_COMMON_ARGS_H_
#define SRC_COMMON_ARGS_H_

#include "utils.h"
#include "vec.h"

strvec_t *args_filenames(int argc, char **argv, bool_t(cond_cb)(const char *));

#endif  // SRC_COMMON_ARGS_H_
