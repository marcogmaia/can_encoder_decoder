#include <CppUTest/TestHarness.h>

extern "C" {
#include <stdio.h>
#include <string.h>
#include "encoder.h"
#include "decoder.h"
}

TEST_GROUP(decoder) {
    CAN_configs_typedef configs;

    uint8_t destuffed_bitarr[256] = {0};
    CAN_message_typedef msg_encoded, msg_decoded;
    void setup() {}
    void teardown() {}

    void test_configs(const CAN_configs_typedef conf) {
        encoder_encode_msg(conf, &msg_encoded);
        // for(uint32_t i = 0; i < msg_encoded.length; ++i) {
        //     decoder_bit_destuff(msg_encoded.bitarray[i], &msg_decoded);
        // }

        char strbuf[256] = {0};
        bitarray_to_str(strbuf, msg_decoded.bitarray, msg_decoded.length);

        STRCMP_EQUAL(
            "011111111111000100010101010101010101010101010101010101010101010101010101010101010100000000010100011111111111",
            strbuf);

        // LONGS_EQUAL_TEXT(configs.StdId, decoded_configs.StdId, "StdId Failed");
        // BYTES_EQUAL_TEXT(configs.RTR, decoded_configs.RTR, "RTR Failed");
        // BYTES_EQUAL_TEXT(configs.IDE, decoded_configs.IDE, "IDE Failed");
        // BYTES_EQUAL_TEXT(configs.DLC, decoded_configs.DLC, "DLC Failed");
    }
};

TEST(decoder, decode1) {
    configs = {
        .StdId = 0x7FF,
        // .ExtId = 0xFF,
        .IDE = 0,
        .RTR = 0,
    };
    uint8_t data[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    configs.DLC    = sizeof data;
    configs.data   = data;

    test_configs(configs);

    // configs.StdId = 0x0671;
    // test_configs(configs);
}
