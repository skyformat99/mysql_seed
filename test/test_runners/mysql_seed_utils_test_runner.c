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
#include "mysql_seed_utils.h"
#include <math.h>

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_string_length(void);
extern void test_put_string(void);
extern void test_ten_pow_map(void);
extern void test_digit_count(void);
extern void test_put_number(void);


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
  UnityBegin("../test/mysql_seed_utils_test.c");
  RUN_TEST(test_string_length, 13);
  RUN_TEST(test_put_string, 27);
  RUN_TEST(test_ten_pow_map, 87);
  RUN_TEST(test_digit_count, 95);
  RUN_TEST(test_put_number, 103);

  return (UnityEnd());
}
