#include "generate/column_string_hash.h"

#define BCSH_MALLOC_FAILURE						\
MALLOC_FAILURE_MESSAGE("build_column_string_hash")
#define BCSH_GROUP_MALLOC_FAILURE					\
MALLOC_FAILURE_MESSAGE("build_column_string_hash_group")

extern inline void
hash_state_init(struct HashState *const restrict state,
		void *const restrict buffer,
		const void *const restrict until);

extern inline void
hash_state_shuffle(struct HashState *const restrict state);

extern inline void
set_last_hash_nibble(char *const restrict buffer,
		     const unsigned int last);

extern inline char *
put_last_hash_nibble(char *const restrict buffer,
		     const unsigned int last);

extern inline char *
put_hash_state_odd(char *restrict ptr,
		   struct HashState *const restrict state);
extern inline char *
put_hash_state_even(char *restrict ptr,
		    struct HashState *const restrict state);
extern inline void
set_hash_state_odd(char *restrict ptr,
		   struct HashState *const restrict state);
extern inline void
set_hash_state_even(char *restrict ptr,
		    struct HashState *const restrict state);

void
build_column_string_hash(void *arg)
{
	struct Column *const restrict column
	= (struct Column *const restrict) arg;

	struct Table *const restrict table
	= column->parent;

	/* length of hex character string */
	const size_t length_hash
	= column->spec->type_q.string.scale.fixed;

	/* 1/0 -> odd/even number of hex charaters */
	const size_t odd_nibble = length_hash & 1lu;

	/* one octet → 2 hex characters */
	const size_t length_octets = (length_hash / 2lu) + odd_nibble;

	const size_t size_hash_state_buffer = (sizeof(uint8_t)
					       * (length_octets - 2lu))
					    + (sizeof(word_t) * 2lu);

	const size_t size_hash = length_hash + 1lu;

	const size_t length_column = size_hash * table->spec->row_count;

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

	size_t length_rowspan;
	char *restrict ptr;
	const char *restrict contents_until;
	struct HashState state;

	ptr = column->contents + size_hash_state_buffer;

	/* initialize the hash state */
	hash_state_init(&state,
			column->contents,
			ptr);

	const unsigned int col_count = table->col_count;

	const struct Rowspan *const restrict until = table->rowspans_until;
	struct Rowspan *restrict from		   = column->rowspans_from;

	/* inline char * */
	/* (*const put_hash_state)(char *const restrict, */
	/* 			struct HashState *const restrict) */
	/* = (odd_nibble == 0lu) */
	/* ? put_hash_state_even */
	/* : put_hash_state_odd; */

	/* direct call to put_hash_state function for inlining opportunity */
	if (odd_nibble == 0lu) {
		do {
			length_rowspan = size_hash * from->parent->row_count;

			length_lock_increment(&from->parent->total,
					      length_rowspan,
					      &column->fail_cl);

			contents_until = ptr + length_rowspan;

			from->cell = ptr;

			do {
				ptr = put_hash_state_even(ptr,
							  &state);
			} while (ptr < contents_until);

			from += col_count;
		} while (from < until);
	} else {
		do {
			length_rowspan = size_hash * from->parent->row_count;

			length_lock_increment(&from->parent->total,
					      length_rowspan,
					      &column->fail_cl);

			contents_until = ptr + length_rowspan;

			from->cell = ptr;

			do {
				ptr = put_hash_state_odd(ptr,
							 &state);
			} while (ptr < contents_until);

			from += col_count;
		} while (from < until);
	}

	thread_try_catch_close();
}


void
build_column_string_hash_group(void *arg)
{
	size_t length_rowspan;
	char *restrict ptr;
	size_t *restrict group;
	char *restrict group_string;
	const char *restrict contents_until;
	const char *restrict group_string_until;
	struct HashState state;

	struct Column *const restrict column
	= (struct Column *const restrict) arg;

	struct Table *const restrict table
	= column->parent;

	/* length of hex character string */
	const size_t length_hash
	= column->spec->type_q.string.scale.fixed;

	/* 1/0 -> odd/even number of hex charaters */
	const size_t odd_nibble = length_hash & 1lu;

	/* one octet → 2 hex characters */
	const size_t length_octets = (length_hash / 2lu) + odd_nibble;

	const size_t size_hash_state_buffer = (sizeof(uint8_t)
					       * (length_octets - 2lu))
					    + (sizeof(word_t) * 2lu);

	const size_t size_hash = length_hash + 1lu;

	const size_t row_count = table->spec->row_count;

	const size_t length_column = size_hash * row_count;

	const size_t group_count = column->spec->grp_spec.count;

	GroupPartitioner *const partition_groups
	= column->spec->grp_spec.partition;

	thread_try_catch_open(&free_nullify_cleanup,
			      &column->contents);

	column->contents = malloc((sizeof(size_t) * group_count)
				  + size_hash_state_buffer
				  + length_column);

	if (column->contents == NULL) {
		handler_closure_call(&column->fail_cl,
				     BCSH_MALLOC_FAILURE);
		__builtin_unreachable();
	}

	/* increment table length */
	length_lock_increment(&table->total,
			      length_column,
			      &column->fail_cl);

	const unsigned int col_count = table->col_count;

	const struct Rowspan *const restrict until = table->rowspans_until;
	struct Rowspan *restrict from		   = column->rowspans_from;

	group = (size_t *restrict) (column->contents + size_hash_state_buffer);

	/* initialize the hash state */
	hash_state_init(&state,
			column->contents,
			group);

	group_string = partition_groups(group,
					group_count,
					row_count);

	from->cell = group_string;

	length_rowspan = size_hash * from->parent->row_count;

	length_lock_increment(&from->parent->total,
			      length_rowspan,
			      &column->fail_cl);

	contents_until	   = group_string + length_rowspan;
	group_string_until = group_string + (size_hash * (*group));

	/* direct call to put_hash_state function for inlining opportunity */
	if (odd_nibble == 0lu) {
		ptr = put_hash_state_even(group_string,
					  &state);

		while (1) {
			if (contents_until > group_string_until) {
				while (ptr < group_string_until)
					ptr = put_string_size(ptr,
							      group_string,
							      size_hash);
				group_string = ptr;

				ptr = put_hash_state_even(ptr,
							  &state);

				++group;

				group_string_until = group_string
						   + (size_hash * (*group));
			} else {
				while (ptr < contents_until)
					ptr = put_string_size(ptr,
							      group_string,
							      size_hash);
				from += col_count;

				if (from >= until)
					break;

				from->cell = ptr;

				length_rowspan = size_hash
					       * from->parent->row_count;

				length_lock_increment(&from->parent->total,
						      length_rowspan,
						      &column->fail_cl);

				contents_until = ptr + length_rowspan;
			}
		}
	} else {
		ptr = put_hash_state_odd(group_string,
					 &state);

		while (1) {
			if (contents_until > group_string_until) {
				while (ptr < group_string_until)
					ptr = put_string_size(ptr,
							      group_string,
							      size_hash);
				group_string = ptr;

				ptr = put_hash_state_odd(ptr,
							 &state);

				++group;

				group_string_until = group_string
						   + (size_hash * (*group));
			} else {
				while (ptr < contents_until)
					ptr = put_string_size(ptr,
							      group_string,
							      size_hash);
				from += col_count;

				if (from >= until)
					break;

				from->cell = ptr;

				length_rowspan = size_hash
					       * from->parent->row_count;

				length_lock_increment(&from->parent->total,
						      length_rowspan,
						      &column->fail_cl);

				contents_until = ptr + length_rowspan;
			}
		}
	}

	thread_try_catch_close();
}
