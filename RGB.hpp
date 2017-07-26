#ifndef RGBHEADERDEF
#define RGBHEADERDEF

KOKKOS_INLINE_FUNCTION void RGB(float H, float S, float V, int* R, int* G, int* B);

void RGB(float H, float S, float V, int* R, int* G, int* B){
  float C = V*S;
  float Hp = H/60.0;
  
  float X = C*(1-abs(fmod(Hp,2.0f) - 1));
  
  if(0 <= Hp && Hp <= 1){
    *R = C;
    *G = X;
    *B = 0;
  }
  else{
    if(1 <= Hp && Hp <= 2){
      *R = X;
      *G = C;
      *B = 0;
    }
    else{
      if(2 <= Hp && Hp <= 3){
        *R = 0;
	*G = C;
	*B = X;
      }
      else{
	if(3 <= Hp && Hp <= 4){
	  *R = 0;
	  *G = X;
	  *B = C;
	}
        else{
	  if(4 <= Hp && Hp <= 5){
	    *R = X;
	    *G = 0;
	    *B = C;
	  }
          else{
	    if(5 <= Hp && Hp <= 6){
	      *R = C;
	      *G = 0;
	      *B = X;
	    }
          }
        }
      }
    }
  }
  
  float m = V - C;
  *R += m;
  *G += m;
  *B += m; 
}//end RGB

#endif
