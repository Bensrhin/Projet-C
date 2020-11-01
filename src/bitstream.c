#include "../include/bitstream.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>


/************************/
/************************/
/*  Structure bitsream  */
/***********************/
/************************/


struct bitstream{
  /* Le champ suivant sert à pointer vers le fichier à lire */
  FILE *filename;
  /* Tableau contenant tous les octets du fichier */
  uint8_t *tab_octet;
  /* la taille du tableau precedent */
  uint32_t size;
  /* Indice de l'octet en cours de lecture */
  uint32_t indice_courant;
  /* Nombre de bite restants à lire dans l'octet precedent */
  uint8_t nb_bits_restants;
};

/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/
/*********************** La fonction create_bitstream crée un flux, positionné au début du fichier filename . **********************************************/
/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/

 struct bitstream *create_bitstream(const char *filename){
  struct bitstream *bstream = malloc(sizeof(struct bitstream));
  if(bstream == NULL){
    printf("Creation echouée. Oui vous pouvez réessayez !! \n");
		return NULL;
  }
  /* On ouvre le fichier en mode binaire */
	bstream->filename = fopen(filename, "rb");
  if (bstream->filename == NULL) {
		printf("L'ouverture du fichier donnée est échouée :(. Oui vous pouvez réessayez !! \n");
		return NULL;
  }else{
    /* Obtenir la taille du filename */
    fseek(bstream->filename, 0, SEEK_END);
    bstream->size = ftell(bstream->filename);
    rewind(bstream->filename);
    /* Allocation memoire pour contenir tout le contenut du fichier */
    bstream->tab_octet = (uint8_t*)malloc(sizeof(uint8_t)*bstream->size);
    if(bstream->tab_octet == NULL){
      printf("L'ouverture du fichier donnée est échouée :(. Oui vous pouvez réessayez !! \n");
  		return NULL;
    }else{
      /* Copy du contenut du fichier dans le tab_octet */
      size_t test = fread(bstream->tab_octet, sizeof(uint8_t), bstream->size, bstream->filename);
      if(test != bstream->size){
        printf("L'ouverture du fichier donnée est échouée :(. Oui vous pouvez réessayez !! \n");
    		return NULL;
      }else{
        bstream->indice_courant = 0;
        bstream->nb_bits_restants = 8;
      }
    }
    return bstream;
  }
}

/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/
/* Fonction auxiliaire qui sert à lire le premier bit non encore lu du champ "proch_octet_lire" de la structure stream et decremente nb_bits_restants ******/
/************************** elle modifie dest en le decalant de 1 vers la gauche et y ajouter le bit lu ****************************************************/
/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/

void lecture_bit_of_proch_octet_lire(struct bitstream *stream, uint32_t *dest){
   /* Pas la peine de teste si stream->filename est valide ou on est en fin de stream car la fonction appelante à cette fontion le teste avant de l'applé
    Tout d'abord s'assurer qu'il faut lire un bit du champ "proch_octet_lire" */
  if(stream->nb_bits_restants > 0 && stream->nb_bits_restants <= 8){
    /* lecture du premier bit non encore lu du champ "proch_octet_lire" positionné à l'indice: 8-nb_bits_restants, si on lit de gauche à droit */
    uint8_t octet_courant = stream->tab_octet[stream->indice_courant];
    uint8_t premier_bit_non_lu = (octet_courant >>(stream->nb_bits_restants - 1)) & 1;
    /* Decaler à gauche de 1 *dest " puis y ajouter le premier bit du "proch_octet_lire" non encore lu (on lit de gauche à droite):*/
    /* c'est le bit du poids faible aprés decalage de: stream->nb_bits_restants-1 */
    *dest = ((*dest)<<1) + premier_bit_non_lu;
    /* On decremente le nb_bits_restants */
    stream->nb_bits_restants--;
  }
      // printf(" Tous les bits sont lus !! \n");
      //exit(1);

}

/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/
/************************ La fonction read_bitstream permet de lire des bits et d’avancer dans le flux, on midifiant ***************************************/
/************************************************** le champ proch_octet_lire de la structure bitsream *****************************************************/
/***********************************************************************************************************************************************************/
/***********************************************************************************************************************************************************/

