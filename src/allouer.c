#include "../include/allocation.h"

uint8_t *** allocation_image(uint32_t hauteur, uint32_t largeur, uint8_t couleur)
{
  uint8_t*** mat = malloc(hauteur*sizeof(uint8_t**));
  for (uint32_t d=0; d<hauteur; d++)
  {
    mat[d] = malloc(largeur*sizeof(uint8_t*));
    for (uint32_t b=0; b<largeur;b++)
    {
      mat[d][b] = malloc(couleur*sizeof(uint8_t));
    }
  }
  return mat;
}
void free_image(uint8_t ***mat, uint32_t hauteur, uint32_t largeur)
{
  if (mat == NULL)
  {
    return;
  }
  for (uint32_t d=0; d<hauteur; d++)
  {
    for (uint32_t b=0; b<largeur;b++)
    {
      free(mat[d][b]);
    }
    free(mat[d]);
  }
  free(mat);
}

uint8_t *** allocation_sampling(uint8_t couleur, uint8_t bloc_in_mcu)
{
  uint8_t*** mat = malloc(couleur*sizeof(uint8_t**));
  for (uint32_t d=0; d<couleur; d++)
  {
    mat[d] = malloc(bloc_in_mcu*sizeof(uint8_t*));
    for (uint32_t b=0; b<bloc_in_mcu;b++)
    {
      mat[d][b] = malloc(64*sizeof(uint8_t));
    }
  }
  return mat;
}
void free_sampling(uint8_t ***mat, uint8_t couleur, uint8_t bloc_in_mcu)
{
  if (mat == NULL)
  {
    return;
  }
  for (uint32_t d=0; d<couleur; d++)
  {
    for (uint32_t b=0; b<bloc_in_mcu;b++)
    {
      free(mat[d][b]);
    }
    free(mat[d]);
  }
  free(mat);
}
