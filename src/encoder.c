#include <string.h>
#include <assert.h>
#include "utils.h"
#include "encoder.h"

#include <stdio.h>
/*
 * 1. Start
 * 2. Initialize the array for transmitted stream with the special bit pattern 0111 1110
 * which indicates the beginning of the frame.
 * 3. Get the bit stream to be transmitted in to the array.
 * 4. Check for five consecutive ones and if they occur, stuff a bit 0
 * 5. Display the data transmitted as it appears on the data line after appending 0111 1110
 * at the end
 * 6. For de−stuffing, copy the transmitted data to another array after detecting the stuffed
 * bits
 * 7. Display the received bit stream
 * 8. Stop
 */

// static uint32_t copy_binary_message_counter = 0;

/* @return number of stuffed bits */
// static uint32_t bit_stuffing(uint8_t *dst, uint8_t *bitarray, uint32_t size) {
//     /* a cada 5 bits iguais é adicionado um sexto com valor invertido */
//     uint32_t counter              = 0;
//     uint8_t buffer[256]           = {0};
//     uint8_t lastbit               = 0xFF; /* magic number */
//     uint32_t stuffed_bits_counter = 0;
//     uint32_t lastidx              = 0;
//     for(uint32_t i = 0; i < size; ++i) {
//         uint8_t bit                      = bitarray[i];
//         buffer[i + stuffed_bits_counter] = bit;
//         if(lastbit == bit) {
//             lastbit = bit;
//             ++counter;
//             if(counter == 5) {
//                 counter = 1;
//                 lastbit = !lastbit;
//                 ++stuffed_bits_counter;
//                 buffer[i + stuffed_bits_counter] = lastbit;
//             }
//         }
//         else {
//             lastbit = bit;
//             counter = 1;
//         }
//     }
//     // copy_binary_message_counter += stuffed_bits_counter;
//     memcpy(bitarray, buffer, size + stuffed_bits_counter);
//     return stuffed_bits_counter;
// }

/**
 * @param dst:   vector address for data to be saved
 * @param begin:   source of data to be stuffed based
 * @param end:   end address of section to be stuffed
 */
uint8_t bit_stuff(uint8_t *dst, uint8_t *begin, uint8_t *end) {
    uint8_t buffer[256];
    uint8_t size = 0;

    uint8_t lastbit     = 0xFF;
    uint8_t stuff_count = 0;
    for(uint8_t *ptr = begin; ptr != end; ++ptr) {
        buffer[size++] = *ptr; /*  */

        if(lastbit != buffer[size - 1]) {
            lastbit     = buffer[size - 1];
            stuff_count = 1;
        }
        else {
            ++stuff_count;
        }

        if(stuff_count == 5) {
            lastbit        = !lastbit;
            stuff_count    = 1;
            buffer[size++] = lastbit;
        }
    }
    // memcpy()
    // for(uint32_t i = 0; i < 10; ++i) {
    //     buffer[size++] = 1;
    // }
    printf("\n");
    memcpy(dst, buffer, size);
    uint8_t stuffed_size = size - (end - begin);
    return stuffed_size;
}

/* transform an integer into a bitarray */
static void int_to_bitarray(uint8_t *dst, uint32_t num, uint8_t size) {
    assert(size < 250);
    uint8_t buffer[256] = {0};
    for(uint32_t i = 0; i < size; ++i) {
        buffer[i] = ((num & (1 << (size - 1 - i))) ? 1 : 0);
    }
    // strncpy(dst+copy_binary_message_counter, buffer, size);
    memcpy(dst, buffer, size);
    // return size;
}

static uint8_t autoincrement_ptr_int_to_bitarray(uint8_t **dst, uint32_t num, uint8_t size) {
    int_to_bitarray(*dst, num, size);
    *dst += size;
    return size;
}

static void encoder_message_add_num_to_bits(CAN_message_typedef *p_encoded_message, uint32_t num, uint8_t size) {
    assert(((uint32_t)p_encoded_message->length + (uint32_t)size) < 0xFF);
    int_to_bitarray(p_encoded_message->bitarray + p_encoded_message->length, num, size);
    p_encoded_message->length += size;
}

