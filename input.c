#include <string.h>

int string_in(char *my_str, char *string_list[], size_t num_strings)
{
  for ( int i = 0; i < num_strings; i++ ) if (strcmp(my_str, string_list[i]) == 0 ) return i;
  return 0;
}

void hargs(int argc, char *argv[]) {
  int tmp = string_in("-p", argv, argc);
  if (tmp > 0) {
    char *p;
    int num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      PORT = num;
    }
  }

  tmp = string_in("-s", argv, argc);
  if (tmp > 0) {
    char *p;
    int num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      S_TIME = num;
    }
  }

  tmp = string_in("-c", argv, argc);
  if (tmp > 0) {
    char *p;
    int num;
    errno = 0;
    long conv = strtol(argv[tmp+1], &p, 10);
    if (!(errno != 0 || *p != '\0' || conv > INT_MAX)) {
      num = conv;
      COUNT = num;
    }
  }

  tmp = string_in("-h", argv, argc);
  if (tmp > 0) {
    printf("where you are, there is no help. i'm sorry.\n");
    exit(0);
  }
}
