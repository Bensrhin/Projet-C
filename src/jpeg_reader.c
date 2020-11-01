#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
// #include "jpeg_reader.h"
#include "../include/bitstream.h"
#include "../include/huffman.h"
#include "../include/jpeg_reader.h"



struct section_app0{
  uint32_t offset_app0;
  uint32_t longueur;
  uint32_t *donnee;
};

struct section_DHT{
  uint32_t offset_DHT;
  uint32_t longueur_DHT;
  uint32_t indice_DC_AC;
  uint32_t indice;
  struct huff_table **tab_DC;
  struct huff_table **tab_AC;
};

struct section_DQT{
  uint32_t offset_DQT;
  uint32_t longueur_DQT;
  uint32_t precision;
  uint32_t indiceiq;
  uint8_t **tab_quant;
  uint8_t nb_quant_tables;
};
struct section_SOF0{
  uint32_t offset_sof;
  uint32_t longueur_sof;
  uint32_t precision;
  uint32_t hauteur;
  uint32_t largeur;
  uint32_t nombre_de_composante;
  uint32_t ic_y;
  uint32_t ic_cb;
  uint32_t ic_cr;
  uint32_t fact_sampling_y_ver;
  uint32_t fact_sampling_y_hor;
  uint32_t fact_sampling_cr_ver;
  uint32_t fact_sampling_cr_hor;
  uint32_t fact_sampling_cb_ver;
  uint32_t fact_sampling_cb_hor;
  uint32_t iq_y;
  uint32_t iq_cb;
  uint32_t iq_cr;
};
struct section_sos{
  uint32_t longueur_sos;
  uint8_t *ic;
  uint8_t *ih_dc;
  uint8_t *ih_ac;
};
struct jpeg_desc{
  const char *filename;
  struct section_app0 *s_app0;
  struct section_DQT *s_dqt;
  struct section_SOF0 *s_sof;
  struct section_DHT *s_dht;
  struct bitstream *stream;
  struct section_sos *s_sos;
  uint32_t offset_sos;
};

