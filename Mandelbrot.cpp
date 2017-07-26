#include <iostream>
#include <fstream>
#include <Kokkos_Core.hpp>
#include <Kokkos_Parallel.hpp>
#include <Kokkos_View.hpp>
#include "mpi.h"
#include <assert.h>
#include <limits> 
#include "Mandelcalc.hpp"

//#define USE_COLOR

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  // Initialize Kokkos
  Kokkos::initialize(argc, argv);

  int pixelcountx = 32768;
  float centerx = -0.75;
  float centery =  0.00;
  float lengthx = 2.75;
  float lengthy = 2.00;
  float pixelsize = lengthx/pixelcountx;
  int pixelcounty = trunc(lengthy/pixelsize);

  float minx = centerx - lengthx/2.0;
  float maxy = centery + lengthy/2.0;
#ifdef USE_COLOR
  Kokkos::View<unsigned char**> pixelsR("pixelsR", pixelcountx, pixelcounty);
  Kokkos::View<unsigned char**> pixelsG("pixelsG", pixelcountx, pixelcounty);
  Kokkos::View<unsigned char**> pixelsB("pixelsB", pixelcountx, pixelcounty);
  auto pixelsR_mirror = Kokkos::create_mirror_view(pixelsR);
  auto pixelsG_mirror = Kokkos::create_mirror_view(pixelsG);
  auto pixelsB_mirror = Kokkos::create_mirror_view(pixelsB);
#else
  Kokkos::View<unsigned char**> pixels("pixels", pixelcountx, pixelcounty);
  // Create host mirror of pixels
  auto  pixels_mirror = Kokkos::create_mirror_view(pixels);
#endif

  using range2d_t = Kokkos::Experimental::MDRangePolicy<Kokkos::Experimental::Rank<2>, Kokkos::IndexType<int>>;
  range2d_t range( {0, 0}, {pixelcountx, pixelcounty} );
  
  Kokkos::Timer timer;
  Kokkos::Experimental::md_parallel_for(range, KOKKOS_LAMBDA(int i, int j){
      float x = minx + i*pixelsize;
      float y = maxy - j*pixelsize;
#ifdef USE_COLOR
      // TODO      
#else
      // Strict BW, no grey
      //pixels(i, j) = MandelCalcBW(x, y);
      // Greyscale using distance function
      pixels(i, j) = MandelCalcDist(x, y, pixelsize);
#endif
  });
#ifdef USE_COLOR
  deep_copy(pixelsR_mirror, pixelsR);
  deep_copy(pixelsG_mirror, pixelsG);
  deep_copy(pixelsB_mirror, pixelsB);
#else
  deep_copy(pixels_mirror, pixels);
#endif
  double elapsed_seconds = timer.seconds();
  std::cout << "parallel_for time: " << elapsed_seconds <<" seconds\n";

#ifdef USE_COLOR
  std::ofstream image("mandelbrot.ppm", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
#else
  std::ofstream image("mandelbrot.pgm", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
#endif
  if (image.is_open()) {
#ifdef USE_COLOR
    image << "P6\n";
#else
    image << "P5\n";
#endif
    image << pixelcountx << " " << pixelcounty << "\n";
    image << 255 << "\n";
    for (int j = 0; j < pixelcounty; j++) {
      for (int i = 0; i < pixelcountx; i++) {
#ifdef USE_COLOR
        image << pixelsR_mirror(i, j) << pixelsG_mirror(i, j) << pixelsB_mirror(i, j);
#else
        image << pixels_mirror(i, j);
#endif
      }
    }
    image.close();
  }
  Kokkos::finalize();

  MPI_Finalize();
  return 0;
}
