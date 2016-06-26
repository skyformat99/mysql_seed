/* AUTOGENERATED FILE. DO NOT EDIT. */

//=======Test Runner Used To Run Each Test Below=====
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT() && !TEST_IS_IGNORED) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

//=======Automagically Detected Files To Include=====
#include <unity/unity.h>
#include <setjmp.h>
#include <stdio.h>
#include "mysql_seed_count_string.h"

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_count_string_spec_init(void);
extern void test_count_string_alloc_failure(void);
extern void test_count_string_increment_buffer(void);
extern void test_count_string_init(void);
extern void test_count_string_create(void);


//=======Test Reset Option=====
void resetTest(void);
void resetTest(void)
{
  tearDown();
  setUp();
}


//=======MAIN=====
int main(void)
{
  UnityBegin("../test/mysql_seed_count_string_test.c");
  RUN_TEST(test_count_string_spec_init, 27);
  RUN_TEST(test_count_string_alloc_failure, 46);
  RUN_TEST(test_count_string_increment_buffer, 65);
  RUN_TEST(test_count_string_init, 80);
  RUN_TEST(test_count_string_create, 97);

  return (UnityEnd());
}
