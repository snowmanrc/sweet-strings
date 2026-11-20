#ifndef SWEET_STRINGS_H
#define SWEET_STRINGS_H

#include <assert.h>
#include <cstdint>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct {
    uint32_t len;
    uint32_t cap;
    char*    data;
} sstring;


#define _sstring_growth_factor(num) num + (num >> 1)


// Create a new string from a char array.
static inline sstring sstring_new(const char* c);

// Create a new string from a char array along with its length.
static inline sstring sstring_newl(const char* c, uint32_t l);

// Create a copy of a string
static inline sstring sstring_copy(const sstring* s);

// Deletes a string and frees it's resources.
static inline void sstring_del(sstring* s);

// Reserve capacity for the string buffer.
// Will only reserve if the new capacity is greater than the current capacity.
static inline void sstring_reserve(sstring* s, uint32_t c);

// Resize the string.
// Will resize the buffer (changes capacity) if needed.
static inline void sstring_resize(sstring* s, uint32_t l);

// Return the string's length.
static inline uint32_t sstring_len(const sstring* s);

// Return the string's capacity.
static inline uint32_t sstring_cap(const sstring* s);

// Returns a cstring for libc interop
static inline const char* sstring_cstr(sstring* s, char c);

// Get a character at the specified index.
static inline char sstring_get(const sstring* s, uint32_t i);

// Set a character at the specified index.
static inline void sstring_set(sstring* s, uint32_t i, char c);

// Pushes a character to the end of the string.
static inline void sstring_push(sstring* s, char c);

// Pops a character from the end of the string.
static inline char sstring_pop(sstring* s);

// Appends a string to end.
static inline void sstring_append(sstring* s, const char* c);

// Appends a string to end. Takes its length for faster appending.
static inline void sstring_appendl(sstring* s, const char* c, uint32_t l);

// Clears the data of the string, resetting length to 0.
static inline void sstring_clear(sstring* s);

// Fills the entire string with the specified character.
static inline void sstring_fill(sstring* s, char c);

// Checks whether the string is empty
static inline bool sstring_isempty(const sstring* s);


static inline uint32_t
_sstring_len(const char* c) {
    const char* a = c;
    uint32_t len = 0;
    while (*(a + len) != '\0') {
        len++;
    }
    return len;
}

static inline 
int32_t _sstring_max(int32_t a, int32_t b) {
    return (a > b) ? a : b;
}

static inline sstring
sstring_new(const char* c) {
	assert(c && "SweetStrings: NULL c-string");

    uint32_t len = _sstring_len(c);
    sstring s = {};
    sstring_reserve(&s, len);
    s.len = len;
    memcpy(s.data, c, len);
    return s;
}


static inline sstring 
sstring_newl(const char* c, uint32_t l) {
	assert(c && "SweetStrings: NULL c string");

    sstring s = {};
    sstring_reserve(&s, l);
    s.len = l;
    memcpy(s.data, c, l);
    return s;
}


static inline sstring sstring_copy(const sstring* s) {
	assert(s && "SweetStrings: NULL string");

	return sstring_newl(s->data, s->len);
}


static inline void
sstring_del(sstring* s) {
	assert(s && "SweetStrings: NULL string");

    if (s->data) {
        free(s->data);
    }
    s->len = 0;
    s->cap = 0;
}


static inline void
sstring_reserve(sstring* s, uint32_t c) {
	assert(s && "SweetStrings: NULL string");

    if (s->cap >= c) return;
    char* data = (char*) realloc(s->data, c);

    if (!data) {};
    s->data = data;
    s->cap = c;
}


static inline void
sstring_resize(sstring* s, uint32_t l) {
	assert(s && "SweetStrings: NULL string");

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


static inline uint32_t
sstring_len(const sstring* s) {
	assert(s && "SweetStrings: NULL string");

    return s->len;
}


static inline uint32_t
sstring_cap(const sstring* s) {
	assert(s && "SweetStrings: NULL string");

    return s->cap;
}


static inline const char*
sstring_cstr(sstring* s) {
	assert(s && "SweetStrings: NULL string");

    sstring_reserve(s, s->len + 1);
    s->data[s->len] = '\0';
    return s->data;
}


static inline char
sstring_get(const sstring* s, uint32_t i) {
	assert(s && "SweetStrings: NULL string");
    assert(i < s->len && "SweetStrings: Out of bounds access");

    return s->data[i];
}


static inline void
sstring_set(sstring* s, uint32_t i, char c) {
	assert(s && "SweetStrings: NULL string");
    assert(i < s->len && "SweetStrings: Out of bounds access");

    s->data[i] = c;
}


static inline void 
sstring_push(sstring* s, char c) {
	assert(s && "SweetStrings: NULL string");

	if(s->len >= s->cap) {
		uint32_t cap = _sstring_growth_factor(s->cap);
		sstring_reserve(s, cap);
		s->cap = cap;
	}
	s->data[s->len++] = c;
}


static inline char 
sstring_pop(sstring* s) {
	assert(s && "SweetStrings: NULL string");
	assert(s->len > 0 && "SweetStrings: Empty string, can not pop.");

	return s->data[--s->len];
}


static inline void 
sstring_append(sstring* s, const char* c) {
	assert(s && "SweetStrings: NULL string");
	assert(c && "SweetStrings: NULL c string");

	uint32_t len = _sstring_len(c);
	if(s->len + len >= s->cap) {
		int32_t cap = _sstring_max(_sstring_growth_factor(s->cap), len);
		sstring_reserve(s, cap);
		s->cap = cap;
	}

	memcpy(s->data + s->len, c, len);
	s->len += len;
}


static inline void 
sstring_appendl(sstring* s, const char* c, uint32_t l) {
	assert(s && "SweetStrings: NULL string");
	assert(c && "SweetStrings: NULL c string");

	if(s->len + l >= s->cap) {
		int32_t cap = _sstring_max(_sstring_growth_factor(s->cap), l);
		sstring_reserve(s, cap);
		s->cap = cap;
	}

	memcpy(s->data + s->len, c, l);
	s->len += l;
}


static inline void
sstring_clear(sstring* s) {
	assert(s && "SweetStrings: NULL string");

    if (s->len > 0) s->data[0] = '\0';
    s->len = 0;
}


static inline void
sstring_fill(sstring* s, char c) {
	assert(s && "SweetStrings: NULL string");

    memset(s->data, c, s->len);
}


static inline bool
sstring_isempty(const sstring* s) {
	assert(s && "SweetStrings: NULL string");
	
    return s->len == 0;
}

#endif // SWEET_STRINGS_H