#ifndef MYSQL_SEED_GENERATE_COLUMN_STRING_NAMES_H_
#define MYSQL_SEED_GENERATE_COLUMN_STRING_NAMES_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include <stdint.h>			/* SIZE_MAX, UINT32_MAX */
#include "random/random.h"		/* urint_t, random_uint_upto */
#include "generate/generator.h"		/* string/parallelization utils */

#define FIRST_NAMES_COUNT 200lu
#define LAST_NAMES_COUNT  200lu
#define FIRST_NAMES_MAP_I_LAST 199l
#define LAST_NAMES_MAP_I_LAST  199l

#define FIRST_NAME_SIZE_MAX 12lu	/* 11 chars + '\0' */
#define LAST_NAME_SIZE_MAX  11lu	/* 10 chars + '\0' */
#define FULL_NAME_SIZE_MAX  (((FIRST_NAME_SIZE_MAX) * 2lu)	\
			     + LAST_NAME_SIZE_MAX		\
			     + 2lu)	/* 2 spaces */

#if (SIZE_MAX < UINT32_MAX)
#	define FIRST_NAMES_COUNT_MAX SIZE_MAX
#	define LAST_NAMES_COUNT_MAX  SIZE_MAX
#	define FULL_NAMES_COUNT_MAX  SIZE_MAX
#else
#	define FIRST_NAMES_COUNT_MAX 450000lu
#	define LAST_NAMES_COUNT_MAX  450000lu
#	define FULL_NAMES_COUNT_MAX  150000lu
#endif /* if (SIZE_MAX < UINT32_MAX) */


/* failure messages
 *─────────────────────────────────────────────────────────────────────────── */
#define BCSN_FIRST_MAX_EXCEEDED						\
FAILURE_REASON("build_column_string_names_first",			\
	       "'FIRST_NAMES_COUNT_MAX' ("				\
	       EXPAND_STRINGIFY(FIRST_NAMES_COUNT_MAX) ") exceeded\n")
#define BCSN_FIRST_MALLOC_FAILURE					\
MALLOC_FAILURE_MESSAGE("build_column_string_names_first")

#define BCSN_LAST_MAX_EXCEEDED						\
FAILURE_REASON("build_column_string_names_last",			\
	       "'LAST_NAMES_COUNT_MAX' ("				\
	       EXPAND_STRINGIFY(LAST_NAMES_COUNT_MAX) ") exceeded\n")
#define BCSN_LAST_MALLOC_FAILURE					\
MALLOC_FAILURE_MESSAGE("build_column_string_names_last")

#define BCSN_FULL_MAX_EXCEEDED						\
FAILURE_REASON("build_column_string_names_full",			\
	       "'FULL_NAMES_COUNT_MAX' ("				\
	       EXPAND_STRINGIFY(FULL_NAMES_COUNT_MAX) ") exceeded\n")
#define BCSN_FULL_MALLOC_FAILURE					\
MALLOC_FAILURE_MESSAGE("build_column_string_names_full")


/* enum, struct declarations
 *─────────────────────────────────────────────────────────────────────────── */
struct NameMap {
	const struct Label *names;
	const urint_t i_last;
};

/* global variables
 *─────────────────────────────────────────────────────────────────────────── */
extern const struct Label first_names[FIRST_NAMES_COUNT];
extern const struct Label last_names[LAST_NAMES_COUNT];

extern const struct NameMap first_name_map;
extern const struct NameMap last_name_map;

inline char *
put_random_initial(char *const restrict string)
{
	*string = (char) random_int_in_range((rint_t) 'A',
					     (rint_t) 'Z');
	return string + 1l;
}

inline const struct Label *
name_map_sample(const struct NameMap *const restrict map)
{
	return &map->names[ random_uint_upto(map->i_last) ];
}

inline char *
put_single_names(char *restrict ptr,
		 const struct NameMap *const restrict map,
		 size_t count)
{
	do {
		ptr = put_label(ptr,
				name_map_sample(map));
		*ptr = '\0';
		++ptr;

		--count;
	} while (count > 0lu);

	return ptr;
}

inline char *
put_full_names(char *restrict ptr,
	       size_t count)
{
	do {
		ptr = put_label(ptr,
				name_map_sample(&first_name_map));

		*ptr = ' ';
		++ptr;

		if (coin_flip()) {
			ptr = coin_flip()
			      ? put_random_initial(ptr)
			      : put_label(ptr,
					  name_map_sample(&first_name_map));

			*ptr = ' ';
			++ptr;
		}

		ptr = put_label(ptr,
				name_map_sample(&last_name_map));

		*ptr = '\0';
		++ptr;

		--count;
	} while (count > 0lu);

	return ptr;
}


void
build_column_names_first(void *arg);

void
build_column_names_last(void *arg);

void
build_column_names_full(void *arg);


#endif	/* ifndef MYSQL_SEED_GENERATE_COLUMN_STRING_NAMES_H_ */
