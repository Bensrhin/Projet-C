#ifndef __CONVERSION_RGB_H__
#define __CONVERSION_RGB_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "allocation.h"
#define saturer(valeur) (valeur  >  255  ?  255 : valeur < 0  ?   0 : valeur)


extern void RGB(uint8_t *Y, uint8_t *Cb, uint8_t *Cr, uint8_t *R, uint8_t *G, uint8_t *B);
extern uint8_t *** conversion_YCbCr_vers_RGB(uint8_t ***tableaux, uint8_t couleur,
                          uint8_t dimension);

#endif
