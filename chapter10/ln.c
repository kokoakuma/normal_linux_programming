#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// https://github.com/TechEmpower/FrameworkBenchmarks/issues/1202
// vagrantはshared folderにおけるハードリンクに対応していない
int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "%s: no argument\n", argv[0]);
    exit(1);
  }
  if (link(argv[1], argv[2]) < 0)
  {
    perror(argv[1]);
    exit(1);
  }
  exit(0);
}