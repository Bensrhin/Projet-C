#include "../include/conversion_pixel.h"

void ecriture_matrice(struct image* image_mcus, uint32_t mcu, uint8_t ***tableaux, uint8_t ***matrice_pixel)
{

  uint16_t height = ceil((double)((8 * image_mcus->nb_blocs_hori * mcu) / image_mcus->largeur));
  height *= 8 * image_mcus->nb_blocs_vert;
  uint16_t width = 8 * image_mcus->nb_blocs_hori * (mcu % (image_mcus->largeur / (8 * image_mcus->nb_blocs_hori)));
  for (uint8_t p = 0; p < 64; p++)
  {
    for (uint8_t v=0; v<image_mcus->nb_blocs_vert;v++)
    {
      for (uint8_t h=0; h<image_mcus->nb_blocs_hori;h++)
      {
        uint16_t x = p / (8) + v*8;
        uint16_t y = p % (8) + h*8;

        for (uint8_t i = 0; i < image_mcus->couleur; i++)
        {
          if ((height+x<image_mcus->hauteur) && (width+y<image_mcus->largeur))
          {
            matrice_pixel[height + x][width + y][i] = tableaux[i][v*image_mcus->nb_blocs_hori + h][p];
          }
        }
      }
    }
  }
}
