#include <stdio.h>
#define STRSIZE 22

int main(int argc, char *argv[]) {

  char s1[] = "break";
  char *s2 = "breakpoints";
  char logo[STRSIZE];

  snprintf(logo, STRSIZE, "%c %s %d %s.", 'I', s1, 2 + 2, s2);
  puts(logo);

  return 0;
}
