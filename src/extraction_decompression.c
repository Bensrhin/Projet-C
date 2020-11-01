#include "../include/decompression.h"

/* D'abord: l'extraction des tables DC et AC du bloc d'indice i*/
void extraction_un_bloc(struct jpeg_desc *jdesc, struct huff_table * table_DC,
  struct huff_table * table_AC, uint8_t Y_Cb_Cr)
  {
    /* i : est l'indice de bloc à extraire*/
    table_DC = get_huffman_table(jdesc, DC, Y_Cb_Cr);
    table_AC = get_huffman_table(jdesc, AC, Y_Cb_Cr);
  }

void decompression_bloc(struct bitstream *stream,
  struct huff_table * table_DC,
  struct huff_table * table_AC, uint16_t *BLOC_8x8,
  uint16_t predicateur){

  uint8_t magnitude_DC = next_huffman_value(table_DC, stream);
  /* cette fonction retourne la prochaine valeur atteinte en parcourant la table
      de Huffman, telle valeur est codée sur un octet non signé,
      elle représente la magnitude du prochain coefficient DC.*/
  uint32_t byte = 0;
  read_bitstream(stream, magnitude_DC, &byte, true);
  uint16_t res;
  if (byte < (1 << (magnitude_DC - 1) )) {
      res =  byte - (1<< magnitude_DC) + 1;
} else {
      res = byte;
    }

  BLOC_8x8[0] = res + predicateur;

  /* Le décodage des 63 coefficients AC */
  uint8_t symbole_rle;
  for (uint8_t coeff = 1; coeff < 64; coeff++)
  {
    uint32_t byte = 0;
    /*next_huffman_value : retourne un symbole RLE des coefficients AC*/
    symbole_rle = next_huffman_value(table_AC, stream);
    uint8_t magnitude_AC = symbole_rle % 16; /* bits du poids faible */
    int8_t nombre_de_zero = symbole_rle / 16;  /* bits du poids fort */
    /* On a trois cas à traiter: */
    if (nombre_de_zero == 0 && magnitude_AC == 0)
    {
      /* End of block*/
      break;
    }
    else if (nombre_de_zero == 15 && magnitude_AC == 0)
    {
      /* code ZRL:  rle == 0xF0: désigne un saut de 16 coefficients nuls */
      coeff += nombre_de_zero;
    }
    else if (magnitude_AC == 0)
    {
      /* 0x?0 */
      fprintf(stderr, "symbole invalide\n");
    }
    else if (0 < magnitude_AC && magnitude_AC < 11)
    {
      /* 0xab */
      /*a coefficients nuls, puis coefficient non nul de magnitude b*/
      coeff += nombre_de_zero;
      read_bitstream(stream, magnitude_AC, &byte, true);
      uint16_t res;
      if (byte < (1 << (magnitude_AC - 1) )) {
          res =  byte - (1<< magnitude_AC) + 1;
    } else {
          res = byte;
      }
      BLOC_8x8[coeff] = res;
    }
  }
}
