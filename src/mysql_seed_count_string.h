#ifndef MYSQL_SEED_MYSQL_SEED_COUNT_STRING_H_
#define MYSQL_SEED_MYSQL_SEED_COUNT_STRING_H_

#define DEFAULT_USER_COUNT 25000

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include <stdlib.h>		/* malloc, free */
#include "mysql_seed_log.h"	/* seed_log, string utils */

#if (SIZE_MAX < UINT32_MAX)
#	define UPTO_MAX 9999lu
#	define UPTO_MAX_DIGIT_COUNT 4u
#else
#	define UPTO_MAX 99999999lu
#	define UPTO_MAX_DIGIT_COUNT 8u
#endif /* if (SIZE_MAX < UINT32_MAX) */

#define SIZE_1_9		18lu	   /* (1 digit  + '\0') * 9 */
#define SIZE_10_99		270lu	   /* (2 digits + '\0') * 90 */
#define SIZE_100_999		3600lu	   /* (3 digits + '\0') * 900 */
#define SIZE_1_99		(SIZE_1_9	+ SIZE_10_99)
#define SIZE_1_999		(SIZE_1_99	+ SIZE_100_999)

#if (UPTO_MAX == 99999999lu)
#define SIZE_1000_9999		45000lu	   /* (3 digits + '\0') * 9000 */
#define SIZE_10000_99999	540000lu   /* (4 digits + '\0') * 90000 */
#define SIZE_100000_999999	6300000lu  /* (5 digits + '\0') * 900000 */
#define SIZE_1000000_9999999	72000000lu /* (6 digits + '\0') * 9000000 */
#define SIZE_1_9999		(SIZE_1_999	+ SIZE_1000_9999)
#define SIZE_1_99999		(SIZE_1_9999	+ SIZE_10000_99999)
#define SIZE_1_999999		(SIZE_1_99999	+ SIZE_100000_999999)
#define SIZE_1_9999999		(SIZE_1_999999	+ SIZE_1000000_9999999)
#endif	/*  if (UPTO_MAX >= 9999999lu) */

#define UPTO_MAX_EXCEEDED_FAILURE_MESSAGE "'UPTO_MAX' exceeded\n"
#define MALLOC_FAILURE_MESSAGE		  "malloc failure (out of memory)\n"

#define CS_ALLOC_FAILURE_MESSAGE_BEGIN				\
"failed to allocate count string memory for 'upto' of "

#define CS_ALLOC_FAILURE_MESSAGE_MIDDLE				\
" ('UPTO_MAX' = " EXPAND_STRINGIFY(UPTO_MAX) ")\nreason:\n\t"

/* struct declarations
 *─────────────────────────────────────────────────────────────────────────── */
struct CountStringSpec {
	unsigned int mag_upto;	/* ⌊ log₁₀(upto) ⌋ */
	size_t base_upto;	/* 10^mag_upto */
	size_t size_until_base;	/* sizeof("1", "2", ... "base - 1") */
	size_t size_upto_upto;	/* sizeof("1", "2", ... "upto") */
};




/* #if (CHAR_BIT < 7) */
/* #error "incompatible 'char' ('CHAR_BIT' = " EXPAND_STRINGIFY(CHAR_BIT) ")" */
/* #endif */

/* #if	(ULONG_MAX < UINT64_MAX) */
/* typedef uint_fast32_t Word; */
/* #define WORD_MAX UINT_FAST32_MAX */
/* #define WORD_WIDTH 4 */

/* #else */

/* typedef uint_fast64_t Word; */
/* #define WORD_MAX UINT_FAST64_MAX */
/* #define WORD_WIDTH 8 */
/* #endif /1* if (ULONG_MAX < UINT64_MAX) *1/ */


union DigitsBuffer {
	char string[UPTO_MAX_DIGIT_COUNT + 1u];
#if (UPTO_MAX_DIGIT_COUNT > 4u)
	CharBuff9 width_9;
	CharBuff8 width_8;
	CharBuff7 width_7;
	CharBuff6 width_6;
#endif /* if (UPTO_MAX_DIGIT_COUNT > 4u) */
	CharBuff5 width_5;
	CharBuff4 width_4;
	CharBuff3 width_3;
	CharBuff2 width_2;
};

union DigitsPointer {
	char string[UPTO_MAX_DIGIT_COUNT + 1u];
#if (UPTO_MAX_DIGIT_COUNT > 4u)
	CharBuff9 *restrict width_9;
	CharBuff8 *restrict width_8;
	CharBuff7 *restrict width_7;
	CharBuff6 *restrict width_6;
#endif /* if (UPTO_MAX_DIGIT_COUNT > 4u) */
	CharBuff5 *restrict width_5;
	CharBuff4 *restrict width_4;
	CharBuff3 *restrict width_3;
	CharBuff2 *restrict width_2;
};




struct CountStringBuilder {
	union DigitsBuffer buffer;
	union DigitsPointer pointer;
	char *restrict active;
};




