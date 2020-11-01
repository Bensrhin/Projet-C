#include <stdlib.h>
#include <stdio.h>
#include "../include/bitstream.h"
#include "../include/huffman.h"
#include "../include/jpeg_reader.h"
#define LEFT  0
#define RIGHT 1

struct huff_table{
  // les informations sont fournies en deux temps:
  /* < 0 3 2 d e a c > :
          - 0 3 2     : nombre de codes pour les profondeurs 1 2 et 3.
          - d e a c b : valeurs des feuilles.
  */
  // D’abord, on donne le nombre de codes de chaque longueur
  //  comprise entre 1 et 16 bits, chaque nombre est codé sur
  // deux octets non signés car : np <= 2**p - 1.
  // on utilise uint32_t car on l'utilise dans le read_bitstream.
  uint32_t *nombre_de_codes;
  // Ensuite, on donne les valeurs triées dans l’ordre des codes.
  // une feuille contenant une valeur codée sur un octet non signé
  uint8_t *valeurs_feuilles;
  struct Noeud *racine;
};

uint32_t *valeur(struct huff_table *table){

  return table->nombre_de_codes;
}
struct Noeud{
  uint8_t valeur;
  uint32_t indice;
  bool est_feuille;
  struct Noeud *fils_gauche;
  struct Noeud *fils_droite;
};
void free_noeud(struct Noeud * noeud){
  if (noeud == NULL){ return ;}
  free_noeud(noeud->fils_droite);
  free_noeud(noeud->fils_gauche);
  free(noeud);
}
void free_huffman_table(struct huff_table *table){
  if (table == NULL){return;}
  free_noeud(table->racine);
  free(table->valeurs_feuilles);
  free(table->nombre_de_codes);
  free(table);

}
// void free_tree(struct Noeud **tree){
//   if (tree == NULL){ return ;}
//   free_noued(*tree);
// }

struct huff_table *load_huffman_table(struct bitstream *stream,
                                      uint16_t *nb_byte_read){

    // parcourt le flux de bits stream pour construire une
    // table de Huffman propre à l’image en cours de décodage
    struct huff_table *table_huffman = malloc(sizeof(struct huff_table));

    table_huffman->nombre_de_codes = malloc(16*sizeof(uint32_t));
    // on parcourt les 16 profondeurs tout en enregistrant le nombre des codes
    uint32_t nombre_code_total = 0;
    uint32_t byte;
    for (uint8_t i=0; i<16; i++){
      read_bitstream(stream, 8, &(table_huffman->nombre_de_codes)[i], true);
      //(table_huffman->nombre_de_codes)[i] = byte;
      nombre_code_total += table_huffman->nombre_de_codes[i];
      *nb_byte_read += 1;
    }

    table_huffman->valeurs_feuilles = malloc(nombre_code_total*sizeof(uint8_t));
    // on parcourt les 16 profondeurs en enregistrant cette fois les valeurs.
    for (uint8_t i=0; i<nombre_code_total; i++){
      read_bitstream(stream, 8, &(byte), true);
      table_huffman->valeurs_feuilles[i] = byte;
      //table_huffman->valeurs_feuilles[i] = byte;
      *nb_byte_read += 1;
    }
    // Cette fonction retourne l’adresse d’une structure de type
    // struct huff_table crée.
    struct Noeud *racine = huff_table_to_arbre(table_huffman);
    table_huffman->racine = racine;
    return table_huffman;
}



