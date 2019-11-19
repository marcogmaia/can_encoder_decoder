#include <CppUTest/TestHarness.h>

extern "C" {
#include <stdio.h>
#include <string.h>
#include "encoder.h"
#include "decoder.h"
}

TEST_GROUP(decoder) {
    CAN_configs_typedef config, test_config;
    CAN_message_typedef encoded_message;

    void setup() {}
    void teardown() {
        decoder_decode_msg(NULL, 1);
        decoder_decode_msg(NULL, 1);
        decoder_decode_msg(NULL, 1);
    }

    void test_configs(CAN_configs_typedef conf) {
        encoder_encode_msg(&conf, &encoded_message);

        CAN_configs_typedef decoded_configs;
        for(uint32_t i = 0; i < encoded_message.length; ++i) {
            decoder_decode_msg(&decoded_configs, encoded_message.bitarray[i]);
        }

        LONGS_EQUAL_TEXT(conf.StdId, decoded_configs.StdId, "StdId Failed");
        BYTES_EQUAL_TEXT(conf.RTR, decoded_configs.RTR, "RTR Failed");
        BYTES_EQUAL_TEXT(conf.IDE, decoded_configs.IDE, "IDE Failed");
        if(conf.IDE == 1) {
            LONGS_EQUAL_TEXT(conf.ExtId, decoded_configs.ExtId, "ExtId Failed");
        }
        if(conf.RTR == 0) {
            MEMCMP_EQUAL_TEXT(conf.data, decoded_configs.data, conf.DLC, "DATA Failed");
        }
        BYTES_EQUAL_TEXT(conf.DLC, decoded_configs.DLC, "DLC Failed");
        LONGS_EQUAL_TEXT(conf.CRC, decoded_configs.CRC, "CRC Failed");
    }
};

TEST(decoder, decode1) {
    test_config = {
        .StdId = 0x123,
        .ExtId = 0x5FF7,
        .IDE   = 1,
        .RTR   = 0,
    };
    static uint8_t data[] = {0xFF, 0xFF, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0x12};
    test_config.DLC       = sizeof data;
    test_config.data      = data;

    test_configs(test_config);
}

TEST(decoder, decode2) {
    CAN_configs_typedef test_config = {
        .StdId = 0x7FF,
        // .ExtId = 0xFF,
        .IDE = 0,
        .RTR = 0,
    };
    uint8_t data[]   = {0xFF, 0xFF, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0x12};
    test_config.DLC  = sizeof data;
    test_config.data = data;

    for(uint32_t i = 0; i < 0x7FF; ++i) {
        test_config.StdId = i;
        test_configs(test_config);
        teardown();
    }
}


TEST(decoder, decode3) {
    CAN_configs_typedef test_config = {
        .StdId = 0x7FF,
        .ExtId = 0x10FF,
        .IDE   = 1,
        .RTR   = 0,
    };
    uint8_t data[]   = {0xFF, 0xFF, 0xAA, 0xAA, 0xAA, 0xAA, 0xBB, 0x12};
    test_config.DLC  = sizeof data;
    test_config.data = data;

    for(uint32_t i = 0; i < 0x3FFF; ++i) {
        test_config.ExtId = i;
        test_configs(test_config);
        teardown();
    }

    for(uint32_t i = 0; i < 0x7FF; ++i) {
        test_config.StdId = i;
        test_configs(test_config);
        teardown();
    }
}
