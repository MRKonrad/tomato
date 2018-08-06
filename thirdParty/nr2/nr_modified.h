/*!
 * \file nr_modified.h
 * \author Konrad Werys
 * Where I put all the needed NR2 function definitions. I can't use the whole nr.h, because it collides with math.h
 * \date 2018/08/03
 */

#ifndef OXSHMOLLI2_NR_MODIFIED_H
#define OXSHMOLLI2_NR_MODIFIED_H

void amoeba(float **p, float y[], int ndim, float ftol, float (*funk)(float []), int *iter);
float amotry(float **p, float y[], float psum[], int ndim, float (*funk)(float []), int ihi, float fac);
float bessj0(float x);

#endif //OXSHMOLLI2_NR_MODIFIED_H
