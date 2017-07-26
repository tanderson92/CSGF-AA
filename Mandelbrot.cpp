#include <iostream>
#include <fstream>
#include <Kokkos_Core.hpp>
#include <Kokkos_Parallel.hpp>
#include <Kokkos_View.hpp>
#include "mpi.h"
#include <assert.h>
#include <limits> 
#include "Mandelcalc.hpp"

int main(int argc, char **argv) {
/*  // Initialize MPI before Kokkos
  MPI_Init(&argc, &argv);

  // Initialize Kokkos
  Kokkos::initialize(argc, argv);

  const int dim = 1000;

  // Allocate our arrays
  Kokkos::View<double*> a("a", dim);    
  Kokkos::View<double*> b("b", dim);
  Kokkos::View<double*> c("c", dim);

  // Create host mirror of C
  auto  c_mirror = Kokkos::create_mirror_view(c);

  // Initialize a and b
  Kokkos::parallel_for(dim, KOKKOS_LAMBDA(int i) {
      a(i) = sin((double)i)*sin((double)i);
      b(i) = cos((double)i)*cos((double)i);    
  });
 
  // Preform the vector addition
  Kokkos::parallel_for(dim, KOKKOS_LAMBDA(int i) {
    c(i) = a(i) + b(i);
  });

  // Update the mirror
  deep_copy(c_mirror, c);

  // Verify the results of the host mirror
  for(int i=0; i<dim; i++) {
    double eps = abs(1.0 - c_mirror(i));
    assert(eps <= std::numeric_limits<double>::epsilon());    
  };

  std::cout<<"Sum Correct\n";

  Kokkos::finalize();

  MPI_Finalize();
*/

  int pixelcountx = 8192;
  float centerx = -0.75;
  float centery =  0.00;
  float lengthx = 2.75;
  float lengthy = 2.00;
  float pixelsize = lengthx/pixelcountx;
  int pixelcounty = trunc(lengthy/pixelsize);

  float minx = centerx - lengthx/2.0;
  float maxy = centery + lengthy/2.0;
  unsigned char *pixels = new unsigned char[pixelcounty*pixelcountx];

  for (int pixely = 0; pixely < pixelcounty; pixely++) {
    for (int pixelx = 0; pixelx < pixelcountx; pixelx++) {
      float x = minx + pixelx*pixelsize;
      float y = maxy - pixely*pixelsize;
      int i = pixely * pixelcountx + pixelx;
      pixels[i] = MandelCalcBW(x, y);
    }
  }

  std::ofstream image;
  image.open("mandelbrot.pgm");
  if (image.is_open()) {
    image << "P5" << std::endl;
    image << pixelcountx << " " << pixelcounty << std::endl;
    image << 255 << std::endl;
    image.close();
  }
  return 0;
}
