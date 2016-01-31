/*
 * Copyleft 2012 Leeds Met, Z. Cliffe Schreuders
 * May be distributed under the terms of the GPLv3
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main()
{
  printf("          UID             GID  \n"
     "Real      %d  Real      %d  \n"
     "Effective %d  Effective %d  \n",
     getuid (),     getgid (),
     geteuid(),     getegid()
								         );
  FILE *fp = fopen("mysecret", "r");
  if (fp == NULL) {
    printf("Error: Could not open file");
    exit(EXIT_FAILURE);
  }
  char c;
  while ((c=getc(fp)) != EOF)
    putchar(c);
  putchar('\n');

  return EXIT_SUCCESS;
}
