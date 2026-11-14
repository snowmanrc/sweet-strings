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
		// nothing needed here?
	} 
	else if (l < s->cap) {
		memset((s->data + s->len), '\0', l - s->len);
	} else {
		sstring_reserve(s, l);
		memset((s->data + s->len), '\0', s->cap - s->len);
	}

	s->len = l;
}

// Create a new string with the specified length.
// The string is zero initialized.
static inline sstring
sstring_new(uint32_t l) {
	sstring s = {};
	sstring_reserve(&s, l);
	s.len = l;
	memset(s.data, '\0', s.len);
	return s;
}

// Create a new string from a char array.
static inline sstring
sstring_create(const char* c) {
	uint32_t len = _sstring_len(c);
	sstring s = {};
	sstring_reserve(&s, len);
	s.len = len;
	memcpy(s.data, c, len);
	return s;
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


// Return the string's length.
static inline uint32_t
sstring_len(sstring* s) {
	return s->len;
}


// Return the string's capacity.
static inline uint32_t
sstring_cap(sstring* s) {
	return s->cap;
}


// Returns a cstring for libc interop
static inline const char*
sstring_cstr(sstring* s, char c) {
	sstring_reserve(s, s->len + 1);
	s->data[s->len] = '\0';
	return s->data;
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


// Fills the entire string with the specified character.
static inline void
sstring_fill(sstring* s, char c) {
	memset(s->data, c, s->len);
}

// Checks whether the string is empty
static inline bool
sstring_isempty(sstring* s) {
	return s->len == 0;
}

#endif // SWEET_STRINGS_H