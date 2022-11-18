#include "s21_grep.h"

int main(int argc, char *argv[]) {
  options_t *options = get_options(argc, argv);
  grep_t *grep = init_grep(argc, argv, options);

  if (grep != NULL) {
    run_grep(grep);
    delete_grep(grep);
  }

  // delete_options(options);
  return 0;
}
