#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "decoder.h"

/**
 * https://www.kvaser.com/about-can/the-can-protocol/can-error-handling/
 * 
 * os tipos de erros que tem são 5:
 * 1- Bit Monitoring. (manda um bit, recebe outro)
 * 2- Bit Stuffing. (não foi feito o bit sutffing corretamente, preciso ter só um bit pra dar "assert"
 *      no sexto bit pra ver se é oposto aos 5 anteriores)
 * 3- Frame Check. ()
 * 4- Acknowledgement Check.
 * 5- Cyclic Redundancy Check.
 */

static uint32_t bitarray_to_int(uint8_t *bitarr, uint8_t size) {
    // *num = 0;
    uint32_t num = 0;
    for(uint32_t i = 0; i < size; ++i) {
        num |= bitarr[i] << (size - 1 - i);
    }
    return num;
}

/* remover o sexto bit consecutivo igual */
static void decoder_bit_destuff(uint8_t sample_bit, CAN_message_typedef *destuffed_bitarr) {
    // static uint8_t current_bit = 0xFF;
    static uint8_t bufsize        = 0;
    static uint8_t buffer[256]    = {0};
    static uint8_t last_bit       = 0xFF;
    static uint8_t curr_bit_count = 0;


    if(curr_bit_count == 5) {
        curr_bit_count = 0;
        last_bit       = 0xFF;
        return;
    }

    buffer[bufsize] = sample_bit;
    ++bufsize;

    /* detect EOF */
    {
        static uint8_t eof_cnt = 0;
        if(sample_bit == 1)
            ++eof_cnt;
        else
            eof_cnt = 0;
        if(eof_cnt == 7) {
            buffer[bufsize]     = 1;
            buffer[bufsize + 1] = 1;
            buffer[bufsize + 2] = 1;
            bufsize += 3;
            memcpy(destuffed_bitarr->bitarray, buffer, bufsize);
            destuffed_bitarr->length = bufsize;
            return;
        }
    }


    if(last_bit != sample_bit) {
        curr_bit_count = 1;
        last_bit       = sample_bit;
    }
    else if(curr_bit_count < 5) {
        /* last_bit == sample_bit */
        ++curr_bit_count;
    }
}


static uint32_t decoded_message_index = 0;
/* alway zero the decoded_message_index (decoded_message_index = 0)
 * before calling binary_to_numbedecoded_message_indexr
 */
void static binary_to_number(void *num, uint8_t *bitarr, uint32_t size) {
    *(uint32_t *)num = 0;
    for(uint32_t i = 0; i < size; ++i) {
        *(uint32_t *)num |= bitarr[i + decoded_message_index] << (size - 1 - i);
    }
    decoded_message_index += size;
}

void decoder_decoded_message_to_configs(CAN_configs_typedef *configs_dst, uint8_t *decoded_message) {
    decoded_message_index = 1; /* skip SoF */
    binary_to_number(&configs_dst->StdId, decoded_message, 11);
    binary_to_number(&configs_dst->RTR, decoded_message, 1);
    binary_to_number(&configs_dst->IDE, decoded_message, 1);
    binary_to_number(&configs_dst->r0, decoded_message, 1);

    if(configs_dst->IDE == 0) {
        /* Std frame */
        binary_to_number(&configs_dst->DLC, decoded_message, 4);
        // memcpy(&configs_dst->data, decoded_message + 19, configs_dst->DLC);
        decoded_message_index += configs_dst->DLC * 8;
        binary_to_number(&configs_dst->CRC, decoded_message, 15);
    }
    else {
        /* extended frame */
        // memcpy(&configs_dst->SRR, decoded_message + 12, 1);
        // memcpy(&configs_dst->ExtId, decoded_message + 14, 18);
        // memcpy(&configs_dst->RTR, decoded_message + 32, 1);
        // memcpy(&configs_dst->r0, decoded_message + 33, 1);
        // memcpy(&configs_dst->r1, decoded_message + 34, 1);
        // memcpy(&configs_dst->DLC, decoded_message + 35, 4);
        // memcpy(&configs_dst->data, decoded_message + 36, configs_dst->DLC * 8);
        // memcpy(&configs_dst->CRC, decoded_message + 36 + configs_dst->DLC * 8, 15);
    }
}

uint8_t *decoder_decode_msg(uint8_t *encoded_message, uint32_t *size) {
    static uint8_t decoded_message[256];


    // *size = bit_unstuffing(decoded_message, encoded_message, *size);

    return decoded_message;
}
