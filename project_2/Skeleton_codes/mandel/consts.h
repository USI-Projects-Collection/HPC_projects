#ifndef CONSTS_H_
#define CONSTS_H_

// maximum number of iterations
#define MAX_ITERS 10000

// image size
#ifndef IMAGE_WIDTH
#define IMAGE_WIDTH  4096
#endif

#ifndef IMAGE_HEIGHT
#define IMAGE_HEIGHT 4096
#endif

// the extent of the parameter plane ( MIN_X + iMIN_Y <= c < MAX_X + iMAX_Y )
#define MIN_X -2.1
#define MAX_X  0.7
#define MIN_Y -1.4
#define MAX_Y  1.4

#endif /* CONSTS_H_ */