/* misc helper functions
 *─────────────────────────────────────────────────────────────────────────── */
/* count of chars required for "1", "2", ... "upto" null-terminated ascii
 * strings plus a final '\0' character to indicate end */
inline void
count_string_spec_init(struct CountStringSpec *const restrict spec,
		       const size_t upto)
{
#if (UPTO_MAX == 99999999lu)
	if (upto < 10000lu) {
#endif	/*  if (UPTO_MAX == 9999999lu) */
		if (upto < 100lu) {
			if (upto < 10lu) {
				spec->mag_upto	      = 0u;
				spec->base_upto	      = 1lu;
				spec->size_until_base = 0lu;
				spec->size_upto_upto  = 2lu * upto;
			} else {
				spec->mag_upto	      = 1u;
				spec->base_upto	      = 10lu;
				spec->size_until_base = SIZE_1_9;
				spec->size_upto_upto  = SIZE_1_9
						      + (3lu
							 * (upto - 9lu));
			}
		} else {
			if (upto < 1000lu) {
				spec->mag_upto	      = 2u;
				spec->base_upto	      = 100lu;
				spec->size_until_base = SIZE_1_99;
				spec->size_upto_upto  = SIZE_1_99
						      + (4lu
							 * (upto - 99lu));
			} else {
				spec->mag_upto	      = 3u;
				spec->base_upto	      = 1000lu;
				spec->size_until_base = SIZE_1_999;
				spec->size_upto_upto  = SIZE_1_999
						      + (5lu
							 * (upto - 999lu));
			}
		}
#if (UPTO_MAX == 99999999lu)
	} else {
		if (upto < 1000000lu) {
			if (upto < 100000lu) {
				spec->mag_upto	      = 4u;
				spec->base_upto	      = 10000lu;
				spec->size_until_base = SIZE_1_9999;
				spec->size_upto_upto  = SIZE_1_9999
						      + (6lu
							 * (upto - 9999lu));
			} else {
				spec->mag_upto	      = 5u;
				spec->base_upto	      = 100000lu;
				spec->size_until_base = SIZE_1_99999;
				spec->size_upto_upto  = SIZE_1_99999
						      + (7lu
							 * (upto - 99999lu));
			}
		} else {
			if (upto < 10000000lu) {
				spec->mag_upto	      = 6u;
				spec->base_upto	      = 1000000lu;
				spec->size_until_base = SIZE_1_999999;
				spec->size_upto_upto  = SIZE_1_999999
						      + (8lu
							 * (upto - 999999lu));
			} else {
				spec->mag_upto	      = 7u;
				spec->base_upto	      = 10000000lu;
				spec->size_until_base = SIZE_1_9999999;
				spec->size_upto_upto  = SIZE_1_9999999
						      + (9lu
							 * (upto - 9999999lu));
			}
		}
	}
#endif	/*  if (UPTO_MAX == 9999999lu) */
}



inline void
count_string_log_alloc_failure(const size_t upto,
			       const char *const restrict failure)
{

	seed_log_handle_lock();

	seed_log_append_string(CS_ALLOC_FAILURE_MESSAGE_BEGIN);

	seed_log_append_digits(upto);

	seed_log_append_string(CS_ALLOC_FAILURE_MESSAGE_MIDDLE);

	seed_log_append_string(failure);

	seed_log_handle_unlock();
}

inline void
count_string_init(char *const restrict string,
		  const struct CountStringSpec *const restrict spec,
		  size_t upto)
{
	char *restrict ptr;

	ptr = string[spec->size_until_base];

	/* *ptr = '\0'; */
	/* --ptr; */

	switch (spec->mag_upto) {
#if (UPTO_MAX_DIGIT_COUNT == 8u)
	case 7u:
	case 6u:
	case 5u:
	case 4u:
#endif /* if (UPTO_MAX_DIGIT_COUNT == 8u) */
	case 3u:
	case 2u:
	case 1u:
	default:
		/* string[spec->size_upto_upto] = '\0'; */
		return;
	}
}

/* top-level functions
 *─────────────────────────────────────────────────────────────────────────── */
inline char *
count_string_create(const size_t upto)
{
	if (upto > UPTO_MAX) {
		count_string_log_alloc_failure(upto,
					       UPTO_MAX_EXCEEDED_FAILURE_MESSAGE);
		return NULL;
	}

	struct CountStringSpec spec;

	count_string_spec_init(&spec,
			       upto);

	/* + 1 for final null byte */
	char *const restrict string = malloc(spec.size_upto_upto + 1lu);


	if (string == NULL) {
		count_string_log_alloc_failure(upto,
					       MALLOC_FAILURE_MESSAGE);
		return NULL;
	}

	count_string_init(string,
			  &spec,
			  upto);

	return string;
}


#endif	/* MYSQL_SEED_MYSQL_SEED_COUNT_STRING_H_ */
