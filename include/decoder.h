#pragma once

#include "utils.h"
#include "candefs.h"


/**
 * @param p_config_dst pointer to struct where the decoded config will be saved
 * @param p_encoded_message pointer to encoded message, which will be decoded
 */
void decoder_decoded_message_to_configs(CAN_configs_typedef *p_configs_dst, uint8_t *p_decoded_message);
// void decoder_bit_destuff(uint8_t sample_bit, CAN_message_typedef *destuffed_bitarr);
uint8_t *decoder_decode_msg(uint8_t *encoded_message, uint32_t *size);