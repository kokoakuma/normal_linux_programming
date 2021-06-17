#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int i;
  if (argc < 2)
  {
    fprintf(stderr, "%s: no arguments\n", argv[0]);
    exit(1);
  }
  for (i = 1; i < argc; i++)
  {
    // unlink success = 0, fail = -1;
    if (unlink(argv[i]) < 0)
    {
      perror(argv[0]);
      exit(1);
    }
  }
  exit(0);
}