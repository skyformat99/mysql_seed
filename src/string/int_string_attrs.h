#ifndef MYSQL_SEED_STRING_INT_STRING_ATTRS_H_
#define MYSQL_SEED_STRING_INT_STRING_ATTRS_H_

/* external dependencies
 *─────────────────────────────────────────────────────────────────────────── */
#include <limits.h>

/* constants
 *─────────────────────────────────────────────────────────────────────────── */
/* UINT8_MAX  = 255			(3  digits) */
/* UINT16_MAX = 65535			(5  digits) */
/* UINT32_MAX = 4294967295		(10 digits) */
/* UINT64_MAX = 18446744073709551615	(20 digits) */

#if   (UINTMAX_MAX == UINT64_MAX)
#	define HAVE_INT_STRING_ATTRS	1
#	define DIGIT_COUNT_UINTMAX_MAX	20u
#	define DIGIT_COUNT_INTMAX_MAX	19u
#	define DIGIT_COUNT_INTMAX_MIN	19u
#	define DIGIT_STRING_UINTMAX_MAX	"18446744073709551615"
#	define DIGIT_STRING_INTMAX_MAX	"9223372036854775807"
#	define DIGIT_STRING_INTMAX_MIN	"9223372036854775808"
#elif (UINTMAX_MAX == UINT32_MAX)
#	define HAVE_INT_STRING_ATTRS	1
#	define DIGIT_COUNT_UINTMAX_MAX	10u
#	define DIGIT_COUNT_INTMAX_MAX	10u
#	define DIGIT_COUNT_INTMAX_MIN	10u
#	define DIGIT_STRING_UINTMAX_MAX	"4294967295"
#	define DIGIT_STRING_INTMAX_MAX	"2147483647"
#	define DIGIT_STRING_INTMAX_MIN	"2147483648"
#elif (UINTMAX_MAX == UINT16_MAX)
#	define HAVE_INT_STRING_ATTRS	1
#	define DIGIT_COUNT_UINTMAX_MAX	5u
#	define DIGIT_COUNT_INTMAX_MAX	5u
#	define DIGIT_COUNT_INTMAX_MIN	5u
#	define DIGIT_STRING_UINTMAX_MAX	"65535"
#	define DIGIT_STRING_INTMAX_MAX	"32767"
#	define DIGIT_STRING_INTMAX_MIN	"32768"
#elif (UINTMAX_MAX == UINT8_MAX)
#	define HAVE_INT_STRING_ATTRS	1
#	define DIGIT_COUNT_UINTMAX_MAX	3u
#	define DIGIT_COUNT_INTMAX_MAX	3u
#	define DIGIT_COUNT_INTMAX_MIN	3u
#	define DIGIT_STRING_UINTMAX_MAX	"255"
#	define DIGIT_STRING_INTMAX_MAX	"127"
#	define DIGIT_STRING_INTMAX_MIN	"128"
#else
#	define HAVE_INT_STRING_ATTRS	0
#	undef  DIGIT_COUNT_UINTMAX_MAX
#	undef  DIGIT_COUNT_INTMAX_MAX
#	undef  DIGIT_COUNT_INTMAX_MIN
#	undef  DIGIT_STRING_UINTMAX_MAX
#	undef  DIGIT_STRING_INTMAX_MAX
#	undef  DIGIT_STRING_INTMAX_MIN
#	include <stdio.h>	/* sprintf, snprintf */
#	include <inttypes.h>	/* stroumax */
#	include <errno.h>	/* errno (checking return of stroumax) */
#endif	/* if (UINTMAX_MAX == UINT64_MAX) */

#endif /* ifndef MYSQL_SEED_STRING_INT_STRING_ATTRS_H_ */
