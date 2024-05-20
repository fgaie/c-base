#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <flo.h>
#include <flo/mem.h>

#define ARENA_SIZE (1024 * 1024)

struct Arena {
  u8 data[ARENA_SIZE];
  u64 top;
  u64 last;

  Arena *next;
};

Arena *arena_new(void) {
  Arena *res = malloc(sizeof(*res));
  if (res == NULL) {
    fprintf(stderr, "arena_new: Could not allocate %lu bytes: %s\n",
            sizeof(*res), strerror(errno));
    return NULL;
  }

  res->top = 0;
  res->last = -1;
  res->next = NULL;

  return res;
}

void arena_free(Arena *a) {
  if (a != NULL) {
    arena_free(a->next);
    free(a);
  }
}

void *arena_alloc(Arena *a, u64 size) {
  if (a == NULL) {
    return malloc(size);
  }

  if (size > ARENA_SIZE) {
    fprintf(stderr, "arena_alloc: Could not allocate more then %d bytes\n",
            ARENA_SIZE);
    return NULL;
  }

  if (a->top + size > ARENA_SIZE) {
    if (a->next == NULL) {
      a->next = arena_new();
      if (a->next == NULL) {
        fprintf(stderr, "arena_alloc: Could not allocate %lu bytes: %s\n", size,
                strerror(errno));
        return NULL;
      }

      return arena_alloc(a, size);
    }
  }

  void *res = &a->data[a->top];
  a->last = a->top;
  a->top += size;
  return res;
}

void *arena_allocz(Arena *a, u64 size) {
  void *res = arena_alloc(a, size);
  if (res != NULL) {
    memset(res, 0, size);
  }
  return res;
}

void *arena_realloc(Arena *a, void *p, u64 size) {
  if (a == NULL) {
    return realloc(p, size);
  }

  if (p == &a->data[a->last] && size <= ARENA_SIZE - a->last) {
    a->top = a->last + size;
    return p;
  }

  void *res = arena_alloc(a, size);
  if (p != NULL && res != NULL) {
    // souds dangerous
    memcpy(res, p, size);
  }

  return res;
}

void arena_pop(Arena *a, u64 size) {
  a->top = a->top > size ? a->top - size : 0;
}

void arena_reset(Arena *a) {
  arena_free(a->next);
  a->top = 0;
}

char *arena_fmt(Arena *a, const char *fmt, ...) {
  va_list ap;
  va_list copy;
  va_start(ap, fmt);
  va_copy(copy, ap);

  int size = vsnprintf(NULL, 0, fmt, ap);
  if (size < 0) {
    fprintf(stderr, "Could not format `%s`: %s\n", fmt, strerror(errno));
    return "";
  }

  char *res = arena_allocz(a, size + 1);
  vsnprintf(res, size + 1, fmt, copy);

  va_end(ap);
  va_end(copy);

  return res;
}

u64 arena_size(const Arena *a) {
  u64 res = 0;
  while (a != NULL) {
    res += a->top;
    a = a->next;
  }

  return res;
}
