#ifndef MANDELCALCHEADERDEF
#define MANDELCALHEADERDEF

#include "RGB.hpp"

KOKKOS_INLINE_FUNCTION int MandelCalcBW(float x, float y);
KOKKOS_INLINE_FUNCTION int MandelCalcDist(float x, float y, float pixelsize);
KOKKOS_INLINE_FUNCTION int Color(float distance, float pixelsize);
KOKKOS_INLINE_FUNCTION void MandelCalcHSV(float x, float y, float pixelsize, float* H, float* S, float* V);
KOKKOS_INLINE_FUNCTION float HSV(float distance, float pixelsize);

int MandelCalcBW(float x, float y){
  int iter = 0;
  int iter_max = 1000;
  float rad = 0, zx = 0, zy = 0, rad_max = 2.0;

  while(rad < rad_max && iter < iter_max){
    float zx_new = zx*zx - zy*zy + x;
    zy = 2*zx*zy + y;
    zx = zx_new;
    rad = sqrt(zx*zx + zy*zy);
    iter++;
  }//end while

  int val = 0;
  if (iter < iter_max) {
    //this pixel is not in the mandelbrot set
    val = 255;
  }
  return(val);
 
}//end MandelCalcBW

int MandelCalcDist(float x, float y, float pixelsize){
  int iter = 0;
  int iter_max = 10000;
  float rad = 0, zx = 0, zy = 0, rad_max = 1 << 18,  distance = 0, dzx = 0, dzy = 0;

  while(rad < rad_max && iter < iter_max){
    float dzx_new = 2*zx*dzx - 2*zy*dzy + 1;
    dzy = 2*(zx*dzy + zy*dzx);
    dzx = dzx_new;

    float zx_new = zx*zx - zy*zy + x;
    zy = 2*zx*zy + y;
    zx = zx_new;
    rad = sqrt(zx*zx + zy*zy);
    iter++;
  }//end while                                                                                                                                   
  float dz_mag = sqrt(dzx*dzx + dzy*dzy);
  distance = 2*log(rad)*rad/dz_mag;

  int val = Color(distance, pixelsize);
  return(val);

}//end MandelCalcDist

int Color(float distance, float pixelsize){
  if (distance < pixelsize/2.0)
    return round(pow(2.0*distance/pixelsize, 1/3.0) * 255);
  else
    return 255;
}//end Color

void MandelCalcHSV(float x, float y, float pixelsize, int* R, int* G, int* B){
  float H;
  float S; 
  float V;
  int iter = 0;
  int iter_max = 10000;
  float rad = 0, zx = 0, zy = 0, rad_max = 1 << 18,  distance = 0, dzx = 0, dzy = 0;

  while(rad < rad_max && iter < iter_max){
    float dzx_new = 2*zx*dzx - 2*zy*dzy + 1;
    dzy = 2*(zx*dzy + zy*dzx);
    dzx = dzx_new;

    float zx_new = zx*zx - zy*zy + x;
    zy = 2*zx*zy + y;
    zx = zx_new;
    rad = sqrt(zx*zx + zy*zy);
    iter++;
  }//end while                                                                                                                                 
                                                                                                                                                
  float dz_mag = sqrt(dzx*dzx + dzy*dzy);
  distance = 2*log(rad)*rad/dz_mag;
  if(iter < iter_max){
    V = HSV(distance, pixelsize);
    S = 0.7;
    H = log((float) iter)/log((float) iter_max)*10;
    H = H - floor(H);
  }
  else{
    //return white
    H = 0;
    S = 0;
    V = 1;
  }

  RGB(H, S, V, R, G, B);

}//end MandelCalcHSV  

float HSV(float distance, float pixelsize){
  float V = 0;
  if (distance < pixelsize/2.0)
   V = pow(2.0*distance/pixelsize, 1/3.0);
  else
    V = 1;

  return(V);

}//end HSV

#endif
