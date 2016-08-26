#include "unity.h"
#include "random/random.h"
#include <string.h>

__typeof__(glob_rng32) unset_glob_rng32;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_random_uint32_from(void)
{
	const char *restrict failure;
	uint32_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_32_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_uint32_from(UINT32_MAX - 100u);

		TEST_ASSERT_TRUE(random >= (UINT32_MAX - 100u));

		if (random < (UINT32_MAX - 50u)) {
			if (random < (UINT32_MAX - 75u))
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < (UINT32_MAX - 25u))
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_uint32_from:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}

void test_random_int32_from(void)
{
	const char *restrict failure;
	uint32_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_32_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_int32_from(INT32_MAX - 100);

		TEST_ASSERT_TRUE(random >= (INT32_MAX - 100));

		if (random < (INT32_MAX - 50)) {
			if (random < (INT32_MAX - 75))
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < (INT32_MAX - 25))
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_int32_from:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}

void test_random_uint64_from(void)
{
	const char *restrict failure;
	uint64_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_64_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_uint64_from(UINT64_MAX - 100u);

		TEST_ASSERT_TRUE(random >= (UINT64_MAX - 100u));

		if (random < (UINT64_MAX - 50u)) {
			if (random < (UINT64_MAX - 75u))
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < (UINT64_MAX - 25u))
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_uint64_from:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}

void test_random_int64_from(void)
{
	const char *restrict failure;
	uint64_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_64_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_int64_from(INT64_MAX - 100);

		TEST_ASSERT_TRUE(random >= (INT64_MAX - 100));

		if (random < (INT64_MAX - 50)) {
			if (random < (INT64_MAX - 75))
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < (INT64_MAX - 25))
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_int64_from:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}


void test_random_uint32_upto(void)
{
	const char *restrict failure;
	uint32_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_32_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_uint32_upto(100u);

		TEST_ASSERT_TRUE(random <= 100u);

		if (random < 50u) {
			if (random < 25u)
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < 75u)
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_uint32_upto:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}

void test_random_int32_upto(void)
{
	const char *restrict failure;
	int32_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_32_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_int32_upto(INT32_MIN + 100);

		TEST_ASSERT_TRUE(random <= (INT32_MIN + 100));

		if (random < (INT32_MIN + 50)) {
			if (random < (INT32_MIN + 25))
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < (INT32_MIN + 75))
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_int32_upto:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}

void test_random_uint64_upto(void)
{
	const char *restrict failure;
	uint64_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_64_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_uint64_upto(10000000000u);

		TEST_ASSERT_TRUE(random <= 10000000000u);

		if (random < 5000000000u) {
			if (random < 2500000000u)
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < 7500000000u)
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_uint64_upto:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}

void test_random_int64_upto(void)
{
	const char *restrict failure;
	int64_t random;

	unsigned int count_q0 = 0u;
	unsigned int count_q1 = 0u;
	unsigned int count_q2 = 0u;
	unsigned int count_q3 = 0u;

	TEST_ASSERT_TRUE(random_64_constructor(&failure));

	for (int i = 0; i < 10000; ++i) {

		random = random_int64_upto(INT64_MIN + 100);

		TEST_ASSERT_TRUE(random <= (INT64_MIN + 100));

		if (random < (INT64_MIN + 50)) {
			if (random < (INT64_MIN + 25))
				++count_q0;
			else
				++count_q1;
		} else {
			if (random < (INT64_MIN + 75))
				++count_q2;
			else
				++count_q3;
		}
	}

	printf("\nrandom_int64_upto:\tq0: %4u\tq1: %4u\tq2: %4u\tq3: %4u\n\n",
	       count_q0, count_q1, count_q2, count_q3);
}
