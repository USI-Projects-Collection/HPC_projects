#include <stdio.h>
#include <stdlib.h>

#include "consts.h"
#include "pngwriter.h"
#include "walltime.h"

int main(int argc, char **argv) {
  char output_name[256];
  if (argc > 1) {
    snprintf(output_name, sizeof(output_name), "%s", argv[1]);
  } else {
    snprintf(output_name, sizeof(output_name), "mandel.png");
  }
  png_data *pPng = png_create(IMAGE_WIDTH, IMAGE_HEIGHT);

  double x, y, x2, y2, cx, cy;

  double fDeltaX = (MAX_X - MIN_X) / (double)IMAGE_WIDTH;
  double fDeltaY = (MAX_Y - MIN_Y) / (double)IMAGE_HEIGHT;

  long nTotalIterationsCount = 0;
  long total_pixels = (long)IMAGE_WIDTH * (long)IMAGE_HEIGHT;
  int *colors = (int *)malloc(sizeof(int) * total_pixels);
  if (!colors) {
    fprintf(stderr, "Failed to allocate color buffer\n");
    return 1;
  }

  long i, j;

  double time_start = walltime();
  // do the calculation
  cy = MIN_Y;
  for (j = 0; j < IMAGE_HEIGHT; j++) {
#pragma omp parallel for schedule(static) reduction(+ : nTotalIterationsCount) private(x, y, x2, y2, cx)
    for (i = 0; i < IMAGE_WIDTH; i++) {
      cx = MIN_X + i * fDeltaX;
      x = cx;
      y = cy;
      x2 = x * x;
      y2 = y * y;
      // compute the orbit z, f(z), f^2(z), f^3(z), ...
      // count the iterations until the orbit leaves the circle |z|=2.
      // stop if the number of iterations exceeds the bound MAX_ITERS.
      int n = 0;
      while ((x2 + y2 <= 4.0) && (n < MAX_ITERS)) {
        double xy = x * y;
        y = 2.0 * xy + cy;
        x = x2 - y2 + cx;
        x2 = x * x;
        y2 = y * y;
        n++;
      }
      nTotalIterationsCount += n;
      // n indicates if the point belongs to the mandelbrot set
      // plot the number of iterations at point (i, j)
      int c = ((long)n * 255) / MAX_ITERS;
      colors[j * IMAGE_WIDTH + i] = c;
    }
    cy += fDeltaY;
  }
  double time_end = walltime();

  // print benchmark data
  printf("Total time:                 %g seconds\n",
         (time_end - time_start));
  printf("Image size:                 %ld x %ld = %ld Pixels\n",
         (long)IMAGE_WIDTH, (long)IMAGE_HEIGHT,
         (long)(IMAGE_WIDTH * IMAGE_HEIGHT));
  printf("Total number of iterations: %ld\n", nTotalIterationsCount);
  printf("Avg. time per pixel:        %g seconds\n",
         (time_end - time_start) / (double)(IMAGE_WIDTH * IMAGE_HEIGHT));
  printf("Avg. time per iteration:    %g seconds\n",
         (time_end - time_start) / (double)nTotalIterationsCount);
  printf("Iterations/second:          %g\n",
         nTotalIterationsCount / (time_end - time_start));
  // assume there are 8 floating point operations per iteration
  printf("MFlop/s:                    %g\n",
         nTotalIterationsCount * 8.0 / (time_end - time_start) * 1.e-6);

  for (j = 0; j < IMAGE_HEIGHT; j++) {
    for (i = 0; i < IMAGE_WIDTH; i++) {
      int c = colors[j * IMAGE_WIDTH + i];
      png_plot(pPng, i, j, c, c, c);
    }
  }

  free(colors);

  png_write(pPng, output_name);
  return 0;
}
