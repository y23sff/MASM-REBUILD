/* array.h — drop-in replacement for core.h arr_* macros, passing pointers directly.
 *
 *   arch_module_t *arch_registry_arr = NULL;
 *   arr_push(arch_registry_arr, module);
 *   for (size_t i = 0; i < arr_len(arch_registry_arr); i++) { ... }
 */
#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  size_t len;
  size_t cap;
} arr_header;

#define arr_hdr(a) ((arr_header *)(a) - 1)
#define arr_len(a) ((a) ? arr_hdr(a)->len : (size_t)0)
#define arr_cap(a) ((a) ? arr_hdr(a)->cap : (size_t)0)
#define arr_empty(a) (arr_len(a) == 0)

static inline size_t arr_reserve_impl(void **pa, size_t elem_size, size_t n) {
  arr_header *h;
  size_t cap;
  if (*pa == NULL) {
    if (n == 0) return 1;
    cap = 8;
    while (cap < n) cap *= 2;
    h = (arr_header *)calloc(1, sizeof(arr_header) + cap * elem_size);
    if (!h) return 0;
    h->cap = cap;
    *pa = (void *)(h + 1);
    return 1;
  }
  h = arr_hdr(*pa);
  if (h->cap >= n) return 1;
  cap = h->cap ? h->cap : 8;
  while (cap < n) cap *= 2;
  h = (arr_header *)realloc(h, sizeof(arr_header) + cap * elem_size);
  if (!h) return 0;
  h->cap = cap;
  *pa = (void *)(h + 1);
  return 1;
}

#define arr_reserve(a, n) arr_reserve_impl((void **)&(a), sizeof(*(a)), (n))

#define arr_push(a, value)                                                     \
  do {                                                                         \
    if (arr_reserve(a, arr_len(a) + 1)) {                                      \
      (a)[arr_hdr(a)->len++] = (value);                                        \
    }                                                                          \
  } while (0)

#define arr_pop(a) ((a)[--arr_hdr(a)->len])

#define arr_clear(a)                                                           \
  do {                                                                         \
    if (a) arr_hdr(a)->len = 0;                                                \
  } while (0)

#define arr_free(a)                                                            \
  do {                                                                         \
    if (a) {                                                                   \
      free(arr_hdr(a));                                                        \
      (a) = NULL;                                                              \
    }                                                                          \
  } while (0)

#define arr_insert(a, i, value)                                                \
  do {                                                                         \
    size_t _ai = (i);                                                          \
    if (arr_reserve(a, arr_len(a) + 1)) {                                      \
      if (_ai > arr_hdr(a)->len) _ai = arr_hdr(a)->len;                        \
      memmove(&(a)[_ai + 1], &(a)[_ai],                                        \
              (arr_hdr(a)->len - _ai) * sizeof(*(a)));                         \
      (a)[_ai] = (value);                                                      \
      arr_hdr(a)->len++;                                                       \
    }                                                                          \
  } while (0)

#define arr_remove(a, i)                                                       \
  do {                                                                         \
    size_t _ai = (i);                                                          \
    if ((a) && _ai < arr_hdr(a)->len) {                                        \
      memmove(&(a)[_ai], &(a)[_ai + 1],                                        \
              (arr_hdr(a)->len - _ai - 1) * sizeof(*(a)));                     \
      arr_hdr(a)->len--;                                                       \
    }                                                                          \
  } while (0)

#endif /* ARRAY_H */
