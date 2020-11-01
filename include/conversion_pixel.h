#ifndef __CONVERSION_PIXEL_H__
#define __CONVERSION_PIXEL_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "image.h"


extern void ecriture_matrice(struct image* image_mcus, uint32_t mcu,
          uint8_t ***tableaux, uint8_t ***matrice_pixel);


#endif
