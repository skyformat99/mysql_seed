#ifndef MYSQL_SEED_GENERATE_COUNTER_H_
#define MYSQL_SEED_GENERATE_COUNTER_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include "generate/generator.h"	/* UPTO_MAX, Counter, string/thread utils */


/* failure messages
 *─────────────────────────────────────────────────────────────────────────── */
#define COUNTER_CREATE_FAILURE_MESSAGE(REASON)				\
FAILURE_REASON("counter_create", REASON)

#define CC_UPTO_MAX_EXCEEDED_FAILURE_MESSAGE				\
COUNTER_CREATE_FAILURE_MESSAGE("'UPTO_MAX' of "				\
			       UPTO_MAX_STRING " exceeded\n")

#define CC_ALLOC_FAILURE_MESSAGE					\
COUNTER_CREATE_FAILURE_MESSAGE(MALLOC_FAILURE_REASON)

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
	char digits[SIZE_UPTO_MAX_STR];
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
	char *restrict digits;
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

/* misc helper functions
 *─────────────────────────────────────────────────────────────────────────── */
/* count of chars required for "1", "2", ... "upto" null-terminated ascii
 * strings */
inline void
counter_size_internals(struct Counter *const restrict counter)
{
#ifdef LARGE_UPTO_MAX
	if (counter->upto < MAG_4_MIN) {
#endif	/*  ifdef LARGE_UPTO_MAX */
		if (counter->upto < MAG_2_MIN) {
			if (counter->upto < MAG_1_MIN) {
				counter->mag_upto    = 0u;
				counter->size_digits = SIZE_MAG_0_STR
						     * counter->upto;
			} else {
				counter->mag_upto    = 1u;
				counter->size_digits = SIZE_MAG_0_1_STR
						     + (SIZE_MAG_1_STR
						        * (counter->upto
							   - MAG_0_MAX));
			}
		} else {
			if (counter->upto < MAG_3_MIN) {
				counter->mag_upto    = 2u;
				counter->size_digits = SIZE_MAG_0_2_STR
						     + (SIZE_MAG_2_STR
							* (counter->upto
							   - MAG_1_MAX));
			} else {
				counter->mag_upto    = 3u;
				counter->size_digits = SIZE_MAG_0_3_STR
						     + (SIZE_MAG_3_STR
							* (counter->upto
							   - MAG_2_MAX));
			}
		}
#ifdef LARGE_UPTO_MAX
	} else {
		if (counter->upto < MAG_6_MIN) {
			if (counter->upto < MAG_5_MIN) {
				counter->mag_upto    = 4u;
				counter->size_digits = SIZE_MAG_0_4_STR
						     + (SIZE_MAG_4_STR
							* (counter->upto
							   - MAG_3_MAX));
			} else {
				counter->mag_upto    = 5u;
				counter->size_digits = SIZE_MAG_0_5_STR
						     + (SIZE_MAG_5_STR
							* (counter->upto
							   - MAG_4_MAX));
			}
		} else {
			if (counter->upto < MAG_7_MIN) {
				counter->mag_upto    = 6u;
				counter->size_digits = SIZE_MAG_0_6_STR
						     + (SIZE_MAG_6_STR
							* (counter->upto
							   - MAG_5_MAX));
			} else {
				counter->mag_upto    = 7u;
				counter->size_digits = SIZE_MAG_0_7_STR
						     + (SIZE_MAG_7_STR
							* (counter->upto
							   - MAG_6_MAX));
			}
		}
	}
#endif	/*  ifdef LARGE_UPTO_MAX */
}


/* count of chars required for "1", "2", ... "upto" null-terminated ascii
 * strings */
