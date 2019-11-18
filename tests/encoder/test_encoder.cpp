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
    char actual[256];
    const char* expected;
    void setup() {
        memset(actual, 0, sizeof actual);
    }
    void teardown() {}
    void expect(const char* s) {}
    void given(int charsWritten) {
        LONGS_EQUAL(strlen(expected), charsWritten);
        // STRCMP_EQUAL(expected, output);
        // BYTES_EQUAL(0xaa, output[strlen(expected) + 1]);
    }
};
// END: testGroup


TEST(encoder, encode1) {
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
    CAN_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);
    // char actual[256] = {0};
    bitarray_to_str(actual, encoded_message.bitarray, encoded_message.length);
    STRCMP_EQUAL(
        "011001110010000011111001010101010101010101010101010101010101010101010101010101011001100111011111111111",
        actual);
    // LONGS_EQUAL(93, encoded_message.length);
}

TEST(encoder, encode2) {
    CAN_configs_typedef config = {
        .StdId = 0x7b,
        // .ExtId = 0x3007A,
        .IDE = 0,
        .RTR = 1,
        .DLC = 0,
    };

    uint8_t data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA};
    // config.DLC     = 15;
    config.data = data;

    // uint32_t returned_msg_size;
    CAN_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);
    // char actual[256] = {0};
    bitarray_to_str(actual, encoded_message.bitarray, encoded_message.length);
    // 0 0000111110011
    STRCMP_EQUAL("00000111110011100000101101000100101011111111111", actual);
    // LONGS_EQUAL(93, encoded_message.length);
}

TEST(encoder, encode3) {
    CAN_configs_typedef config = {
        .StdId = 0x0672,
        // .ExtId = 0xFF,
        .IDE = 0,
        .RTR = 0,
    };
    uint8_t data[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    config.DLC     = sizeof data;
    config.data    = data;

    CAN_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);
    // char actual[256] = {0};
    bitarray_to_str(actual, encoded_message.bitarray, encoded_message.length);
    STRCMP_EQUAL(
        "0110011100100001000101010101010101010101010101010101010101010101010101010101010101000001000010100011111111111",
        actual);
}


/*
 (14)ID_A - 0x0449 srr = 1 ide =1 id_b =0x3007A (stuff)  rtr =1 dlc = 0  data = vazio CRC = 111101111110101 (ACK=ok)(até
 EOF) */
TEST(encoder, encode4) {
    CAN_configs_typedef config = {
        .StdId = 0x0449,
        .ExtId = 0x3007A,
        .IDE   = 1,
        .RTR   = 1,
        .DLC   = 0,
    };

    CAN_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);
    // char actual[256] = {0};
    bitarray_to_str(actual, encoded_message.bitarray, encoded_message.length);
    STRCMP_EQUAL("0100010010011111000001000001111100101000001010111011111000011111111111", actual);
}

TEST(encoder, encode5) {
    CAN_configs_typedef config = {
        .StdId = 0x0449,
        .ExtId = 0x3007A,
        .IDE   = 1,
        .RTR   = 0,
        .DLC   = 0,
    };
    uint8_t data[] = {0xAA, 0xAA, 0xAA};
    config.data    = data;
    config.DLC     = sizeof data;

    CAN_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);
    // char actual[256] = {0};
    bitarray_to_str(actual, encoded_message.bitarray, encoded_message.length);
    STRCMP_EQUAL("0100010010011111000001000001111100100000101110101010101010101010101000011111001011101111111111",
                 actual);
}

/*
(13)ID_A - 0x0449 srr = 1 ide =1 id_b =0x3007A (stuff)  rtr =1 dlc = 8  data = vazio CRC = 111101111110101 (ACK=ok)(até
EOF) */
TEST(encoder, encode6) {
    CAN_configs_typedef config = {
        .StdId = 0x0449,
        .ExtId = 0x3007A,
        .IDE   = 1,
        .RTR   = 1,
        .DLC   = 8,
    };

    CAN_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);
    // char actual[256] = {0};
    bitarray_to_str(actual, encoded_message.bitarray, encoded_message.length);
    STRCMP_EQUAL("010001001001111100000100000111110010100100001010011111001101111111111", actual);
}