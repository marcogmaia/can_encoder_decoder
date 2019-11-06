#include <stdio.h>
#include <stdlib.h>
#include "decoder.h"
#include "encoder.h"


void print_binary(uint8_t bin) {
    printf("%u%u%u%u%u%u%u%u\n", bin & 0x80 ? 1 : 0, bin & 0x40 ? 1 : 0, bin & 0x20 ? 1 : 0, bin & 0x10 ? 1 : 0,
           bin & 0x08 ? 1 : 0, bin & 0x04 ? 1 : 0, bin & 0x02 ? 1 : 0, bin & 0x01 ? 1 : 0);
}

encoder_configs_typedef config = {
    .StdId = 0x0672,
    .IDE   = 0,
    .RTR   = 0,
    .DLC   = 7,
};


void binary_to_str(char *dst, uint8_t *src, uint32_t size) {
    for(uint32_t i = 0; i < size; ++i) {
        dst[i] = src[i] + '0';
    }
}

int main() {
    uint8_t data[] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    config.DLC     = sizeof data;
    config.data    = data;
    if(config.DLC > 8) config.DLC = 8;


    uint32_t size;
    uint8_t *returned_msg = encoder_encode_msg(config, &size);
    uint8_t buffer[256]   = {0};
    binary_to_str(buffer, returned_msg, size);
    printf("%s\n", buffer);


    return 0;
}
