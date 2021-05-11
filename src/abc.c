#include <stdlib.h>
#include <stdio.h>

void __attribute__ ((constructor)) nu_init(void);
void __attribute__ ((destructor)) nu_fini(void);

void __attribute__ ((constructor)) nu_init(void)
{
  fprintf(stderr, "start\n");
}

void __attribute__ ((destructor)) nu_fini(void)
{
  fprintf(stderr, "finish\n");
}

extern void *__libc_malloc(size_t size);
extern void __libc_free(void *ptr);
extern void *__libc_calloc(size_t nmemb, size_t size);
extern void *__libc_realloc(void *ptr, size_t size);

void *malloc(size_t size)
{
  void *ptr = __libc_malloc(size);
  fprintf(stderr, "malloc %s, %d\n", __FILE__, __LINE__);
  return ptr;
}

void free(void* ptr)
{
  fprintf(stderr, "free\n");
  __libc_free(ptr);
}

void *calloc(size_t nmemb, size_t size)
{
  void *ptr = __libc_calloc(nmemb, size);
  fprintf(stderr, "calloc\n");
  return ptr;
}

void *realloc(void *ptr, size_t size)
{
  void *newptr = __libc_realloc(ptr, size);
  fprintf(stderr, "realloc\n");
  return newptr;
}

// LD_PRELOAD=`pwd`/libabc.so ./test