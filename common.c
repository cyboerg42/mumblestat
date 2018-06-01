#include <sys/time.h>
#include <time.h>
#include <string.h>

uint64_t get_posix_clock_time ()
{
  struct timespec ts;
  if (clock_gettime (CLOCK_MONOTONIC, &ts) == 0) return (uint64_t) (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
  else return 0;
}

void die(char *s)
{
  perror(s);
  exit(1);
}
