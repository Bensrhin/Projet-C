#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#define PI 3.14159265358979323846

extern void butterfly(float *x0, float *x1);
extern void rotator(float k, uint8_t n,float *x0, float *x1);
extern void re_organisation(float *tab1);
extern void idct_bis(float *tab_zz);
extern void transpose(float **mat, float **mat_tr);
extern void tab_to_mat(float **mat, uint16_t *tab);
extern uint8_t sature(float a);
extern void stages(uint16_t *tab_zz, uint8_t *tab_idct);
