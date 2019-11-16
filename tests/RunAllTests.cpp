#include "CppUTest/CommandLineTestRunner.h"

IMPORT_TEST_GROUP(encoder);
IMPORT_TEST_GROUP(utils);

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}