#include <string.h>
#include <assert.h>
#include "encoder.h"

static uint32_t encoder_bit_stuffing_counter = 0;

// void byte_to_binary(uint8_t *dst, uint8_t src) {
//     for(uint8_t i = 0; i < 8; ++i) {
//         dst[i] = src & (1 << i);
//     }
// }

// static void reverse_array(uint8_t *dst, uint8_t *src, uint32_t size) {
//     uint8_t r[256] = {0};
//     for(uint32_t i = 0; i < size; ++i) {
//         r[i] = src[size - i - 1];
//     }
//     memcpy(dst, src, size);
// }

uint32_t crc15(uint8_t *bitarray, uint32_t size) {
    // uint8_t res[15] = {0};
    uint8_t crc[15] = {0};

    for(uint32_t i = 0; i < size; ++i) {
        uint8_t invert = bitarray[i] ^ crc[14];
        crc[14]        = crc[13] ^ invert;
        crc[13]        = crc[12];
        crc[12]        = crc[11];
        crc[11]        = crc[10];
        crc[10]        = crc[9] ^ invert;
        crc[9]         = crc[8];
        crc[8]         = crc[7] ^ invert;
        crc[7]         = crc[6] ^ invert;
        crc[6]         = crc[5];
        crc[5]         = crc[4];
        crc[4]         = crc[3] ^ invert;
        crc[3]         = crc[2] ^ invert;
        crc[2]         = crc[1];
        crc[1]         = crc[0];
        crc[0]         = invert;
    }
    uint32_t res = 0x0;
    for(uint32_t i = 0; i < 15; ++i) {
        res |= crc[i] << i;
    }
    return res;
}

/* @return number of stuffed bits */
static uint32_t bit_stuffing(uint8_t *bitarray, uint32_t size) {
    /* a cada 5 bits iguais é adicionado um sexto com valor invertido */
    uint32_t counter              = 1;
    uint8_t buffer[256]           = {bitarray[0]};
    uint8_t lastbit               = bitarray[0];
    uint32_t stuffed_bits_counter = 0;
    for(uint32_t i = 1, buffer_index = 1; i < size + stuffed_bits_counter; ++i, ++buffer_index) {
        buffer[buffer_index] = bitarray[i];
        if(buffer[buffer_index] == lastbit) {
            ++counter;
        }
        else {
            counter = 1;
            lastbit = buffer[buffer_index];
        }

        if(counter == 5) {
            /* houve bit stuffing */
            ++stuffed_bits_counter;
            ++buffer_index;
            buffer[buffer_index] = (lastbit == 1) ? 0 : 1;
            counter              = 1;
            lastbit              = buffer[buffer_index];
        }
    }
    memcpy(bitarray, buffer, size + stuffed_bits_counter);
    return stuffed_bits_counter;
}


static uint32_t copy_binary_message_counter = 0;
/* always clear the counter (copy_binary_message_counter = 0) before calling
 * copy_binary for the first time */
static uint32_t copy_binary(uint8_t *dst, uint32_t src, uint32_t size) {
    assert(size < 250);
    uint8_t buffer[256] = {0};
    for(uint32_t i = 0; i < size; ++i) {
        buffer[i] = ((src & (1 << (size - 1 - i))) ? 1 : 0);
    }
    memcpy(dst + copy_binary_message_counter, buffer, size);
    copy_binary_message_counter += size;
    return size;
}

uint8_t *encoder_encode_msg(encoder_configs_typedef config, uint32_t *returned_msg_size) {
    static uint8_t encoded_message[256];
    memset(encoded_message, 0, sizeof encoded_message);
    copy_binary_message_counter = 0;

    if(config.DLC > 8) config.DLC = 8;

    copy_binary(encoded_message, 0, 1); /* SoF */
    copy_binary(encoded_message, config.StdId, 11);

    if(config.IDE == 0) {
        /* frame Standard */
        copy_binary(encoded_message, config.RTR, 1);
        copy_binary(encoded_message, config.IDE, 1);
        copy_binary(encoded_message, 0, 1); /* r0 */
        copy_binary(encoded_message, config.DLC, 4);
        if(config.RTR == 0) {
            /* DATA frame */
            for(uint32_t i = 0; i < config.DLC; ++i) {
                copy_binary(encoded_message, config.data[i], 8);
            }
        }
        else {
            /* REMOTE frame */
            /* não preciso fazer mais nada, pois remote não tem payload */
        }
    }
    else {
        /* IDE == 1, frame estendido */
        copy_binary(encoded_message, 1, 1); /* SRR must be recessive (1) */
        copy_binary(encoded_message, config.IDE, 1);
        copy_binary(encoded_message, config.ExtId, 18);
        copy_binary(encoded_message, config.RTR, 1);
        copy_binary(encoded_message, 0, 1); /* r1 */
        copy_binary(encoded_message, 0, 1); /* r0 */
        copy_binary(encoded_message, config.DLC, 4);
        if(config.RTR == 0) {
            /* DATA frame */
            for(uint32_t i = 0; i < config.DLC; ++i) {
                copy_binary(encoded_message, config.data[i], 8);
            }
        }
        else {
            /* REMOTE frame */
        }
    }

    /* agora já tenho todos os dados, falta calcular o CRC */

    /* add CRC */
    uint32_t crc = crc15(encoded_message, copy_binary_message_counter);
    copy_binary(encoded_message, crc, 15);

    /* bit stuffing aqui */
    int32_t num_of_stuffed_bits = bit_stuffing(encoded_message, copy_binary_message_counter);

    /* A patir do CRC_delimiter não há mais bitstuffing*/

    /* TODO: add CRC_delimiter */
    /* TODO: add ACK_field */
    /* TODO: add ACK_delimiter */
    /* TODO: add EOF */

    *returned_msg_size = copy_binary_message_counter + num_of_stuffed_bits;
    return encoded_message;
}
