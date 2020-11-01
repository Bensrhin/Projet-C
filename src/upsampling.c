#include "../include/upsampling.h"

void upsampling_horizental(uint8_t *table, uint8_t *table_gauche,
uint8_t *table_droite){
  bool gauche = false;
  uint8_t compte, indice_d = 0, indice_g = 0;
  for (uint8_t i = 0; i<64; i++){
    compte = i % 4;
    if (compte == 0){ gauche = !gauche;}
    if (gauche){
    table_gauche[indice_g] = table[i];
    table_gauche[indice_g + 1] = table[i];
    indice_g += 2;
    }
    else {
      table_droite[indice_d] = table[i];
      table_droite[indice_d + 1] = table[i];
      indice_d += 2;
    }
  }
}
void upsampling_vertical(uint8_t *table, uint8_t *table_haut,
uint8_t *table_bas){
  for (uint8_t i=0; i<32; i++){

    table_haut[8*(i/8) + i] = table[i];
    table_haut[8*(i/8) + i + 8] = table[i];
    table_bas[8*(i/8) + i] = table[i + 32];
    table_bas[8*(i/8) + i + 8] = table[i + 32];
  }
}

uint8_t cas_sampling(uint8_t hori, uint8_t hori_C, uint8_t vert, uint8_t vert_C)
{
  if ((hori_C == hori) && (vert==vert_C))
  {
    return 1;
  }
  else if ((hori_C == hori / 2) && (vert_C == vert))
  {
    return 2;
  }
  else if ((hori_C == hori) && (vert_C == vert/2))
  {
    return 3;
  }
  else if ((hori_C == hori/2) && (vert_C == vert/2))
  {
    return 4;
  }
  else if ((hori_C == hori/4) && (vert_C == vert))
  {
    return 5;
  }
  else if ((hori_C == hori) && (vert_C == vert/4))
  {
    return 6;
  }
  else if ((hori_C == hori/4) && (vert_C == vert/2))
  {
    return 7;
  }
  else if ((hori_C == hori/2) && (vert_C == vert/4))
  {
    return 8;
  }
  else
  {
    fprintf(stderr, "ce cas d'échantillonage n'est pas compatible avec notre décodeur.\n");
    return 10;
  }
}
void stockage(struct jpeg_desc *jdesc, uint8_t *tableau8, uint8_t ***tableaux, uint8_t j,
              uint8_t hori, uint8_t vert, uint8_t n)
{
    uint8_t hori_C = get_frame_component_sampling_factor(jdesc, DIR_H, j);
    uint8_t vert_C = get_frame_component_sampling_factor(jdesc, DIR_V, j);
    uint8_t cas = cas_sampling(hori, hori_C, vert, vert_C);


    if (cas == 1)
    {
      tableaux[j][n] = tableau8;
    }
    uint8_t * table_droite = malloc(64*sizeof(uint8_t));
    uint8_t * table_gauche = malloc(64*sizeof(uint8_t));
    if (cas == 2)
    {
      upsampling_horizental(tableau8, table_gauche, table_droite);
      tableaux[j][2*n] = table_gauche;
      tableaux[j][2*n + 1] =  table_droite;

    }
    else if (cas == 3)
    // sous_échantillonnage vertical
    {
      upsampling_vertical(tableau8, table_gauche, table_droite);
      tableaux[j][n ] = table_gauche;
      tableaux[j][n + hori] = table_droite;


    }
    else if (cas > 3)
    // sous_échantillonnage vertical et  horizental
    {
      uint8_t * table_haut_gauche = malloc(64*sizeof(uint8_t));
      uint8_t * table_bas_gauche = malloc(64*sizeof(uint8_t));
      uint8_t * table_haut_droite = malloc(64*sizeof(uint8_t));
      uint8_t * table_bas_droite = malloc(64*sizeof(uint8_t));

      if (cas == 4){
        upsampling_horizental(tableau8, table_gauche, table_droite);
        upsampling_vertical(table_gauche, table_haut_gauche, table_bas_gauche);
        upsampling_vertical(table_droite, table_haut_droite, table_bas_droite);
        tableaux[j][2*n] = table_haut_gauche;
        tableaux[j][2*n + 1] = table_haut_droite;
        tableaux[j][2*n + hori] = table_bas_gauche;
        tableaux[j][2*n + 1 + hori] = table_bas_droite;
        free(table_droite);
        free(table_gauche);
      }
      else if (cas == 7){
        uint8_t * table_gauche_1 = malloc(64*sizeof(uint8_t));
        uint8_t * table_droite_1 = malloc(64*sizeof(uint8_t));
        upsampling_horizental(tableau8, table_gauche_1, table_droite_1);

        upsampling_horizental(table_gauche_1, table_gauche, table_droite);
        upsampling_vertical(table_gauche, table_haut_gauche, table_bas_gauche);
        upsampling_vertical(table_droite, table_haut_droite, table_bas_droite);

        tableaux[j][n] = table_haut_gauche;
        tableaux[j][n + 1] = table_haut_droite;
        tableaux[j][n + hori] = table_bas_gauche;
        tableaux[j][n + hori + 1] = table_bas_droite;

        upsampling_horizental(table_droite_1, table_gauche, table_droite);
        upsampling_vertical(table_gauche, table_haut_gauche, table_bas_gauche);
        upsampling_vertical(table_droite, table_haut_droite, table_bas_droite);

        tableaux[j][n + 2] = table_haut_gauche;
        tableaux[j][n + 3] = table_haut_droite;
        tableaux[j][n + 2 + hori] = table_bas_gauche;
        tableaux[j][n + 3 + hori] = table_bas_droite;
        free(table_droite);
        free(table_gauche);
        free(table_droite_1);
        free(table_gauche_1);
      }
      else if (cas == 8){
        uint8_t * table_gauche_1 = malloc(64*sizeof(uint8_t));
        uint8_t * table_droite_1 = malloc(64*sizeof(uint8_t));
        upsampling_vertical(tableau8, table_gauche_1, table_droite_1);

        upsampling_horizental(table_gauche_1, table_gauche, table_droite);
        upsampling_vertical(table_gauche, table_haut_gauche, table_bas_gauche);
        upsampling_vertical(table_droite, table_haut_droite, table_bas_droite);

        tableaux[j][0] = table_haut_gauche;
        tableaux[j][1] = table_haut_droite;
        tableaux[j][2] = table_bas_gauche;
        tableaux[j][3] = table_bas_droite;

        upsampling_horizental(table_droite_1, table_gauche, table_droite);
        upsampling_vertical(table_gauche, table_haut_gauche, table_bas_gauche);
        upsampling_vertical(table_droite, table_haut_droite, table_bas_droite);

        tableaux[j][4] = table_haut_gauche;
        tableaux[j][5] = table_haut_droite;
        tableaux[j][6] = table_bas_gauche;
        tableaux[j][7] = table_bas_droite;
;
        free(table_droite);
        free(table_gauche);
        free(table_droite_1);
        free(table_gauche_1);
      }
      else if (cas == 5) {
        upsampling_horizental(tableau8, table_gauche, table_droite);
        upsampling_horizental(table_gauche, table_haut_gauche, table_bas_gauche);
        upsampling_horizental(table_droite, table_haut_droite, table_bas_droite);
        tableaux[j][4*n] = table_haut_gauche;
        tableaux[j][4*n + 2] = table_haut_droite;
        tableaux[j][4*n + 1] = table_bas_gauche;
        tableaux[j][4*n + 3] = table_bas_droite;

        free(table_droite);
        free(table_gauche);
      }
      else {
        upsampling_vertical(tableau8, table_gauche, table_droite);
        upsampling_vertical(table_gauche, table_haut_gauche, table_bas_gauche);
        upsampling_vertical(table_droite, table_haut_droite, table_bas_droite);
        tableaux[j][n] = table_haut_gauche;
        tableaux[j][n + 2*hori] = table_haut_droite;
        tableaux[j][n + hori] = table_bas_gauche;
        tableaux[j][n + 3*hori] = table_bas_droite;

        free(table_droite);
        free(table_gauche);
      }
  }
}
