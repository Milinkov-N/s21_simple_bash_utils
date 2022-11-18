#include "utils.h"

char *read_file(const char *filename) {
  FILE *fp = {0};
  int bytes = 256;
  char *file_contents = {0};

  if ((file_contents = calloc(bytes, sizeof(char))) != NULL) {
    if ((fp = fopen(filename, "r")) != NULL) {
      char ch;
      int i = 0;

      while ((ch = getc(fp)) != EOF) {
        if (i + 1 == bytes) {
          char *tmp = {0};
          int new_size = bytes * 2;

          if ((tmp = calloc(new_size, sizeof(char))) != NULL) {
            for (int j = 0; j < i; j++) tmp[j] = file_contents[j];
            free(file_contents);
            file_contents = tmp;
            bytes = new_size;
          }
        }

        if (file_contents != NULL) file_contents[i++] = ch;
      }

      fclose(fp);
    } else {
      free(file_contents);
      file_contents = NULL;
    }
  }

  return file_contents;
}

char *mstrtok(char *str, const char *delim) {
  static char *source = NULL;
  char *ptr = 0, *output = 0;

  if (str != NULL) source = str;

  if (source != NULL && delim != NULL) {
    if ((ptr = strpbrk(source, delim)) != NULL) {
      *ptr = 0;
      output = source;
      source = ++ptr;
    }
  }

  return output;
}

char *str_replace(char *orig, const char *rep, const char *with) {
  char *init_ptr = orig, *result = NULL, *ins, *tmp;

  if (orig != NULL && rep != NULL) {
    int len_rep;
    len_rep = strlen(rep);

    if (len_rep != 0) {
      int len_with, count;
      if (!with) with = "";
      len_with = strlen(with);

      ins = orig;
      for (count = 0; (tmp = strstr(ins, rep)); ++count) ins = tmp + len_rep;
      tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

      if (result != NULL && tmp != NULL) {
        while (count--) {
          ins = strstr(orig, rep);
          int len_front = ins - orig;
          tmp = strncpy(tmp, orig, len_front) + len_front;
          tmp = strcpy(tmp, with) + len_with;
          orig += len_front + len_rep;
        }

        strcpy(tmp, orig);
        free(init_ptr);
      }
    }
  }

  return result;
}

int starts_with(const char *pre, const char *str) {
  size_t lenpre = strlen(pre);
  size_t lenstr = strlen(str);

  return lenstr < lenpre ? 0 : memcmp(pre, str, lenpre) == 0;
}

int ends_with(const char *pre, const char *str) {
  size_t lenpre = strlen(pre);
  size_t lenstr = strlen(str);

  return lenstr < lenpre ? 0 : memcmp(pre, str + lenstr - lenpre, lenpre) == 0;
}

char *tolower_str(char *str) {
  char *lowercase_str = strdup(str);
  char *iter_ptr = lowercase_str;

  for (; *iter_ptr; ++iter_ptr) *iter_ptr = tolower(*iter_ptr);

  return lowercase_str;
}

void *safe_realloc(void *ptr, size_t new_size) {
  (void)new_size;
  if (ptr != NULL) {
    void *tmp = realloc(ptr, new_size);

    if (tmp != NULL) {
      ptr = tmp;
    } else {
      free(ptr);
      ptr = NULL;
    }
  }

  return ptr;
}