void encoder_encode_msg(CAN_configs_typedef config, CAN_message_typedef *p_encoded_message) {
    // static uint8_t encoded_message[256];
    memset(p_encoded_message->bitarray, 0, sizeof p_encoded_message->bitarray);
    p_encoded_message->length = 0;

    // copy_binary(encoded_message->bitarray, 0, 1); /* SoF */
    // copy_binary(encoded_message->bitarray, config.StdId, 11);
    uint8_t *ptr_bitarr = p_encoded_message->bitarray;
    uint8_t *length     = &p_encoded_message->length;

    // *length += autoincrement_ptr_int_to_bitarray(&ptr_bitarr, 0, 1); /* SoF */
    // *length += autoincrement_ptr_int_to_bitarray(&ptr_bitarr, config.StdId, 11);

    encoder_message_add_num_to_bits(p_encoded_message, 0, 1);
    /* O BUG é que não tá começando com length == 0 !!! */
    encoder_message_add_num_to_bits(p_encoded_message, config.StdId, 11);

    uint8_t data_length = (config.DLC > 8) ? 8 : config.DLC;
    if(config.IDE == 0) {
        /* frame Standard */
        encoder_message_add_num_to_bits(p_encoded_message, config.RTR, 1);
        encoder_message_add_num_to_bits(p_encoded_message, config.IDE, 1);
        encoder_message_add_num_to_bits(p_encoded_message, 0, 1); /* r0 */
        encoder_message_add_num_to_bits(p_encoded_message, config.DLC, 4);
        if(config.RTR == 0) {
            /* DATA frame */
            for(uint32_t i = 0; i < data_length; ++i) {
                encoder_message_add_num_to_bits(p_encoded_message, config.data[i], 8);
            }
        }
        else {
            /* REMOTE frame
             * não preciso fazer mais nada, pois remote não tem payload
             * */
        }
    }
    else {
        /* IDE == 1, frame estendido */
        encoder_message_add_num_to_bits(p_encoded_message, 1, 1); /* SRR must be recessive (1) */
        encoder_message_add_num_to_bits(p_encoded_message, config.IDE, 1);
        encoder_message_add_num_to_bits(p_encoded_message, config.ExtId, 18);
        encoder_message_add_num_to_bits(p_encoded_message, config.RTR, 1);
        encoder_message_add_num_to_bits(p_encoded_message, 0, 1); /* r1 */
        encoder_message_add_num_to_bits(p_encoded_message, 0, 1); /* r0 */
        encoder_message_add_num_to_bits(p_encoded_message, config.DLC, 4);
        if(config.RTR == 0) {
            /* DATA frame */
            for(uint32_t i = 0; i < data_length; ++i) {
                // copy_binary(encoded_message->bitarray, config.data[i], 8);
                encoder_message_add_num_to_bits(p_encoded_message, config.data[i], 8);
            }
        }
        else {
            /*
             * REMOTE frame
             * não é necessário adicionar payload
             * */
        }
    }

    /* agora já tenho todos os dados, falta calcular o CRC */

    /* add CRC */
    uint32_t crc = crc15(p_encoded_message->bitarray, p_encoded_message->length);
    encoder_message_add_num_to_bits(p_encoded_message, crc, 15);


    /* bit stuffing aqui */
    // int32_t num_of_stuffed_bits
    //     = bit_stuffing(encoded_message->bitarray, encoded_message->bitarray, copy_binary_message_counter);
    // copy_binary_message_counter += num_of_stuffed_bits;

    p_encoded_message->length += bit_stuff(p_encoded_message->bitarray, p_encoded_message->bitarray,
                                           p_encoded_message->bitarray + p_encoded_message->length);

    /* A patir do CRC_delimiter não há mais bitstuffing*/
    /* add CRC_delimiter */
    encoder_message_add_num_to_bits(p_encoded_message, 1, 1);
    /* add ACK_field */
    encoder_message_add_num_to_bits(p_encoded_message, 1, 1);
    /* add ACK_delimiter */
    encoder_message_add_num_to_bits(p_encoded_message, 1, 1);
    /* add EOF */
    encoder_message_add_num_to_bits(p_encoded_message, 0x7F, 7);
    // bit_stuff(encoded_message->bitarray, encoded_message->bitarray, encoded_message->bitarray +
    // encoded_message->length);
}
