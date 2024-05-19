#ifndef BASE_MEM_H
#define BASE_MEM_H

#include <base.h>

typedef struct Arena Arena;

Arena *arena_new(void);
void arena_free(Arena *);

void *arena_alloc(Arena *, u64 size);
void *arena_allocz(Arena *, u64 size);

void *arena_realloc(Arena *, void *p, u64 size);

void arena_pop(Arena *, u64 size);

void arena_reset(Arena *);

#endif // BASE_MEM_H
