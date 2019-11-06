#include <string.h>
#include <assert.h>
#include "decoder.h"


static uint32_t bit_unstuffing(uint8_t *dst, uint8_t *bitarray, uint32_t size) {
    assert(size > 11);
    uint8_t buffer[256]      = {0};
    uint8_t *bptr            = buffer;
    uint8_t lastbit          = 0xFF; /* magic number */
    int32_t counter          = 0;
    uint32_t unstuff_counter = 0;
    /* os ultimos 10 bits n tem bit stuffing */
    for(uint32_t i = 0; i < size - 10; ++i) {
        uint8_t bit = bitarray[i];
        if(bit == lastbit) {
            ++counter;
            if(counter == 5) {
                *bptr = bit;
                ++bptr;
                ++i; /* skip bit */
                ++unstuff_counter;
                lastbit = 0xFF;
                continue;
            }
        }
        else {
            lastbit = bit;
            counter = 1;
        }
        *bptr = bit;
        ++bptr;
    }
    uint32_t newsize = size - unstuff_counter;
    memcpy(dst, buffer, newsize);
    memcpy(dst + newsize - 10, bitarray + size - 10, 10);
    return size - unstuff_counter;
}


static uint32_t decoded_message_index = 0;
/* alway zero the decoded_message_index (decoded_message_index = 0)
 * before calling binary_to_numbedecoded_message_indexr
 */
void static binary_to_number(uint32_t *num, uint8_t *bitarr, uint32_t size) {
    *num = 0;
    for(uint32_t i = 0; i < size; ++i) {
        *num |= bitarr[i + decoded_message_index] << (size - 1 - i);
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

    *size = bit_unstuffing(decoded_message, encoded_message, *size);

    return decoded_message;
}