struct jpeg_desc *read_jpeg ( const char *filename ){
  struct section_app0 *s_app0 = malloc(sizeof(struct section_app0));
  struct section_DQT *s_dqt = malloc(sizeof(struct section_DQT));
  struct section_SOF0 *s_sof = malloc(sizeof(struct section_SOF0));
  struct section_DHT *s_dht = malloc(sizeof(struct section_DHT));
  struct jpeg_desc *jdesc = malloc(sizeof(struct jpeg_desc));
  jdesc -> filename = filename;
  s_dqt -> tab_quant = malloc(4*sizeof(uint8_t *));
  s_dht -> tab_DC = malloc(4*sizeof(struct huff_table *));
  s_dht -> tab_AC = malloc(4*sizeof(struct huff_table *));
  struct bitstream *stream = create_bitstream(filename);
  uint32_t byte = 0;
  read_bitstream(stream, 16, &byte, false);
  read_bitstream(stream, 16, &(s_app0 -> offset_app0), false);
  read_bitstream(stream, 16, &(s_app0 -> longueur), false);
  // lecture de jfif
  read_bitstream(stream, 32, &byte, false);
  read_bitstream(stream, 8, &byte, false);
  // nombre d oct restants dans la section app0
  uint8_t res = s_app0 -> longueur -5 - 2;
  (s_app0-> donnee) = malloc(sizeof(uint32_t) * res);
  for(uint8_t i = 0; i<res; i++){
    read_bitstream(stream, 8, &(s_app0-> donnee)[i], false);
  }
  // lecture du prochain en-tete
  read_bitstream(stream, 16, &byte, false);
  // si byte est l en-tete d une section commentaire
  if(byte == 0xfffe){
      read_bitstream(stream, 16, &byte, false);
      uint32_t cpt = 0;
      for(uint8_t i = 0; i<byte-2; i++){
          read_bitstream(stream, 8, &cpt, false);
      }
        read_bitstream(stream, 16, &byte, false);
  }
  // lecture de l entete dqt
  while(byte == 65499){
    s_dqt -> offset_DQT = byte;
    read_bitstream(stream, 16, &(s_dqt -> longueur_DQT), false);
    read_bitstream(stream, 4, &(s_dqt -> precision), false);
    read_bitstream(stream, 4, &(s_dqt -> indiceiq), false);
    res = s_dqt -> longueur_DQT - 2 - 1;
    s_dqt -> tab_quant[s_dqt -> indiceiq] = malloc(sizeof(uint32_t)*res);
    for(uint8_t k = 0; k<res; k++){
        read_bitstream(stream, 8, &byte, false);
        (s_dqt -> tab_quant)[s_dqt -> indiceiq][k] = byte;
    }
    s_dqt -> nb_quant_tables += 1;
    read_bitstream(stream, 16, &byte, false);
  }
  // section sof
  s_sof -> offset_sof = byte;
  read_bitstream(stream, 16, &(s_sof -> longueur_sof), false);
  read_bitstream(stream, 8, &(s_sof -> precision), false);
  read_bitstream(stream, 16, &(s_sof -> hauteur), false);
  read_bitstream(stream, 16, &(s_sof -> largeur), false);
  read_bitstream(stream, 8, &(s_sof -> nombre_de_composante), false);
  if(s_sof -> nombre_de_composante == 1){
     read_bitstream(stream, 8, &(s_sof -> ic_y), false);
     read_bitstream(stream, 4, &(s_sof -> fact_sampling_y_hor), false);
     read_bitstream(stream, 4, &(s_sof -> fact_sampling_y_ver), false);
     read_bitstream(stream, 8, &(s_sof -> iq_y), false);
   }
  else{
    read_bitstream(stream, 8, &(s_sof -> ic_y), false);
    read_bitstream(stream, 4, &(s_sof -> fact_sampling_y_hor), false);
    read_bitstream(stream, 4, &(s_sof -> fact_sampling_y_ver), false);
    read_bitstream(stream, 8, &(s_sof -> iq_y), false);
    read_bitstream(stream, 8, &(s_sof -> ic_cb), false);
    read_bitstream(stream, 4, &(s_sof -> fact_sampling_cb_hor), false);
    read_bitstream(stream, 4, &(s_sof -> fact_sampling_cb_ver), false);
    read_bitstream(stream, 8, &(s_sof -> iq_cb), false);
    read_bitstream(stream, 8, &(s_sof -> ic_cr), false);
    read_bitstream(stream, 4, &(s_sof -> fact_sampling_cr_hor), false);
    read_bitstream(stream, 4, &(s_sof -> fact_sampling_cr_ver), false);
    read_bitstream(stream, 8, &(s_sof -> iq_cr), false);
  }
  read_bitstream(stream, 16, &byte, false);
  // section dht
  while(byte == 65476){
    // printf("%u\n", byte);
    s_dht -> offset_DHT = byte;
    read_bitstream(stream, 16, &(s_dht -> longueur_DHT), false);
    read_bitstream(stream, 3, &byte, false);
    uint16_t nb_bytes_read = 0;
    if(byte != 0){
      fprintf(stderr,"erreur les 3 premiers bites sont != 0\n");
      return NULL;
    }
    read_bitstream(stream, 1, &(s_dht -> indice_DC_AC), false);
    // si indice_DC_AC = 0 on remplit la table DC sinon la table AC
    if(s_dht -> indice_DC_AC == 0){
       read_bitstream(stream, 4, &(s_dht -> indice), false);
       (s_dht -> tab_DC)[s_dht -> indice] = load_huffman_table(stream, &nb_bytes_read);
       read_bitstream(stream, 16, &byte, false);
     }
    else{
       read_bitstream(stream, 4, &(s_dht -> indice), false);
       (s_dht -> tab_AC)[s_dht -> indice] = load_huffman_table(stream, &nb_bytes_read);
       read_bitstream(stream, 16, &byte, false);
     }
  }
  // section sos
  jdesc -> offset_sos = byte;
  struct section_sos *s_sos = malloc(sizeof(struct section_sos));
  read_bitstream(stream, 16, &(s_sos -> longueur_sos), false);
  read_bitstream(stream, 8, &byte, false);
  s_sos -> ic = malloc(sizeof(uint8_t)*(s_sof -> nombre_de_composante));
  s_sos -> ih_dc = malloc(sizeof(uint8_t)*s_sof -> nombre_de_composante);
  s_sos -> ih_ac = malloc(sizeof(uint8_t)*s_sof -> nombre_de_composante);
  for(uint8_t k = 0; k<(s_sof -> nombre_de_composante); k++){
    read_bitstream(stream, 8, &byte, false);
    (s_sos -> ic)[k] = byte;
    read_bitstream(stream, 4, &byte, false);
    (s_sos -> ih_dc)[k] = byte;
    read_bitstream(stream, 4, &byte, false);
    (s_sos -> ih_ac)[k] = byte;
    byte = 0;
  }
  jdesc -> s_sos = s_sos;
  jdesc -> s_sof = s_sof;
  jdesc -> s_app0 = s_app0;
  jdesc -> s_dqt = s_dqt;
  jdesc -> s_dht = s_dht;
  jdesc -> stream = stream;
  return jdesc;
}

uint16_t get_image_size ( struct jpeg_desc *jpeg , enum direction dir){
    switch(dir){
      case 0:
         return (jpeg -> s_sof) -> largeur;
      case 1:
         return (jpeg -> s_sof) -> hauteur;
      default:
         printf("erreur\n");
         return EXIT_FAILURE;
    }
}

uint8_t get_nb_components ( const struct jpeg_desc *jpeg){
  return (jpeg -> s_sof) -> nombre_de_composante;
}

