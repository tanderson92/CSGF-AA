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
  MPI_Init(&argc, &argv);

  // Initialize Kokkos
  Kokkos::initialize(argc, argv);

  int pixelcountx = 128;
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

  std::ofstream image("mandelbrot.pgm", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (image.is_open()) {
    image << "P5\n";
    image << pixelcountx << " " << pixelcounty << "\n";
    image << 255 << "\n";
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
