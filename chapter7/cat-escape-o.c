#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void do_cat(FILE *f, int escape);

int main(int argc, char *argv[])
{
  int opt;
  int escape = 0;
  int i;

  while ((opt = getopt(argc, argv, "e")) != -1)
  {
    switch(opt)
    {
      case 'e':
        escape = 1;
		    break;
      case '?':
        fprintf(stderr, "Usage: %s [-e] [file...]\n", argv[0]);
        exit(1); // 異常終了
    }
  }
  argc -= optind;
  argv += optind;
  if (argc == 0)
  {
    do_cat(stdin, escape);
  }
  else
  {
    for (i = 0;i < argc; i++)
    {
      FILE *f;
      f = fopen(argv[i], "r");
      if (!f)
      {
        perror(argv[i]);
        exit(1);
      }
      do_cat(f, escape);
      fclose(f);
    }
  }
  exit(0);
}

static void
do_cat(FILE *f, int escape)
{
  int c;
  if (escape)
  {
    while ((c = fgetc(f)) != EOF)
    {
      switch(c)
      {
        case '\t':
          // error -> return EOF
          if (fputs("\\t", stdout) == EOF) exit(1);
          break;
        case '\n':
          // error -> return EOF
          if (fputs("$\n", stdout) == EOF) exit(1);
          break;
        default:
          if (putchar(c) < 0) exit(1);
          break;
      }
    }
  }
  else
  {
    while ((c = fgetc(f)) != EOF)
    {
      // stream 終了時に-1を返す
      if (putchar(c) < 0) exit(1);
    }
  }
}