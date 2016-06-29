#ifndef MYSQL_SEED_GEN_GEN_COUNT_STRING_H_
#define MYSQL_SEED_GEN_GEN_COUNT_STRING_H_

#define DEFAULT_USER_COUNT 25000

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "gen/gen_strings.h"	/* string utils, log, parallelization utils */

#if (SIZE_MAX < UINT32_MAX)
#	define UPTO_MAX 9999lu
#	define MAG_UPTO_MAX 3u
#	define SIZE_UPTO_MAX_STR 5u
#	undef  LARGE_UPTO_MAX
#else
#	define UPTO_MAX 99999999lu
#	define MAG_UPTO_MAX 7u
#	define SIZE_UPTO_MAX_STR 9u
#	define LARGE_UPTO_MAX
#endif /* if (SIZE_MAX < UINT32_MAX) */



/* failure messages
 *─────────────────────────────────────────────────────────────────────────── */
#define UPTO_MAX_EXCEEDED_FAILURE_MESSAGE "'UPTO_MAX' exceeded\n"

#define CS_ALLOC_FAILURE_MESSAGE_BEGIN					\
"\n\nfailed to allocate count string memory for 'upto' of "

#define CS_ALLOC_FAILURE_MESSAGE_MIDDLE					\
" ('UPTO_MAX' = " EXPAND_STRINGIFY(UPTO_MAX) ")\nreason:\n\t"

#define CS_INIT_FAILURE_MESSAGE						\
"\n\nfailed to retrieve 'count_string'\nreason:\n\tgeneration failure\n"

/* struct declarations, typedefs
 *─────────────────────────────────────────────────────────────────────────── */
#ifdef LARGE_UPTO_MAX
typedef CharBuffer9 Mag7String;
typedef CharBuffer8 Mag6String;
typedef CharBuffer7 Mag5String;
typedef CharBuffer6 Mag4String;
#endif /* ifdef LARGE_UPTO_MAX */
typedef CharBuffer5 Mag3String;
typedef CharBuffer4 Mag2String;
typedef CharBuffer3 Mag1String;
typedef CharBuffer2 Mag0String;

union DigitsBuffer {
	char string[SIZE_UPTO_MAX_STR];
#ifdef LARGE_UPTO_MAX
	Mag7String mag_7;
	Mag6String mag_6;
	Mag5String mag_5;
	Mag4String mag_4;
#endif /* ifdef LARGE_UPTO_MAX */
	Mag3String mag_3;
	Mag2String mag_2;
	Mag1String mag_1;
	Mag0String mag_0;
};

union DigitsPointer {
	char *restrict string;
#ifdef LARGE_UPTO_MAX
	Mag7String *restrict mag_7;
	Mag6String *restrict mag_6;
	Mag5String *restrict mag_5;
	Mag4String *restrict mag_4;
#endif /* ifdef LARGE_UPTO_MAX */
	Mag3String *restrict mag_3;
	Mag2String *restrict mag_2;
	Mag1String *restrict mag_1;
	Mag0String *restrict mag_0;
};

struct CountStringSpec {
	unsigned int mag_upto;	/* ⌊ log₁₀(upto) ⌋ */
	size_t size_digits;	/* sizeof("1", "2", ... "upto") */
};

struct CountString {
	size_t upto;
	bool incomplete;	/* flipped false once 'pointers' are set */
	char **pointers;	/* digit pointers */
	SeedThreadCond done;	/* broadcasted once 'pointers' are set */
	SeedMutex processing;	/* condition lock */
};

/* macro constants
 *─────────────────────────────────────────────────────────────────────────── */