void arbre(struct Noeud *racine, uint32_t profondeur, struct Noeud **tree){
  if (profondeur >= 16){
    racine->valeur = 0;
    racine->est_feuille = true;
    racine->fils_gauche = NULL;
    racine->fils_droite = NULL;
    //printf("meeee \n" );
  }
  else
  {

    struct Noeud *fils_gauche = malloc(sizeof(struct Noeud));
    struct Noeud *fils_droite = malloc(sizeof(struct Noeud));
    racine->valeur = 0;
    racine->est_feuille = false;
    fils_gauche->indice = 2*(racine->indice) + 1;
    tree[fils_gauche->indice] = fils_gauche;
    fils_droite->indice = 2*(racine->indice) + 2;
    tree[fils_droite->indice] = fils_droite;
    //printf("haha \n" );
    racine->fils_gauche = fils_gauche;
    //printf("2 \n" );
    racine->fils_droite = fils_droite;
    arbre(fils_gauche, profondeur + 1, tree);
    //printf("midle \n" );
    arbre(fils_droite, profondeur + 1, tree);
    //printf("fin \n" );
  }
}
// void parcours(struct Noeud *racine){
//   char tab2[5] = {"d", "e", "a", "c", "b"};
//   if (racine->est_feuille){
//     printf("%u ",racine->valeur);
//   }
//   else{
//     //printf("%u %u  \n", racine->indice, racine->fils_droite->indice);
//     parcours(racine->fils_gauche);
//     //printf("%u \n", racine->indice);
//     parcours(racine->fils_droite);
//   }
// }
struct Noeud * huff_table_to_arbre(struct huff_table *table_huffman){
    struct Noeud *racine = malloc(sizeof(struct Noeud));
    racine->indice=0;
    racine->est_feuille=false;
    struct Noeud **tree = malloc((2<<16)*sizeof(struct Noeud*));
    // for (uint32_t i=0; i<(2<<16); i++){
    //   tree[i] = malloc((2<<16)*sizeof(struct Noeud));
    //}
    arbre(racine, 0, tree);
    //printf("%u \n", !tree[(2<<15)-16]->est_feuille);
    uint32_t sauter = 0, sauter_prof = 1;
    uint32_t feuille;

    for (uint32_t prof=0; prof<16; prof++){
      //if (prof == 4){ return racine;}
      for (feuille=0; feuille<table_huffman->nombre_de_codes[prof]; feuille++){
        // printf(" feuille %u \n", feuille);
        // printf(" feuille %u \n", table_huffman->nombre_de_codes[prof]);

        tree[sauter_prof+feuille]->est_feuille = true;

        tree[sauter_prof+feuille]->valeur = table_huffman->valeurs_feuilles[sauter+feuille];
        //printf("valeur :  %u \n", tree[sauter_prof+feuille]->valeur);
      }

      sauter += table_huffman->nombre_de_codes[prof];
      sauter_prof += table_huffman->nombre_de_codes[prof];
      sauter_prof = 2*sauter_prof + 1;
      //printf("pou %u \n", table_huffman->nombre_de_codes[prof]);
    }



    free(tree);
    //parcours(racine);
    return racine;
}

int8_t next_huffman_value(struct huff_table *table,
                            struct bitstream * stream){
    struct Noeud *racine = table->racine;
    uint32_t byte;
    while (!racine->est_feuille){
      read_bitstream(stream, 1, &byte, true);
      if (byte == 0){
        racine = racine->fils_gauche;
      }
      else {
        racine = racine->fils_droite;
      }
    }
    return racine->valeur;
}







//
// int main(){
//   struct huff_table * huffman = malloc(sizeof(struct huff_table));
//   int16_t tab1[16] = {0, 3 ,2};
//   // tab1[0] = 0;
//   // tab1[1] = 3;
//   // tab1[2] = 2;
//   huffman->nombre_de_codes = malloc(16*sizeof(uint16_t));
//   for(uint8_t i=0; i<16;i++){
//     huffman->nombre_de_codes[i] = tab1[i];
//     printf("%u ", huffman->nombre_de_codes[i]);
//   }
//   //huffman->nombre_de_codes = tab1;
//   printf("\n");
//   uint8_t tab3[5] = {0, 1, 2, 3, 4};
//   huffman->valeurs_feuilles = calloc((2<<15), sizeof(uint8_t));
//   //huffman->valeurs_feuilles = tab3;
//   for(uint8_t i=0; i<5;i++){
//     huffman->valeurs_feuilles[i] =tab3[i];
//   }
//   for(uint8_t i=0; i<16;i++){
//     printf("%u ", huffman->valeurs_feuilles[i]);
//   }
//   //printf("%u \n", (2<<15)-16);
//   struct Noeud *racine = huff_table_to_arbre(huffman);
//   parcours(racine);
// }
