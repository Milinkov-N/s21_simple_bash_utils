#include "args.h"

strvec_t *args_filenames(int argc, char **argv, bool_t(cond_cb)(const char *)) {
  strvec_t *filenames = init_strvec(4);

  for (int i = 1; i < argc && filenames != NULL; i++)
    if (cond_cb(argv[i])) strvec_push(filenames, argv[i]);

  return filenames;
}
