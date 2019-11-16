//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved
//-    For use by owners of Test-Driven Development for Embedded C,
//-    and attendees of Renaissance Software Consulting, Co. training
//-    classes.
//-
//-    Available at http://pragprog.com/titles/jgade/
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3
//-
//-    Authorized users may use this source code in your own
//-    projects, however the source code may not be used to
//-    create training material, courses, books, articles, and
//-    the like. We make no guarantees that this source code is
//-    fit for any purpose.
//-
//-    www.renaissancesoftware.net james@renaissancesoftware.net
//- ------------------------------------------------------------------


#include "CppUTest/TestHarness.h"

extern "C" {
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "utils.h"
}


// START: testGroup
TEST_GROUP(utils) {
    char output[100];
    const char* expected;
    void setup() {}
    void teardown() {}
    void expect(const char* s) {
        expected = s;
    }
    void given(int charsWritten) {
        LONGS_EQUAL(strlen(expected), charsWritten);
        STRCMP_EQUAL(expected, output);
    }
};
// END: testGroup

TEST(utils, BitArrayToStr) {
    char dst[255] = {'\0'};
    uint8_t str[] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1};
    bitarray_to_str(dst, str, sizeof str);

    STRCMP_EQUAL("100001000111100011", dst);
}
