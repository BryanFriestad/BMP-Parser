#ifndef IMAGE_EDITOR_H_INCLUDED
#define IMAGE_EDITOR_H_INCLUDED

#include "bmp_image.h"

BMP_Image invert_colors(BMP_Image im);
BMP_Image grayscale_colors(BMP_Image im);
BMP_Image encode_message(BMP_Image im, uint8_t* message, int msg_len, int bits_per_comp);

#endif // IMAGE_EDITOR_H_INCLUDED
