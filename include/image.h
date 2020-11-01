#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "jpeg_reader.h"

struct image{
  uint32_t hauteur;
  uint32_t hauteur_reelle;
  uint32_t largeur;
  uint32_t largeur_reelle;
  uint32_t nb_mcus;
  uint8_t nb_blocs_hori;
  uint8_t nb_blocs_vert;
  uint8_t couleur;
  uint8_t *dim_couleur;
};

extern struct image* image_details(struct jpeg_desc *jdesc);

#endif