#define MAG_0_MIN		1lu
#define MAG_1_MIN		10lu
#define MAG_2_MIN		100lu
#define MAG_3_MIN		1000lu
#define MAG_0_MAX		9lu
#define MAG_1_MAX		99lu
#define MAG_2_MAX		999lu
#define MAG_3_MAX		9999lu
#define OFF_MAG_0_MIN		0l
#define OFF_MAG_1_MIN		MAG_0_MAX
#define OFF_MAG_2_MIN		MAG_1_MAX
#define OFF_MAG_3_MIN		MAG_2_MAX
#define SIZE_MAG_0_STR		2lu
#define SIZE_MAG_1_STR		3lu
#define SIZE_MAG_2_STR		4lu
#define SIZE_MAG_3_STR		5lu
#define SIZE_MAG_0_0_STR	0lu	   /* empty */
#define SIZE_MAG_0_1_STR	18lu	   /* (1 digit  + '\0') * 9 */
#define SIZE_MAG_1_2_STR	270lu	   /* (2 digits + '\0') * 90 */
#define SIZE_MAG_2_3_STR	3600lu	   /* (3 digits + '\0') * 900 */
#define SIZE_MAG_0_2_STR	(SIZE_MAG_0_1_STR + SIZE_MAG_1_2_STR)
#define SIZE_MAG_0_3_STR	(SIZE_MAG_0_2_STR + SIZE_MAG_2_3_STR)
#ifdef LARGE_UPTO_MAX
#	define MAG_4_MIN	10000lu
#	define MAG_5_MIN	100000lu
#	define MAG_6_MIN	1000000lu
#	define MAG_7_MIN	10000000lu
#	define MAG_4_MAX	99999lu
#	define MAG_5_MAX	999999lu
#	define MAG_6_MAX	9999999lu
#	define MAG_7_MAX	99999999lu
#	define OFF_MAG_4_MIN	MAG_3_MAX
#	define OFF_MAG_5_MIN	MAG_4_MAX
#	define OFF_MAG_6_MIN	MAG_5_MAX
#	define OFF_MAG_7_MIN	MAG_6_MAX
#	define SIZE_MAG_4_STR	6lu
#	define SIZE_MAG_5_STR	7lu
#	define SIZE_MAG_6_STR	8lu
#	define SIZE_MAG_7_STR	9lu
#	define SIZE_MAG_3_4_STR	45000lu	   /* (3 digits + '\0') * 9000 */
#	define SIZE_MAG_4_5_STR	540000lu   /* (4 digits + '\0') * 90000 */
#	define SIZE_MAG_5_6_STR	6300000lu  /* (5 digits + '\0') * 900000 */
#	define SIZE_MAG_6_7_STR	72000000lu /* (6 digits + '\0') * 9000000 */
#	define SIZE_MAG_0_4_STR	(SIZE_MAG_0_3_STR + SIZE_MAG_3_4_STR)
#	define SIZE_MAG_0_5_STR	(SIZE_MAG_0_4_STR + SIZE_MAG_4_5_STR)
#	define SIZE_MAG_0_6_STR	(SIZE_MAG_0_5_STR + SIZE_MAG_5_6_STR)
#	define SIZE_MAG_0_7_STR	(SIZE_MAG_0_6_STR + SIZE_MAG_6_7_STR)
#endif	/*  ifdef LARGE_UPTO_MAX */


/* global variables
 *─────────────────────────────────────────────────────────────────────────── */
extern const Mag0String mag_0_min_string;
extern const Mag1String mag_1_min_string;
extern const Mag2String mag_2_min_string;
extern const Mag3String mag_3_min_string;
#ifdef LARGE_UPTO_MAX
extern const Mag4String mag_4_min_string;
extern const Mag5String mag_5_min_string;
extern const Mag6String mag_6_min_string;
extern const Mag7String mag_7_min_string;
#endif	/*  ifdef LARGE_UPTO_MAX */

extern struct CountString count_string;
extern const struct timespec count_string_await_span;

/* misc helper functions
 *─────────────────────────────────────────────────────────────────────────── */
/* count of chars required for "1", "2", ... "upto" null-terminated ascii
 * strings plus a final '\0' character to indicate end */
