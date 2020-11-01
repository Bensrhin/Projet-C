#include "../include/pgm_ppm.h"
#include <string.h>

void conv(const char *filename, char *nv, uint8_t indice){
  uint8_t point = 0;
  for(size_t i=0; i<strlen(filename); i++){
    if (filename[i] != '.')
    {
    nv[i] = filename[i];
    }
    else
    {
      nv[i] = '.';
      point = i;
      break;
    }
  }
  nv[point + 1] = 'p';
  if (indice == 1){
    nv[point + 2] = 'g';
  }
  else{
    nv[point + 2] ='p';
  }

  nv[point + 3] = 'm';
  nv[point + 4] = '\0';
}

void pgm_ppm(struct image* image_mcus, uint8_t ***matrice_pixel, struct jpeg_desc *jdesc){
  uint8_t couleur = get_nb_components(jdesc);
  const char *filename = get_filename(jdesc);
  char nv[strlen(filename)];
  conv(filename, nv, couleur);
  FILE *fichier;
  fichier = fopen(nv, "wb");
  if (couleur == 1){
    fprintf(fichier, "P5\n");
  }
  else{
    fprintf(fichier, "P6\n");
  }

  fprintf(fichier, "%d %d\n", image_mcus->largeur_reelle , image_mcus->hauteur_reelle);
  fprintf(fichier, "255\n");
  for(uint16_t h = 0; h<image_mcus->hauteur_reelle; h++){
     for(uint16_t l = 0; l<image_mcus->largeur_reelle; l++){
       for (uint8_t i = 0; i < couleur; i++){
            fwrite(&matrice_pixel[h][l][i], sizeof(uint8_t), 1, fichier);

          }
    }
  }
  fclose(fichier);

}
