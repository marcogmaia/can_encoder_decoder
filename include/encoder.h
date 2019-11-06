#pragma once
#include <stdint.h>

typedef struct encoder_configs {
    uint32_t StdId; /* Specifies the standard identifier.
                          This parameter must be a number between Min_Data = 0 and
                        Max_Data = 0x7FF. */
    uint32_t ExtId; /* Specifies the extended identifier.
                         This parameter must be a number between Min_Data = 0 and
                       Max_Data = 0x1FFFFFFF. */
    uint32_t IDE;   /* Specifies the type of frame for the message that will be
                       transmitted. */
    uint32_t RTR;   /* Specifies the type of frame for the message that will be
                       transmitted.*/
    uint32_t DLC;   /* Specifies the length of the frame that will be
                       transmitted. This parameter must be a number between Min_Data
                       = 0 and Max_Data = 8. */
    uint8_t *data;  /* pointer to data to be transmitted */
} encoder_configs_typedef;

uint8_t *encoder_encode_msg(encoder_configs_typedef config, uint32_t *returned_msg_size);