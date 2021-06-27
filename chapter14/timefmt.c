#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>


int main(int argc, char *argv[])
{
  time_t t;
  struct tm *tm;
  struct timeval tv;

  setlocale(LC_TIME, "");

  time(&t);
  gettimeofday(&tv, NULL);
  printf("time = %ld\n", (long)t);
  printf("ctime = %s", ctime(&t));
  printf("tv.tv_sec = %ld\n", (long)tv.tv_sec);
  printf("tv.tv.usec = %ld\n", (long)tv.tv_usec);
  printf("ctime(tv) = %s", ctime(&tv.tv_sec));

  tm = gmtime(&t);
  printf("asctime(utc) = %s", asctime(tm));
  printf("mktime(utc) = %ld\n", (long)t);
  tm =localtime(&t);
  printf("asctime(local) = %s", asctime(tm));
  printf("mktime(local) = %ld", (long)t);
  exit(0);
}