struct bitstream *get_bitstream (const struct jpeg_desc *jpeg){
  uint32_t byte = 0;
  for(uint8_t i = 0; i<3; i++){
      read_bitstream(jpeg->stream, 8, &byte, false);
}
  return jpeg->stream;
}
struct huff_table * get_huffman_table ( const struct jpeg_desc *jpeg ,
       enum acdc acdc, uint8_t index){
      switch(acdc){
        case 0:
           return ((jpeg -> s_dht) -> tab_DC)[index];
        case 1:
           return ((jpeg -> s_dht) -> tab_AC)[index];
        default:
           fprintf(stderr,"erreur acdc est entre 0 et 1\n");
           return NULL;
      }
  }

uint8_t *get_quantization_table (const struct jpeg_desc *jpeg ,uint8_t index ){
    return ((jpeg-> s_dqt)->tab_quant)[index];
  }

uint8_t get_nb_quantization_tables ( const struct jpeg_desc *jpeg){
  return (jpeg->s_dqt) -> nb_quant_tables;
}

uint8_t get_frame_component_sampling_factor (const struct jpeg_desc *jpeg ,enum direction dir ,
                  uint8_t frame_comp_index ){
  switch(dir){
    case 0:
       if(frame_comp_index == 0){
         return (jpeg -> s_sof) -> fact_sampling_y_hor;
       }
       else if(frame_comp_index == 1){
         return (jpeg -> s_sof) -> fact_sampling_cb_hor;
       }
       else{
          return (jpeg -> s_sof) -> fact_sampling_cr_hor;
       }
    case 1:
       if(frame_comp_index == 0){
           return (jpeg -> s_sof) -> fact_sampling_y_ver;
        }
      else if(frame_comp_index == 1){
         return (jpeg -> s_sof) -> fact_sampling_cb_ver;
      }
      else{
          return (jpeg -> s_sof) -> fact_sampling_cr_ver;
      }
    default:
       fprintf(stderr,"erreur dir est entre 0 et 1\n");
       return EXIT_FAILURE;
  }
}

uint8_t get_frame_component_id ( const struct jpeg_desc *jpeg ,
             uint8_t frame_comp_index ){
   if(frame_comp_index == 0){
     return (jpeg -> s_sof) -> ic_y;
   }
   else if(frame_comp_index == 1){
     return (jpeg->s_sof) -> ic_cb;
   }
   else{
     return (jpeg -> s_sof) -> ic_cr;
   }
}
uint8_t get_frame_component_quant_index( const struct jpeg_desc *jpeg ,
             uint8_t frame_comp_index ){
   if(frame_comp_index == 0){
     return (jpeg -> s_sof) -> iq_y;
   }
   else if(frame_comp_index == 1){
     return (jpeg->s_sof) -> iq_cb;
   }
   else{
     return (jpeg -> s_sof) -> iq_cr;
   }
}

uint8_t get_scan_component_huffman_index(const struct jpeg_desc *jpeg, enum acdc acdc, uint8_t scan_comp_index){
  switch(acdc){
    case 0:
         return ((jpeg -> s_sos) -> ih_dc)[scan_comp_index];
    case 1:
         return ((jpeg -> s_sos) -> ih_ac)[scan_comp_index];
    default:
        fprintf(stderr,"erreur acdc est entre 0 et 1\n");
        return EXIT_FAILURE;

  }
}
char *get_filename(const struct jpeg_desc *jpeg){
  return (char *)jpeg -> filename;
}
void close_dht(struct jpeg_desc *jpeg){
  for(uint8_t i = 0; i<4; i++){
    if( ((jpeg->s_dht) -> tab_DC)[i] != NULL){
       free_huffman_table(((jpeg->s_dht) -> tab_DC)[i]);
    }

    if(((jpeg->s_dht) -> tab_AC)[i] != NULL){
       free_huffman_table(((jpeg->s_dht) -> tab_AC)[i]);
    }
  }
  free((jpeg->s_dht) -> tab_DC);
  free((jpeg->s_dht) -> tab_AC);
  free(jpeg -> s_dht);
}

void close_dqt(struct jpeg_desc *jpeg){
  for(uint8_t i = 0; i<4; i++){
     if(((jpeg->s_dqt) -> tab_quant)[i] != NULL){
        free(((jpeg -> s_dqt)->tab_quant)[i]);
     }
   }
   free((jpeg -> s_dqt) -> tab_quant);
   free(jpeg -> s_dqt);
}
void close_app0(struct jpeg_desc *jpeg){
  free((jpeg -> s_app0)->donnee);
  free(jpeg -> s_app0);
}
void close_sof(struct jpeg_desc *jpeg){
  free(jpeg -> s_sof);
}
void close_sos(struct jpeg_desc *jpeg){
  free((jpeg -> s_sos) -> ic);
  free((jpeg -> s_sos) -> ih_dc);
  free((jpeg -> s_sos) -> ih_ac);
  free(jpeg -> s_sos);
}


void close_jpeg(struct jpeg_desc *jpeg){
  close_app0(jpeg);
  close_dqt(jpeg);
  close_sof(jpeg);
  close_dht(jpeg);
  close_bitstream(jpeg->stream);
  close_sos(jpeg);
  free(jpeg);
}
