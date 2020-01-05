/**
 * @Author: S. Sharma <silentcat>
 * @Date:   2020-01-04T18:48:40-06:00
 * @Email:  silentcat@protonmail.com
 * @Last modified by:   silentcat
 * @Last modified time: 2020-01-04T19:19:01-06:00
 */

#include "alloc.h"
#include <stdio.h>

static int len = 0;
static char alloc[MAX_LEN];

void *malloc_c(int size)
{
  void *ptr = NULL;
  if (MAX_LEN-len >= size) {
    ptr = alloc + len;
    len += size;
    return ptr;
  }
  return ptr;
}

void *calloc_c(int n, int size_each)
{
  void *ptr = malloc_c(n*size_each);
  return ptr;
}
