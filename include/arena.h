#pragma once

#include <stddef.h>
#include <stdint.h>

typedef struct Arena *Arena;

Arena arena_init(size_t capacity);
void arena_release(Arena *arena);
void *arena_alloc(Arena arena, size_t size);
void arena_pop(Arena arena, size_t size);
size_t arena_size(Arena arena);
size_t arena_capacity(Arena arena);
void arena_clear(Arena arena);
