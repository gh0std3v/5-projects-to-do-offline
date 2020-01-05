/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2020-01-04T18:59:55-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2020-01-04T19:09:13-06:00
 */

#include "alloc.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
  char *ptr = malloc_c(10);
  strncpy(ptr, "Hey there", 9);
  assert(ptr && !strcmp(ptr, "Hey there"));
  int *arr = calloc_c(100, sizeof(int));
  for (int i = 0; i < 100; i++)
    arr[i] = i+1;
  for (int i = 0; i < 100; i++)
    assert(arr[i] == i+1);
  void *q = malloc_c(10000);
  assert(q == NULL);
  q = calloc_c(2, 10000);
  assert(q == NULL);
  printf("All tests passed.\n");
  return 0;
}
