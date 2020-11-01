#include "../include/conversion_rgb.h"


void RGB(uint8_t *Y, uint8_t *Cb, uint8_t *Cr, uint8_t *R, uint8_t *G, uint8_t *B)
{
  for (uint8_t i = 0; i<64; i++)
  {
    float y = Y[i], cb = Cb[i], cr = Cr[i], r, g , b;
    r = saturer(y  + 1.4016868 * (cr - 128.0));
    g = saturer(y - 0.3436954 * (cb - 128.0) - 0.7141690 * (cr - 128.0));
    b = saturer(y + 1.7721604 * (cb - 128.0));
    R[i] = r, G[i] = g, B[i] = b;
  }
}

uint8_t *** conversion_YCbCr_vers_RGB(uint8_t ***tableaux, uint8_t couleur,
                          uint8_t dimension)
{
  if (couleur == 3)
  {
    uint8_t ***pixel_of_mcu = allocation_sampling(couleur, dimension);
    for (uint8_t bloc = 0; bloc<dimension; bloc++)
    {

      RGB(tableaux[0][bloc], tableaux[1][bloc], tableaux[2][bloc], pixel_of_mcu[0][bloc], pixel_of_mcu[1][bloc], pixel_of_mcu[2][bloc]);
    }
    return pixel_of_mcu;
  }
  else
  {
    return tableaux;
  }
}
