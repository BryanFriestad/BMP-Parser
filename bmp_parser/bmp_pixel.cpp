#include "bmp_pixel.h"

Generic_BMP_Pixel::Generic_BMP_Pixel(unsigned char p_depth){
    pixel_depth = p_depth;
}

BMP_Pixel_24_bit::BMP_Pixel_24_bit() : Generic_BMP_Pixel(8){
    red = 0;
    green = 0;
    blue = 0;
}

BMP_Pixel_24_bit::BMP_Pixel_24_bit(unsigned char r, unsigned char g, unsigned char b) : Generic_BMP_Pixel(8){
    red = r;
    green = g;
    blue = b;
}

void BMP_Pixel_24_bit::set_color(unsigned char r, unsigned char g, unsigned char b){
    red = r;
    green = g;
    blue = b;
}
