#include "../include/decodeur.h"

void decodage(struct bitstream *stream, struct jpeg_desc *jdesc){

  struct huff_table * table_DC;
  struct huff_table * table_AC;
  uint8_t ***pixel_of_mcu;

  struct image* image_mcus = image_details(jdesc);
  uint8_t ***matrice_pixel = allocation_image(image_mcus->hauteur,
                              image_mcus->largeur, image_mcus->couleur);

  uint8_t ***tableaux = allocation_sampling(image_mcus->couleur,
                               image_mcus->dim_couleur[0]);

  uint16_t predicateur[3] = {0};
  for (uint32_t mcu = 0; mcu < image_mcus->nb_mcus; mcu++)
  {
    for (uint8_t couleur = 0; couleur < image_mcus->couleur; couleur++)
    {
      uint8_t ordre = get_frame_component_id (jdesc, couleur) - 1;
      uint8_t indice = get_scan_component_huffman_index(jdesc, DC, ordre);

      for (uint8_t bloc = 0; bloc<image_mcus->dim_couleur[ordre]; bloc++)
      {
        uint16_t *bloc_of_bits = calloc(64,sizeof(uint16_t));

        table_DC = get_huffman_table(jdesc, DC, indice);
        table_AC = get_huffman_table(jdesc, AC, indice);
        decompression_bloc(stream, table_DC, table_AC, bloc_of_bits, predicateur[ordre]);
        predicateur[ordre] = bloc_of_bits[0];

        quantification_inverse(jdesc, bloc_of_bits, indice);

        zig_zag_inverse(bloc_of_bits);

        uint8_t * bloc_of_pixel = malloc(64*sizeof(uint8_t));
        stages(bloc_of_bits, bloc_of_pixel);
        free(bloc_of_bits);
        if (indice == 1)
        {
          stockage(jdesc, bloc_of_pixel, tableaux, ordre, image_mcus->nb_blocs_hori, image_mcus->nb_blocs_vert, bloc);
        }
        else
        {
          tableaux[ordre][bloc] = bloc_of_pixel;
        }
      }
    }
    pixel_of_mcu = allocation_sampling(image_mcus->couleur,
                                 image_mcus->dim_couleur[0]);
    pixel_of_mcu = conversion_YCbCr_vers_RGB(tableaux, image_mcus->couleur, image_mcus->dim_couleur[0]);
    ecriture_matrice(image_mcus, mcu, pixel_of_mcu, matrice_pixel);
  }
  free_sampling(pixel_of_mcu, image_mcus->couleur, image_mcus->dim_couleur[0]);
  pgm_ppm(image_mcus, matrice_pixel, jdesc);
  free_image(matrice_pixel, image_mcus->hauteur, image_mcus->largeur);
}
