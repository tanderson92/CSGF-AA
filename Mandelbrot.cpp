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
  MPI_Init(&argc, &argv);

  // Initialize Kokkos
  Kokkos::initialize(argc, argv);

  int pixelcountx = 4096;
  float centerx = -0.75;
  float centery =  0.00;
  float lengthx = 2.75;
  float lengthy = 2.00;
  float pixelsize = lengthx/pixelcountx;
  int pixelcounty = trunc(lengthy/pixelsize);

  float minx = centerx - lengthx/2.0;
  float maxy = centery + lengthy/2.0;
  Kokkos::View<unsigned char**> pixels("pixels", pixelcountx, pixelcounty);

  // Create host mirror of pixels
  auto  pixels_mirror = Kokkos::create_mirror_view(pixels);

  using range2d_t = Kokkos::Experimental::MDRangePolicy<Kokkos::Experimental::Rank<2>, Kokkos::IndexType<int>>;
  range2d_t range( {0, 0}, {pixelcountx, pixelcounty} );
  Kokkos::Experimental::md_parallel_for(range, KOKKOS_LAMBDA(int i, int j){
      float x = minx + i*pixelsize;
      float y = maxy - j*pixelsize;
      pixels(i, j) = MandelCalcBW(x, y);
  });
  deep_copy(pixels_mirror, pixels);

  std::ofstream image;
  image.open("mandelbrot.pgm");
  if (image.is_open()) {
    image << "P5" << std::endl;
    image << pixelcountx << " " << pixelcounty << std::endl;
    image << 255 << std::endl;
    for (int j = 0; j < pixelcounty; j++) {
      for (int i = 0; i < pixelcountx; i++) {
        image << pixels_mirror(i, j);
      }
    }
    image.close();
  }
  Kokkos::finalize();

  MPI_Finalize();
  return 0;
}
