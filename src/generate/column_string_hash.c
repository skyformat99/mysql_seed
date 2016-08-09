#include "generate/column_string_hash.h"

#define BCSH_MALLOC_FAILURE						\
MALLOC_FAILURE_MESSAGE("build_column_string_hash")

extern inline void
hash_state_init(struct HashState *const restrict state,
		void *const restrict buffer,
		const void *const restrict until);

extern inline void
set_last_hash_nibble(char *const restrict buffer,
		     const word_t last);

void
build_column_string_hash(void *arg)
{
	struct Column *const restrict column
	= (struct Column *const restrict) arg;

	struct Table *const restrict table
	= column->parent;

	/* length of hex character string */
	const size_t length_hash = column->spec->string.length_scale.fixed;

	/* one octet → 2 hex characters */
	const size_t length_octets = (length_hash / 2lu) + (length_hash & 1lu);

	const size_t size_hash_state_buffer = (sizeof(uint8_t)
					       * (length_octets - 2lu))
					    + (sizeof(word_t) * 2lu);

	const size_t length_column = (length_hash + 1lu)
				   * table->spec->row_count;

	thread_try_catch_open(&free_nullify_cleanup,
			      &column->contents);

	column->contents = malloc(size_hash_state_buffer + length_column);

	if (column->contents == NULL) {
		handler_closure_call(&column->fail_cl,
				     BCSH_MALLOC_FAILURE);
		__builtin_unreachable();
	}

	/* increment table length */
	length_lock_increment(&table->total,
			      length_column,
			      &column->fail_cl);

	char *restrict ptr;
	struct HashState state;

	ptr = column->contents + size_hash_state_buffer;

	hash_state_init(&state,
			column->contents,
			ptr);




	thread_try_catch_close();
}

/* hash_t hash_bytes(byte_t *bytes, */
/* 		  const size_t length, */
/* 		  const hash_t m_prime) */
/* { */

/* 	/1* size_t rem_words = length / sizeof(hash_t); *1/ */
/* 	size_t rem_bytes = length % sizeof(hash_t); */

/* 	ptrdiff_t i = length - rem_bytes; */

/* 	hash_t *word = (hash_t *) bytes; */

/* 	hash_t hash = (word[i] << (sizeof(hash_t) - rem_bytes)) & m_prime; */

/* 	i -= rem_bytes; */

/* 	if (i < 0l) */
/* 		return hash; */

/* 	const hash_t base_mod_size = HASH_MAX & m_prime; */

/* 	do { */
/* 		hash += (base_mod_size * (word[i] & m_prime)) & m_prime; */
/* 		i -= sizeof(hash_t); */

/* 	} while (i >= 0l); */

/* 	return hash; */
/* } */
