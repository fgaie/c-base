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

u64 arena_size(const Arena *);

#endif // BASE_MEM_H
