#ifndef __UPSAMPLING_H__
#define __UPSAMPLING_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "jpeg_reader.h"


extern void upsampling_horizental(uint8_t *table, uint8_t *table_gauche,
uint8_t *table_droite);
extern void upsampling_vertical(uint8_t *table, uint8_t *table_haut,
uint8_t *table_bas);
extern uint8_t cas_sampling(uint8_t hori, uint8_t hori_C, uint8_t vert, uint8_t vert_C);
extern void stockage(struct jpeg_desc *jdesc, uint8_t *tableau8, uint8_t ***tableaux, uint8_t j,
              uint8_t hori, uint8_t vert, uint8_t n);


#endif
