#include "../include/image.h"


struct image* image_details(struct jpeg_desc *jdesc){
  struct image *image_mcu = malloc(sizeof(struct image));

  image_mcu->hauteur_reelle = get_image_size(jdesc, DIR_V);
  image_mcu->largeur_reelle = get_image_size(jdesc, DIR_H);

  image_mcu->couleur = get_nb_components(jdesc);
  uint8_t hori;
  uint8_t vert;
  image_mcu->dim_couleur = malloc(image_mcu->couleur*sizeof(uint8_t));
  uint8_t somme=0;
  for (uint8_t r=0; r<image_mcu->couleur; r++){
    uint8_t ordre = get_frame_component_id (jdesc, r) - 1;
    uint8_t horii = get_frame_component_sampling_factor(jdesc, DIR_H, r);
    uint8_t verti = get_frame_component_sampling_factor(jdesc, DIR_V, r);
    if (ordre==0){
      hori = horii;
      image_mcu->nb_blocs_hori = horii;
      vert = verti;
      image_mcu->nb_blocs_vert = verti;
    }
    if (horii > 4 || horii < 1 || verti > 4 || verti < 1){
      fprintf(stderr, "La valeur d'un facteur h ou v n'est pas comprise entre 1 et 4.\n");
      return NULL;
    }
    somme += horii*verti;
    image_mcu->dim_couleur[ordre] = horii*verti;
  }
  if (somme > 10){
    fprintf(stderr, "La somme des produits hi*vi doit être inférieure ou égale à 10.\n");
    return NULL;
  }
  image_mcu->hauteur = (uint32_t)vert*8*ceil((double)get_image_size(jdesc, DIR_V)/(vert*8));
  image_mcu->largeur = (uint32_t)hori*8*ceil((double)get_image_size(jdesc, DIR_H)/(hori*8));
  uint32_t mcu_hor = (uint32_t)ceil((double)(image_mcu->largeur_reelle) / (8.0 * hori));
  uint32_t mcu_vert = (uint32_t)ceil((double)(image_mcu->hauteur_reelle) / (8.0 * vert));
  image_mcu->nb_mcus = mcu_hor * mcu_vert;
  return image_mcu;
}