inline void
count_string_spec_init(struct CountStringSpec *const restrict spec,
		       const size_t upto)
{
#if (UPTO_MAX == 99999999lu)
	if (upto < MAG_4_MIN) {
#endif	/*  if (UPTO_MAX == 9999999lu) */
		if (upto < MAG_2_MIN) {
			if (upto < MAG_1_MIN) {
				spec->mag_upto	  = 0u;
				spec->size_digits = SIZE_MAG_0_STR * upto;
			} else {
				spec->mag_upto	  = 1u;
				spec->size_digits = SIZE_MAG_0_1_STR
						  + (SIZE_MAG_1_STR
						     * (upto - MAG_0_MAX));
			}
		} else {
			if (upto < MAG_3_MIN) {
				spec->mag_upto	  = 2u;
				spec->size_digits = SIZE_MAG_0_2_STR
						  + (SIZE_MAG_2_STR
						     * (upto - MAG_1_MAX));
			} else {
				spec->mag_upto	  = 3u;
				spec->size_digits = SIZE_MAG_0_3_STR
						  + (SIZE_MAG_3_STR
						     * (upto - MAG_2_MAX));
			}
		}
#if (UPTO_MAX == 99999999lu)
	} else {
		if (upto < MAG_6_MIN) {
			if (upto < MAG_5_MIN) {
				spec->mag_upto	  = 4u;
				spec->size_digits = SIZE_MAG_0_4_STR
						  + (SIZE_MAG_4_STR
						     * (upto - MAG_3_MAX));
			} else {
				spec->mag_upto	  = 5u;
				spec->size_digits = SIZE_MAG_0_5_STR
						  + (SIZE_MAG_5_STR
						     * (upto - MAG_4_MAX));
			}
		} else {
			if (upto < MAG_7_MIN) {
				spec->mag_upto	  = 6u;
				spec->size_digits = SIZE_MAG_0_6_STR
						  + (SIZE_MAG_6_STR
						     * (upto - MAG_5_MAX));
			} else {
				spec->mag_upto	  = 7u;
				spec->size_digits = SIZE_MAG_0_7_STR
						  + (SIZE_MAG_7_STR
						     * (upto - MAG_6_MAX));
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
count_buffer_increment(char *restrict digit)
{
	while ((*digit) == '9') {
		*digit = '0';
		--digit;
	}

	/* digit is in '0' ... '9' */
	++(*digit);
	return;
}



#define SET_RANGE_DIGITS_MAG_UPTO(MAG)					\
do {									\
	digits_ptr.string      = digits + SIZE_MAG_0_ ## MAG ## _STR;	\
	digits_buf.mag_ ## MAG = mag_ ## MAG ##_min_string;		\
	while (1) {							\
		*(digits_ptr.mag_ ## MAG) = digits_buf.mag_ ## MAG;	\
		*current_ptr = digits_ptr.string;			\
		++current_ptr;						\
		if (current_ptr == upto_ptr)				\
			break;						\
		++(digits_ptr.mag_ ## MAG);				\
		count_buffer_increment(active);				\
	}								\
} while (0)

#define SET_RANGE_DIGITS_MAG_UPTO_GT_0(MAG)				\
do {									\
		from_ptr    = string_ptrs + OFF_MAG_ ## MAG ## _MIN;	\
		current_ptr = from_ptr;					\
		SET_RANGE_DIGITS_MAG_UPTO(MAG);				\
		upto_ptr    = from_ptr;					\
		--active;						\
} while (0)

inline void
count_string_pointers_init(char *restrict *const string_ptrs,
			   const unsigned int mag_upto,
			   const size_t upto)
{
	union DigitsBuffer digits_buf;
	union DigitsPointer digits_ptr;
	char *restrict active;
	char *restrict *current_ptr;
	char *restrict *from_ptr;
	char *restrict *upto_ptr;

	/* separate 'upto' x string pointers from 'digits' with 'NULL' */
	upto_ptr  = string_ptrs + upto;
	*upto_ptr = NULL;

	/* point 'digits' after NULL terminator */
	char *const restrict digits = (char *const restrict) (upto_ptr + 1l);

	/* point 'active' at one's digit */
	active = &digits_buf.string[mag_upto];


	switch (mag_upto) {
#ifdef LARGE_UPTO_MAX
	case 7u:
		SET_RANGE_DIGITS_MAG_UPTO_GT_0(7);
	case 6u:
		SET_RANGE_DIGITS_MAG_UPTO_GT_0(6);
	case 5u:
		SET_RANGE_DIGITS_MAG_UPTO_GT_0(5);
	case 4u:
		SET_RANGE_DIGITS_MAG_UPTO_GT_0(4);
#endif /* ifdef LARGE_UPTO_MAX */
	case 3u:
		SET_RANGE_DIGITS_MAG_UPTO_GT_0(3);
	case 2u:
		SET_RANGE_DIGITS_MAG_UPTO_GT_0(2);
	case 1u:
		SET_RANGE_DIGITS_MAG_UPTO_GT_0(1);
	default:
		current_ptr = string_ptrs;
		SET_RANGE_DIGITS_MAG_UPTO(0);
	}
}


/* top-level functions
 *─────────────────────────────────────────────────────────────────────────── */
inline char **
count_string_pointers_create(const size_t upto)
{
	if (upto > UPTO_MAX) {
		count_string_log_alloc_failure(upto,
					       UPTO_MAX_EXCEEDED_FAILURE_MESSAGE);
		return NULL;
	}

	struct CountStringSpec spec;

	count_string_spec_init(&spec,
			       upto);

	/* 'upto' + 1 (NULL terminated) pointers + 'size_digits' ascii chars */
	char **const string_ptrs = malloc((sizeof(char *) * (upto + 1lu))
					  + spec.size_digits);
	if (string_ptrs == NULL) {
		count_string_log_alloc_failure(upto,
					       MALLOC_FAILURE_MESSAGE);
		return NULL;
	}

	count_string_pointers_init(string_ptrs,
				   spec.mag_upto,
				   upto);

	return string_ptrs;
}

inline char **
count_string_get(void)
{
	if (count_string.incomplete) {

		seed_mutex_handle_lock(&count_string.processing);

		seed_thread_cond_handle_await_span(&count_string.done,
						   &count_string.processing,
						   &count_string_await_span);

		seed_mutex_handle_unlock(&count_string.processing);
	}

	if (count_string.pointers == NULL)
		seed_supervisor_exit(CS_INIT_FAILURE_MESSAGE);

	return count_string.pointers;
}

void
count_string_do_init(void *arg);

inline void
count_string_init(const size_t upto)
{
	count_string.upto = upto;
	seed_worker_spawn_independent(&count_string_do_init,
				      NULL);
}

inline void
count_string_destroy(void)
{
	free(count_string.pointers);
}


#endif	/* MYSQL_SEED_GEN_GEN_COUNT_STRING_H_ */
