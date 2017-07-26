#ifndef MANDELCALCHEADERDEF
#define MANDELCALHEADERDEF

KOKKOS_INLINE_FUNCTION int MandelCalcBW(float x, float y);

int MandelCalcBW(float x, float y){
  int iter = 0;
  int iter_max = 1000;
  float rad = 0, zx = 0, zy = 0, rad_max = 2.0;

  while(rad < rad_max && iter < iter_max){
    float zx_new = zx*zx +zy*zy + x;
    zy = 2*zx*zy +y;
    zx = zx_new;
    rad = sqrt(zx*zx +zy*zy);
    iter++;   
  }//end while

  int val = 0;
  if (iter < iter_max) {
    //this pixel is in the mandelbrot set
    val = 255;
  }
  return(val);
 
}//end MandelCalcBW

#endif