inline size_t
counter_size_upto(const size_t upto)
{
#ifdef LARGE_UPTO_MAX
	if (upto < MAG_4_MIN) {
#endif	/*  ifdef LARGE_UPTO_MAX */
		if (upto < MAG_2_MIN) {
			return (upto < MAG_1_MIN)
			     ? (SIZE_MAG_0_STR * upto)
			     : (SIZE_MAG_0_1_STR + (SIZE_MAG_1_STR
						    * (upto - MAG_0_MAX)));
		} else {
			return (upto < MAG_3_MIN)
			     ? (SIZE_MAG_0_2_STR + (SIZE_MAG_2_STR
						    * (upto - MAG_1_MAX)))
			     : (SIZE_MAG_0_3_STR + (SIZE_MAG_3_STR
						    * (upto - MAG_2_MAX)));
		}
#ifdef LARGE_UPTO_MAX
	} else {
		if (upto < MAG_6_MIN) {
			return (upto < MAG_5_MIN)
			     ? (SIZE_MAG_0_4_STR + (SIZE_MAG_4_STR
						    * (upto - MAG_3_MAX)))
			     : (SIZE_MAG_0_5_STR + (SIZE_MAG_5_STR
						    * (upto - MAG_4_MAX)));
		} else {
			return (upto < MAG_7_MIN)
			     ? (SIZE_MAG_0_6_STR + (SIZE_MAG_6_STR
						    * (upto - MAG_5_MAX)))
			     : (SIZE_MAG_0_7_STR + (SIZE_MAG_7_STR
						    * (upto - MAG_6_MAX)));
		}
	}
#endif	/*  ifdef LARGE_UPTO_MAX */
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
	pointer.digits     = digits + SIZE_MAG_0_ ## MAG ## _STR;	\
	buffer.mag_ ## MAG = mag_ ## MAG ##_min_string;			\
	while (1) {							\
		*(pointer.mag_ ## MAG) = buffer.mag_ ## MAG;		\
		*current = pointer.digits;				\
		++current;						\
		if (current == until)					\
			break;						\
		++(pointer.mag_ ## MAG);				\
		count_buffer_increment(active);				\
	}								\
} while (0)

#define SET_RANGE_DIGITS_MAG_UPTO_GT_0(MAG)				\
do {									\
		from    = pointers + OFF_MAG_ ## MAG ## _MIN;		\
		current = from;						\
		SET_RANGE_DIGITS_MAG_UPTO(MAG);				\
		until   = from;						\
		--active;						\
} while (0)

inline void
counter_set_internals(struct Counter *const restrict counter)
{
	union DigitsBuffer buffer;
	union DigitsPointer pointer;
	char *restrict active;
	char *restrict *from;
	char *restrict *current;
	char *restrict *until;

	char *restrict *const restrict pointers = counter->pointers;

	/* point counter buffer 'digits' after 'upto' x counter pointers */
	until = pointers + counter->upto;

	char *const restrict digits = (char *) until;

	/* point last pointer past end of digits buffer */
	*until = digits + counter->size_digits;

	counter->digits = digits;

	/* point 'active' at one's digit */
	active = &buffer.digits[counter->mag_upto];


	switch (counter->mag_upto) {
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
		current = pointers;
		SET_RANGE_DIGITS_MAG_UPTO(0);
	}
}


inline void
counter_init_internals(struct Counter *const restrict counter)
{
	if (counter->upto > UPTO_MAX) {
		handler_closure_call(&counter->fail_cl,
				     CC_UPTO_MAX_EXCEEDED_FAILURE_MESSAGE);
		__builtin_unreachable();
	}

	counter_size_internals(counter);

	/* 'upto' pointers + 'size_digits' ascii chars */
	const size_t size_counter = (sizeof(char *) * counter->upto)
				  + counter->size_digits;

	thread_try_catch_open(free,
			      counter->pointers);

	counter->pointers = malloc(size_counter);

	if (counter->pointers == NULL) {
		handler_closure_call(&counter->fail_cl,
				     CC_ALLOC_FAILURE_MESSAGE);
		__builtin_unreachable();
	}

	counter_set_internals(counter);

	thread_try_catch_close();
}

void
counter_exit_on_failure(void *arg,
			const char *restrict failure)
__attribute__((noreturn));

/* top-level functions
 *─────────────────────────────────────────────────────────────────────────── */
inline void
counter_init(struct Counter *const restrict counter,
	     struct Generator *const restrict parent,
	     const size_t upto)
{
	mutex_init(&counter->processing);
	thread_cond_init(&counter->done);

	counter->ready = false;
	counter->upto  = upto;

	handler_closure_init(&counter->fail_cl,
			     &counter_exit_on_failure,
			     counter);
}

inline void
counter_free_internals(struct Counter *const restrict counter)
{
	free(counter->pointers);
}

void
build_counter(void *arg);

inline void
counter_await(struct Counter *const restrict counter,
	      const struct HandlerClosure *const restrict fail_cl)
{
	if (counter->ready)
		return;

	mutex_lock_try_catch_open(&counter->processing);

	mutex_lock_handle_cl(&counter->processing,
			     fail_cl);

	while (!counter->ready)
		thread_cond_await_handle_cl(&counter->done,
					    &counter->processing,
					    fail_cl);

	mutex_unlock_handle_cl(&counter->processing,
			       fail_cl);

	mutex_lock_try_catch_close();
}

#endif	/* ifndef MYSQL_SEED_GENERATE_COUNTER_H_ */
