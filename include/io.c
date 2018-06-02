#include <stdio.h>
#include <string.h>

u_int8_t string_in(char *my_str, char *string_list[], size_t num_strings)
{
  for ( int i = 0; i < num_strings; i++ ) if (strcmp(my_str, string_list[i]) == 0 ) return i;
  return 0;
}

void p_err(char *s)
{
  fprintf(stderr, s);
  exit(1);
}

void p_png(int *response, float ping)
{
  printf("Users : %i/%i - %.2f ms\n", *(response + 0), *(response + 1), ping);
}
