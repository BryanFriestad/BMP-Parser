#include "bmp_pixel.h"

Generic_BMP_Pixel::Generic_BMP_Pixel(char p_depth){
    pixel_depth = p_depth;
}

BMP_Pixel_24_bit::BMP_Pixel_24_bit() : Generic_BMP_Pixel(8){
    red = 0;
    green = 0;
    blue = 0;
}

BMP_Pixel_24_bit::BMP_Pixel_24_bit(char r, char g, char b) : Generic_BMP_Pixel(8){
    red = r;
    green = g;
    blue = b;
}

void BMP_Pixel_24_bit::set_color(char r, char g, char b){
    red = r;
    green = g;
    blue = b;
}
