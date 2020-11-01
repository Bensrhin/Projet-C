#ifndef __ALLOCATION_H__
#define __ALLOCATION_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

extern uint8_t *** allocation_image(uint32_t hauteur, uint32_t largeur, uint8_t couleur);
extern void free_image(uint8_t ***mat, uint32_t hauteur, uint32_t largeur);
extern uint8_t *** allocation_sampling(uint8_t couleur, uint8_t bloc_in_mcu);
extern void free_sampling(uint8_t ***mat, uint8_t couleur, uint8_t bloc_in_mcu);

#endif
