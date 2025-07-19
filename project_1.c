#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE "Hello, dynamic memory!"
#define MAX_SIZE 1000

int main() {
  char *arr = malloc(MAX_SIZE);
  if (arr == NULL) {
    printf("n/a\n");
    free(arr);
    return 1;
  }

  strcpy(arr, LINE);
  printf("%s", arr);
  free(arr);

  return 0;
}