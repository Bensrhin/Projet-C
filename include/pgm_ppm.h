#ifndef __PGM_PPM_H__
#define __PGM_PPM_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "jpeg_reader.h"
#include "image.h"

extern void conv(const char *filename, char *nv, uint8_t indice);
extern void pgm_ppm(struct image* image_mcus, uint8_t ***matrice_pixel,
            struct jpeg_desc *jdesc);

#endif
