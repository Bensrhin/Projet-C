#ifndef __IQZZ_H__
#define __IQZZ_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "jpeg_reader.h"


extern void quantification_inverse(struct jpeg_desc *jpeg, uint16_t *table, uint8_t indice);
extern void zig_zag_inverse(uint16_t * table);


#endif
