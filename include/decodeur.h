#ifndef __DECODEUR_H__
#define __DECODEUR_H__

#include <stdlib.h>
#include <stdio.h>
#include "../include/jpeg_reader.h"
#include "../include/bitstream.h"
#include "../include/huffman.h"
#include  <string.h>
#include "../include/allocation.h"
#include "../include/image.h"
#include "../include/iqzz.h"
#include "../include/idct.h"
#include "../include/decompression.h"
#include "../include/pgm_ppm.h"
#include "../include/conversion_rgb.h"
#include "../include/conversion_pixel.h"
#include "../include/upsampling.h"

extern void decodage(struct bitstream *stream, struct jpeg_desc *jdesc);

#endif
