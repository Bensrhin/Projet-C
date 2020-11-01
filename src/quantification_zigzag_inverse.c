#include "../include/iqzz.h"
#include <string.h>
void quantification_inverse(struct jpeg_desc *jpeg, uint16_t *table, uint8_t indice){
  /* multiplier élément par élément le bloc fréquentiel par la table
      de quantification */
  uint8_t *table_quantification = get_quantization_table(jpeg, indice);
  /* Valeurs de la table de quantification, stockées au format
    zig-zag de taille 64 octets, donc chaque case codée sur un octet*/
  for(uint8_t i = 0; i<64; i++)
  {
    table[i] *= table_quantification[i];
  }

}


void zig_zag_inverse(uint16_t * table)
{
  /*la réorganisation zig-zag inverse*/
  uint8_t coord_zig_zag[64] =
  {0,   1,  8, 16,  9,  2,  3, 10,
   17, 24, 32, 25, 18, 11,  4,  5,
   12, 19, 26, 33, 40, 48, 41, 34,
   27, 20, 13,  6,  7, 14, 21, 28,
   35, 42, 49, 56, 57, 50, 43, 36,
   29, 22, 15, 23, 30, 37, 44, 51,
   58, 59, 52, 45, 38, 31, 39, 46,
   53, 60, 61, 54, 47, 55, 62, 63};
   /* creer une copie de la table */
   uint16_t copie[64];
   memcpy(copie, table, sizeof copie);
   for(int i=0; i<64;i++)
   {
    table[coord_zig_zag[i]] = copie[i];
   }

}
