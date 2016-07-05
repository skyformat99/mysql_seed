#include "string/string_utils.h"

#if HAVE_INT_STRING_ATTRS
const uintmax_t ten_pow_map[DIGIT_COUNT_UINTMAX_MAX] = {
	[ 0] = 1llu,
	[ 1] = 10llu,
	[ 2] = 100llu
#	if (DIGIT_COUNT_UINTMAX_MAX > 3u)
		     ,
	[ 3] = 1000llu,
	[ 4] = 10000llu
#		if (DIGIT_COUNT_UINTMAX_MAX > 5u)
		      ,
	[ 5] = 100000llu,
	[ 6] = 1000000llu,
	[ 7] = 10000000llu,
	[ 8] = 100000000llu,
	[ 9] = 1000000000llu
#			if (DIGIT_COUNT_UINTMAX_MAX > 10u)
			   ,
	[10] = 10000000000llu,
	[11] = 100000000000llu,
	[12] = 1000000000000llu,
	[13] = 10000000000000llu,
	[14] = 100000000000000llu,
	[15] = 1000000000000000llu,
	[16] = 10000000000000000llu,
	[17] = 100000000000000000llu,
	[18] = 1000000000000000000llu,
	[19] = 10000000000000000000llu
#			endif /* if (DIGIT_COUNT_UINTMAX_MAX > 10u) */
#		endif /* if (DIGIT_COUNT_UINTMAX_MAX > 5u) */
#	endif /* if (DIGIT_COUNT_UINTMAX_MAX > 2u) */
};

extern inline unsigned int
uint_digit_count(uintmax_t n);

extern inline void
do_put_uint(char *restrict buffer,
	    uintmax_t n);
#endif	/* if HAVE_INT_STRING_ATTRS */


#if HAVE_PTR_STRING_ATTRS
const uintptr_t ninety_five_pow_map[LENGTH_MAX_POINTER_ID] = {
	[0] = 1lu,
	[1] = 95lu
#	if (LENGTH_MAX_POINTER_ID > 2u)
		  ,
	[2] = 9025lu
#		if (LENGTH_MAX_POINTER_ID > 3u)
		    ,
	[3] = 857375lu,
	[4] = 81450625lu
#			if (LENGTH_MAX_POINTER_ID > 5u)
			,
	[5] = 7737809375lu,
	[6] = 735091890625lu,
	[7] = 69833729609375lu,
	[8] = 6634204312890625lu,
	[9] = 630249409724609375lu
#			endif /* if (LENGTH_MAX_POINTER_ID > 5u) */
#		endif /* if (LENGTH_MAX_POINTER_ID > 3u) */
#	endif /* if (LENGTH_MAX_POINTER_ID > 2u) */
};

extern inline unsigned int
pointer_id_length(const uintptr_t ptr_n);

extern inline void
#else
extern inline char *
#endif /* if HAVE_PTR_STRING_ATTRS */
do_put_pointer_id(char *restrict buffer,
		  uintptr_t ptr_n);

extern inline char *
put_uint(char *restrict buffer,
	 uintmax_t n);

extern inline char *
put_uint_length(char *restrict buffer,
		uintmax_t n,
		const size_t length);

extern inline char *
put_uint_until(char *restrict buffer,
	       const uintmax_t n,
	       char *const restrict until_ptr);

extern inline char *
put_int(char *restrict buffer,
	intmax_t n);

extern inline char *
put_int_length(char *restrict buffer,
	       intmax_t n,
	       unsigned int length);

extern inline char *
put_int_until(char *restrict buffer,
	      const intmax_t n,
	      char *const restrict until_ptr);

extern inline char *
put_pointer_id(char *restrict buffer,
	       void *const restrict pointer);

extern inline char *
put_pointer_id_length(char *restrict buffer,
		      void *const restrict pointer,
		      const unsigned int length);

extern inline char *
put_pointer_id_until(char *restrict buffer,
		     void *const restrict pointer,
		     char *const restrict until_ptr);

extern inline char *
put_string(char *restrict buffer,
	   const char *restrict string);

extern inline char *
put_string_length(char *restrict buffer,
		  const char *restrict string,
		  size_t length);

extern inline char *
put_string_until(char *restrict buffer,
		 const char *restrict string,
		 char *const restrict until_ptr);

extern inline int
string_compare(const char *restrict string1,
	       const char *restrict string2);

extern inline bool
strings_equal(const char *restrict string1,
	      const char *restrict string2);

extern inline size_t
string_length(const char *const restrict string);

extern inline ssize_t
string_length_limit(const char *const restrict string,
		    ssize_t limit);

extern inline size_t
string_size(const char *const restrict string);

extern inline size_t
string_size_limit(const char *const restrict string,
		  size_t limit);

extern inline bool
parse_uint(uintmax_t *const restrict n,
	   const char *restrict string);

extern inline bool
parse_int(uintmax_t *const restrict n,
	  const char *restrict string);
