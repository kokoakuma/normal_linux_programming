#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <unistd.h>

static void grep_file(regex_t *re, char *path);
static void grep_stream(regex_t *re, FILE *f);

static int opt_invert = 0;
static int opt_ignorecase = 0;
static int opt_count = 0;


int main(int argc, char *argv[])
{
  int opt;
  while ((opt = getopt(argc, argv, "ivc")) != -1)
  {
    switch (opt)
    {
      case 'i':
        opt_ignorecase = 1;
        break;
      case 'v':
        opt_invert = 1;
        break;
      // 追加
      case 'c':
        opt_count = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-iv] [file...]\n", argv[0]);
        exit(1);
    }
  }
  argc -= optind;
  argv += optind;

  if (argc < 1)
  {
    fputs("you did not input pattern you want to find. Please add pattern.", stderr);
    exit(1);
  }

  char *pattern = argv[0];
  argc--;
  argv++;

  int re_mode = REG_EXTENDED | REG_NOSUB | REG_NEWLINE;
  if (opt_ignorecase) re_mode |= REG_ICASE;

  regex_t re;
  // convert pattern(user input) to regex
  int err = regcomp(&re, pattern, re_mode);
  // has error
  if (err != 0)
  {
    char buf[1024];
    regerror(err, &re, buf, sizeof buf);
    puts(buf);
    exit(1);
  }
  if (argc == 0)
  {
    grep_stream(&re, stdin);
  }
  // no error and has 1 file
  else
  {
    int i;
    for (i = 0; i < argc; i++)
    {
      grep_file(&re, argv[i]);
    }
  }
  regfree(&re);
  exit(0);
}

static void grep_file(regex_t *re, char *path)
{
  FILE *f;

  f = fopen(path, "r");
  if (!f)
  {
    perror(path);
    exit(1);
  }
  grep_stream(re, f);
  fclose(f);
}

static void grep_stream(regex_t *re, FILE *f)
{
  char buf[4096];
  int matched;
  int count = 0;

  while(fgets(buf, sizeof buf, f))
  {
    matched = (regexec(re, buf, 0, NULL, 0) == 0);
    if (opt_invert)
    {
      matched = !matched;
    }
    if (matched)
    {
      fputs(buf, stdout);
      count++;
    }
  }
  if (opt_count)
  {
    fprintf(stdout, "total count: %i \n", count);
  }
}