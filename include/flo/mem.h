#ifndef BASE_MEM_H
#define BASE_MEM_H

#include <flo.h>

typedef struct Arena Arena;

Arena *arena_new(void);
void arena_free(Arena *);

void *arena_alloc(Arena *, u64 size);
void *arena_allocz(Arena *, u64 size);

void *arena_realloc(Arena *, void *p, u64 size);

void arena_pop(Arena *, u64 size);

void arena_reset(Arena *);

char *arena_fmt(Arena *a, const char *fmt, ...);

#define VA_MIN_SIZE 8
#define arena_va_append(a, l, x)                                               \
  do {                                                                         \
    if ((l)->len >= (l)->cap) {                                                \
      (l)->cap = (l)->cap == 0 ? VA_MIN_SIZE : (l)->cap * 2;                   \
      (l)->data = arena_realloc(a, (l)->data, (l)->cap * sizeof(*(l)->data));  \
      if ((l)->data == NULL) {                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    (l)->data[(l)->len] = x;                                                   \
    (l)->len += 1;                                                             \
  } while (0)

u64 arena_size(const Arena *);

#endif // BASE_MEM_H
