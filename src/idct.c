#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#define PI 3.14159265358979323846


void butterfly(float *x0, float *x1){
   float tmp1 = *x0;
   float tmp2 = *x1;
   *x0 = (tmp1 + tmp2)/2;
   *x1 = (tmp1 - tmp2)/2;
}

void rotator(float k, uint8_t n,float *x0, float *x1){
  float tmp1 = *x0;
  float tmp2 = *x1;
  *x0 = tmp1*(1/k)*cos(n*PI/16) - tmp2*(float)(1/k)*sin(n*PI/16);
  *x1 = tmp2*(1/k)*cos(n*PI/16) + tmp1*(float)(1/k)*sin(n*PI/16);
}
void re_organisation(float *tab1){
  float tab2[8];
  for(uint8_t i = 0; i<8; i++){
    tab2[i] = tab1[i];
  }
  tab1[1] = tab2[4];
  tab1[3] = tab2[6];
  tab1[4] = tab2[7];
  tab1[5] = tab2[3];
  tab1[6] = tab2[5];
  tab1[7] = tab2[1];
}

void idct_bis(float *tab_zz){
  // stage1
  for(uint8_t i = 0; i<8; i++){
    tab_zz[i] = sqrt(8)*tab_zz[i];
  }
  re_organisation(tab_zz);

  butterfly(&tab_zz[7], &tab_zz[4]);
  tab_zz[5] = (float)(1/sqrt(2))*tab_zz[5];
  tab_zz[6] = (float)(1/sqrt(2))*tab_zz[6];


  // stage2
  butterfly(&tab_zz[0], &tab_zz[1]);
  rotator(sqrt(2), 6, &tab_zz[2], &tab_zz[3]);
  butterfly(&tab_zz[4], &tab_zz[6]);
  butterfly(&tab_zz[7], &tab_zz[5]);
  // stage3
  for(uint8_t j = 0; j<2; j++){
    butterfly(&tab_zz[j], &tab_zz[3-j]);
  }
  rotator(1, 3, &tab_zz[4], &tab_zz[7]);
  rotator(1, 1, &tab_zz[5], &tab_zz[6]);
  //stage4
  for(uint8_t j=0; j<4; j++){
    butterfly(&tab_zz[j], &tab_zz[7-j]);
  }
}
void transpose(float **mat, float **mat_tr){
  for(uint8_t i = 0; i<8; i++){
    mat_tr[i] = malloc(8*sizeof(float));
    for(uint8_t j=0; j<8; j++){
      mat_tr[i][j] = mat[j][i];
    }
  }
}
void tab_to_mat(float **mat, uint16_t *tab){
  for(uint8_t i = 0; i<8; i++){
    mat[i] = malloc(sizeof(float) * 8);
    for(uint8_t j = 0; j<8; j++){
      mat[i][j] = 1.0*(int16_t)tab[8*i + j];
    }
  }
}
uint8_t sature(float a){
  return ((a > 0) && (a<255))*a + 255*((a>255)||(a == 255));
}
void stages(uint16_t *tab_zz, uint8_t *tab_idct){
  float **mat = malloc(8*sizeof(float *));
  tab_to_mat(mat, tab_zz);
  for(uint8_t i = 0; i<8; i++){
    idct_bis(mat[i]);
  }
  float **mat_tr = malloc(8*sizeof(float *));
  transpose(mat, mat_tr);

  for(uint8_t i = 0; i<8; i++){
    idct_bis(mat_tr[i]);
  }
  mat = calloc(8,sizeof(float *));
  transpose(mat_tr, mat);
  for (uint8_t i=0; i<8;i++){
    free(mat_tr[i]);
  }
  free(mat_tr);
  for(uint8_t i = 0; i<8; i++){
    for(uint8_t j = 0; j<8; j++){
      tab_idct[8*i+j] =sature(round((mat[i][j]+128.0)));
    }
  }
  for (uint8_t i=0; i<8;i++){
    free(mat[i]);
  }
  free(mat);
}
