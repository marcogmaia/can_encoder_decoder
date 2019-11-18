#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decoder.h"
#include "encoder.h"


int main() {
    CAN_configs_typedef config = {
        .StdId = 0x7FF,
        .ExtId = 0x0,
        .IDE   = 0,
        .RTR   = 0,
        .DLC   = 15,
    };

    uint8_t data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    config.data    = data;

    CAN_message_typedef encoded_message;
    encoder_encode_msg(config, &encoded_message);

    char strbuf[256] = {0};
    bitarray_to_str(strbuf, encoded_message.bitarray, encoded_message.length);
    printf("encoded:\n%s\n", strbuf);

    return 0;
}
