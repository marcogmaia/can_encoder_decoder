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
#include "encoder.h"
}

#include <algorithm>

// START: testGroup
TEST_GROUP(encoder) {
    char output[100];
    const char* expected;
    void setup() {
        memset(output, 0xaa, sizeof output);
        expected = "";
    }
    void teardown() {}
    void expect(const char* s) {
    }
    void given(int charsWritten) {
        LONGS_EQUAL(strlen(expected), charsWritten);
        STRCMP_EQUAL(expected, output);
        BYTES_EQUAL(0xaa, output[strlen(expected) + 1]);
    }
};
// END: testGroup


TEST(encoder, EncodeMsg) {
    CAN_configs_typedef config = {
        .StdId = 0x0672,
        .IDE   = 0,
        .RTR   = 0,
        .DLC   = 6,
    };
    uint8_t data[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    config.DLC     = sizeof data;
    config.data    = data;

    // uint32_t returned_msg_size;
    Encoded_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);
    char actual[256];
    bitarray_to_str(actual, encoded_message.bitarray, encoded_message.length);
    STRCMP_EQUAL("011001110010000011111001010101010101010101010101010101010101010101010101010101011001100111011111111111",
                 actual);
    // LONGS_EQUAL(93, encoded_message.length);
}
// END: RefactoredTests