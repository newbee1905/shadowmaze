#include "CUnit/CUnitCI.h"

static void test_simple_pass1(void) { CU_ASSERT_FATAL(1 == 2); }

CUNIT_CI_RUN(CU_MAIN_EXE_NAME, CUNIT_CI_TEST(test_simple_pass1));
