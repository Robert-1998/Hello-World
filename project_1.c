#include <stdio.h>
#include <stdlib.h>

int main() {
  int n;
  if (scanf("%d", &n) != 1) {
    printf("n/a\n");
    return 0;
  }
  int *arr = malloc(n * sizeof(int));
  if (arr == NULL) {
    printf("n/a\n");
    free(arr);
    return 0;
  }
  for (int i = 0; i < n; i++) {
    if (scanf("%d", &arr[i]) != 1) {
      printf("n/a\n");
      free(arr);
      return 0;
    }
  }
  for (int i = 0; i < n; i++) {
    printf("%d", arr[i]);
    if (i != n - 1) {
      printf(" ");
    }
  }
  return 0;
}