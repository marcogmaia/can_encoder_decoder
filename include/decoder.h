#pragma once

#include "utils.h"
uint8_t *decoder_decode_msg(uint8_t *encoded_message, uint32_t *size);
void decoder_decoded_message_to_configs(CAN_configs_typedef *configs_dst, uint8_t *decoded_message);