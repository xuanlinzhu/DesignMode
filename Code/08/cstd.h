#ifndef CSTD_H
#define CSTD_H

#include <stddef.h>

int printf(const char *format, ...);
int strcmp(const char *lhs, const char *rhs);
void *memset(void *ptr, int value, size_t num);
void *memcpy(void *dest, const void *src, size_t num);

#endif
