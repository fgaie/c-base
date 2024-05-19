#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <flo.h>
#include <flo/mem.h>

#define ARENA_SIZE (1024 * 1024)

struct Arena {
  u8 data[ARENA_SIZE];
  u64 top;

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

void arena_pop(Arena *a, u64 size) {
  a->top = a->top > size ? a->top - size : 0;
}

void arena_reset(Arena *a) {
  arena_free(a->next);
  a->top = 0;
}