uint8_t read_bitstream(struct bitstream *stream, uint8_t nb_bits, uint32_t *dest, bool discard_byte_stuffing){
  /* Si le stream->filename n'est pas valide ou on est en fin de stream ou trop de bits à lire ou rien à lire */
  // if ((stream->filename == NULL) || (nb_bits > 32) || (nb_bits == 0)){
  //   //printf(" Rien à lire :( !! \n");
  //   exit(1);
  // }
  uint8_t nb_bits_lus = 0;
  /* Aucun bit n'est encore lu et ajouter à dest: *dest = 0 */
  *dest = 0;
  while(nb_bits_lus<nb_bits){
    /* Soit on peut rien lire dans le champ stream->proch_octet_lire */
    if(stream->nb_bits_restants == 0){
      uint8_t octet_lu_prece = stream->tab_octet[stream->indice_courant];
      stream->indice_courant += 1;
      stream->nb_bits_restants = 8;
      uint8_t octet_lu_cour = stream->tab_octet[stream->indice_courant];
      /* faut-il ignorer l'octet_a_recherger ?  */
        if((octet_lu_prece == 0xff) && (octet_lu_cour == 0x00) && discard_byte_stuffing){
            stream->indice_courant += 1;
            stream->nb_bits_restants = 8;
            if(!indice(stream)){
              return nb_bits_lus;
            }
          }
      }
    /* Soit on peut encore lire dans le champ stream->proch_octet_lire, dans ce cas on le lit et on modifie dest */
    lecture_bit_of_proch_octet_lire(stream, dest);
    /* on incremente nb_bits_lus */
    nb_bits_lus++;
  }
  return nb_bits_lus;
}

/***********************************************************************************************************************************************************************************/
/***********************************************************************************************************************************************************************************/
/*************** La fonction end_of_bitstream retourne true si le flux a été entièrement parcouru, false s’il reste des bits à lire ***********************************************/
/***********************************************************************************************************************************************************************************/
/***********************************************************************************************************************************************************************************/


 bool end_of_bitstream(struct bitstream *stream){
  /* Si on arrive à la fin du fichier et que on a tous lu dans le champ stream->proch_octet_lire */
  if( !indice(stream) && (stream->nb_bits_restants == 0)){
    return true;
  }
  return false;
}


void close_bitstream(struct bitstream *stream){
  if(stream != NULL){
    fclose(stream->filename);
    free(stream->tab_octet);
    free(stream);
  }
}

/************************************************************************************************************************************************/
/******************************************** Fonction qui sert à savoir si on est arrive au dernier indice *************************************/
/***********************************************************************************************************************************************/
bool indice(struct bitstream *stream){
  if(stream->indice_courant < stream->size){
    return true;
  }else{
    return false;
  }
}


// int main(int argc, char **argv)
// {
//     if (argc != 2) {
//   /* Si y'a pas au moins un argument en ligne de commandes, on
//    * boude. */
//   fprintf(stderr, "Usage: %s fichier.jpeg\n", argv[0]);
//   return EXIT_FAILURE;
//     }
//     /* On recupere le nom du fichier JPEG sur la ligne de commande. */
//     const char *filename = argv[1];
//     struct bitstream *bst = create_bitstream(filename);
//     printf("%d\n",bst->size );
//     uint32_t *dest = malloc(sizeof(uint32_t));
//     while(indice(bst)){
//       uint8_t n = read_bitstream(bst, 31, dest, false);
//       printf("( %x, %d, %x )", *dest, bst->indice_courant, bst->tab_octet[bst->indice_courant]);
//     }
//     // uint8_t n = read_bitstream(bst, 8, dest, false);
//     // printf("%x, %u\n",*dest, bst->indice_courant );
//     // uint8_t no = read_bitstream(bst, 8, dest, false);
//     // printf("%x, %u\n",*dest, bst->indice_courant );
//     // uint8_t ns = read_bitstream(bst, 8, dest, false);
//     // printf("%x, %u\n",*dest, bst->indice_courant );
//     close_bitstream(bst);
//
//     /* On se congratule. */
//     return EXIT_SUCCESS;
// }
