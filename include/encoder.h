#pragma once
#include "utils.h"

typedef struct Encoded_message {
    uint8_t bitarray[255];
    uint8_t length;
} Encoded_message_typedef;

void encoder_encode_msg(CAN_configs_typedef config, Encoded_message_typedef *encoded_message);