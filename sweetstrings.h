#ifndef SWEET_STRINGS_H
#define SWEET_STRINGS_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint32_t len;
	uint32_t cap;
	char*    data;
} sstring;


static inline uint32_t
_sstring_len(const char* c) {
	const char* a = c;
	uint32_t len = 0;
	while (*(a + len) != '\0') {
		len++;
	}
	return len;
}

// Reserve capacity for the string buffer.
// Will only reserve if the new capacity is greater than the current capacity.
static inline void
sstring_reserve(sstring* s, uint32_t c) {
	if (s->cap >= c) return;
	char* data = (char*) realloc(s->data, c);

	if (!data) {};
	s->data = data;
	s->cap = c;
}

// Resize the string.
// Will resize the buffer (changes capacity) if needed.
static inline void
sstring_resize(sstring* s, uint32_t l) {
	if (l == s->len) return;

	if (l < s->len) {
		s->data[l] = '\0';
	} 
	else if (l < s->cap) {
		memset((s->data + s->len), '\0', l - s->len);
	} else {
		sstring_reserve(s, l);
		memset((s->data + s->len), '\0', s->cap - s->len);
	}

	s->len = l;
}

// Deletes a string and frees it's resources.
static inline void
sstring_del(sstring* s) {
	if (!s) return;

	if (s->data) {
		free(s->data);
	}
	s->len = 0;
	s->cap = 0;
}

// Create a new string.
static inline sstring
sstring_new(const char* c) {
	uint32_t len = _sstring_len(c);
	sstring s = {};
	sstring_reserve(&s, len + 1);
	s.len = len;
	memcpy(s.data, c, len);
	s.data[s.len] = '\0';
	return s;
}

// Get a character at the specified index.
static inline char
sstring_get(sstring* s, uint32_t i) {
	assert(i < s->len && "SweetStrings: Out of bounds access");
	return s->data[i];
}

// Set a character at the specified index.
static inline void
sstring_set(sstring* s, uint32_t i, char c) {
	assert(i < s->len && "SweetStrings: Out of bounds access");
	s->data[i] = c;
}

// Clears the data of the string, resetting length to 0.
static inline void
sstring_clear(sstring* s) {
	if (s->len > 0) s->data[0] = '\0';
	s->len = 0;
}

#endif // SWEET_STRINGS_H