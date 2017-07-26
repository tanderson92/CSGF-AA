#ifndef MANDELCALCHEADERDEF
#define MANDELCALHEADERDEF

#function definitions
int MandelCalcBW(int x, int y);


int MandelCalcBW(int x, int y){
  int iter = 0;
  int iter_max = 1000;
  float rad = 0, zx = 0, zy = 0, rad_max = 2.0;

  while(rad < rad_max && iter < iter_max){
    zx_new = zx*zx +zy*zy + x;
    zy = 2*zx*zy +y;
    nx = zx_new;
    rad = sqrt(zx*zx +zy*zy);
    iter++;   
  }//end while

  val = 0;
  if iter < iter_max{
    //this pixel is in the mandelbrot set
    val = 1;
  }
  return(val);
 
}//end MandelCalcBW

#endif
