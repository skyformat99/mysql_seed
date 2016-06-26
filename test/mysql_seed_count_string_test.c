#include "unity.h"
#include "mysql_seed_count_string.h"

void setUp(void)
{
}

void tearDown(void)
{
}

inline void
test_assert_spec_fields_equal(struct CountStringSpec *const restrict spec,
			      const unsigned int mag_upto_expected,
			      const size_t size_1_upto_expected)
{
	TEST_ASSERT_EQUAL_UINT_MESSAGE(mag_upto_expected,
				       spec->mag_upto,
				       "spec->mag_upto != mag_upto_expected");


	TEST_ASSERT_EQUAL_UINT_MESSAGE(size_1_upto_expected,
				       spec->size_1_upto,
				       "spec->size_1_upto != size_1_upto_expected");
}

void test_count_string_spec_init(void)
{
	struct CountStringSpec spec;

	count_string_spec_init(&spec,
			       3lu);

	test_assert_spec_fields_equal(&spec,
				      0u,
				      sizeof("1") + sizeof("2") + sizeof("3"));

	count_string_spec_init(&spec,
			       25000lu);

	test_assert_spec_fields_equal(&spec,
				      4u,
				      SIZE_MAG_0_4_STR + (15001lu * 6lu));
}

void test_count_string_alloc_failure(void)
{
	char failure[128];

	TEST_ASSERT_NULL(count_string_create(UPTO_MAX + 1lu));

	sprintf(&failure[0],
		CS_ALLOC_FAILURE_MESSAGE_BEGIN
		"%zu"
		CS_ALLOC_FAILURE_MESSAGE_MIDDLE
		UPTO_MAX_EXCEEDED_FAILURE_MESSAGE,
		UPTO_MAX + 1lu);

	TEST_ASSERT_EQUAL_STRING(&failure[0],
				 seed_log_buffer_ptr());

	seed_log_reset();
}

void test_count_string_increment_buffer(void)
{
	char digits[5] = "8998";

	count_buffer_increment(&digits[3]);

	TEST_ASSERT_EQUAL_STRING("8999",
				 &digits[0]);

	count_buffer_increment(&digits[3]);

	TEST_ASSERT_EQUAL_STRING("9000",
				 &digits[0]);
}

void test_count_string_init(void)
{
	char buffer[512];

	char *restrict ptr;

	ptr = &buffer[0];

	count_string_init(ptr,
			  0,
			  3);


	TEST_ASSERT_EQUAL_STRING("1", ptr);

	ptr += sizeof("1");

	TEST_ASSERT_EQUAL_STRING("2", ptr);

	ptr += sizeof("2");

	TEST_ASSERT_EQUAL_STRING("3", ptr);

	ptr += sizeof("3");

	TEST_ASSERT_EQUAL_STRING("",  ptr);
}




