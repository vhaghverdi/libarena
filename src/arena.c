#include "arena.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define DEFAULT_ALIGNMENT (2 * sizeof(void *))

struct Arena {
	char *buffer; // start of arena
	char *pos; // current position
	size_t capacity; // buffer capacity
};

// Return p if already aligned. Otherwise align p to the next boundary.
static uintptr_t align_forward(uintptr_t p, size_t align)
{
	uintptr_t mod = p & (align - 1);
	if (mod != 0)
		p += align - mod;
	return p;
}

Arena arena_init(size_t capacity)
{
	Arena a = calloc(1, sizeof(*a));
	if (a == NULL)
		return NULL;

	// if capacity isn't aligned, align it forward
	capacity = align_forward(capacity, DEFAULT_ALIGNMENT);
	size_t mod = capacity & (DEFAULT_ALIGNMENT - 1);
	size_t pad = mod ? DEFAULT_ALIGNMENT - mod : 0;
	capacity += pad;

	// TODO: use VirtualAlloc (Windows) and mmap (Linux)
	a->buffer = calloc(capacity, 1);
	if (a->buffer == NULL) {
		free(a);
		return NULL;
	}

	a->pos = a->buffer;
	a->capacity = capacity;

	return a;
}

void arena_release(Arena *arena)
{
	if (arena && *arena) {
		free((*arena)->buffer);
		free(*arena);
		*arena = NULL;
	}
}

void *arena_alloc(Arena arena, size_t size)
{
	if (size == 0)
		return NULL;

	uintptr_t mod = (uintptr_t)(arena->pos + size) &
			(DEFAULT_ALIGNMENT - 1);
	uintptr_t padding = mod ? DEFAULT_ALIGNMENT - mod : 0;
	size += padding;

	char *p = arena->pos + size;
	if (p > arena->buffer + arena->capacity)
		return NULL;

	p = arena->pos;
	arena->pos += size;

	return p;
}

void arena_pop(Arena arena, size_t size)
{
	size = align_forward(size, DEFAULT_ALIGNMENT);

	if (size > arena_size(arena))
		arena_clear(arena);
	else
		arena->pos -= size;
}

size_t arena_size(Arena arena)
{
	return arena->pos - arena->buffer;
}

size_t arena_capacity(Arena arena)
{
	return arena->capacity;
}

void arena_clear(Arena arena)
{
	arena->pos = arena->buffer;
}
