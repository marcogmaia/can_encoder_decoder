#include <string.h>
#include <assert.h>
#include "encoder.h"

static uint32_t encoder_bit_stuffing_counter = 0;

void byte_to_binary(uint8_t *dst, uint8_t src) {
    for(uint8_t i = 0; i < 8; ++i) {
        dst[i] = src & (1 << i);
    }
}

static void reverse_array(uint8_t *dst, uint8_t *src, uint32_t size) {
    uint8_t r[256] = {0};
    for(uint32_t i = 0; i < size; ++i) {
        r[i] = src[size - i - 1];
    }
    memcpy(dst, src, size);
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

    /* TODO: add CRC */    

    /* TODO: fazer o bit stuffing aqui */

    /* A patir do CRC_delimiter não há mais bitstuffing*/

    /* TODO: add CRC_delimiter */
    /* TODO: add ACK_field */
    /* TODO: add ACK_delimiter */
    /* TODO: add EOF */

    *returned_msg_size = copy_binary_message_counter;
    return encoded_message;
}
