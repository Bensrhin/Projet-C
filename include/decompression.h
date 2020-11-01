#ifndef __DECOMPRESSION_H__
#define __DECOMPRESSION_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "jpeg_reader.h"
#include "bitstream.h"
#include "huffman.h"

extern void extraction_un_bloc(struct jpeg_desc *jdesc, struct huff_table * table_DC,
  struct huff_table * table_AC, uint8_t Y_Cb_Cr);

extern void decompression_bloc(struct bitstream *stream,
  struct huff_table * table_DC,
  struct huff_table * table_AC, uint16_t *BLOC_8x8,
  uint16_t predicateur);
  
#endif
