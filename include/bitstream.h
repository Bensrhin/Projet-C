#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <stdint.h>
#include <stdbool.h>





struct bitstream;

extern bool indice(struct bitstream *stream);

extern struct bitstream *create_bitstream(const char *filename);

/* Fonction auxiliaire qui sert à lire le premier bit non encore lu de l'octet chargé de la structure stream, decremente nb_bits_restants
 elle modifie e paramettre dest en le decalant de 1 vers la gauche et y ajouter le bit lu */

extern void lecture_bit_of_proch_octet(struct bitstream *stream, uint32_t *dest);

extern uint8_t read_bitstream(struct bitstream *stream,
                              uint8_t nb_bits,
                              uint32_t *dest,
                              bool discard_byte_stuffing);

extern bool end_of_bitstream(struct bitstream *stream);

extern void close_bitstream(struct bitstream *stream);

#endif
