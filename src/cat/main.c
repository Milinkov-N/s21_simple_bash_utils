#include <stdio.h>

#include "../common/args.h"
#include "../common/vec.h"
#include "s21_cat.h"

bool_t cond(const char *curr_arg);

int main(int argc, char *argv[]) {
  strvec_t *filenames = args_filenames(argc, argv, cond);
  options_t *options = get_options(argc, argv);

  if (filenames != NULL) {
    for (int i = 0; i < filenames->len && filenames->data != NULL; i++) {
      char *file_contents = read_file(filenames->data[i]);

      if (file_contents != NULL) {
        if (options != NULL) {
          apply_options(options, file_contents);
        }

        free(file_contents);
      } else {
        fprintf(stderr, "s21_cat: %s: No such file or directory\n",
                filenames->data[i]);
      }
    }

    if (options != NULL) {
      free(options);
    }

    delete_strvec(filenames, 0);
  }

  return 0;
}

bool_t cond(const char *curr_arg) {
  return (!starts_with("-", curr_arg) && !starts_with("--", curr_arg));
}
