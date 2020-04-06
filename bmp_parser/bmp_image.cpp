#include "bmp_image.h"
#include <fstream>
#include "helper_functions.h"

BMP_Image::BMP_Image(char* filename){
    char* bmp = readFileBytes(filename);

    int filesize_offset         = 0x0002;
    int data_offset;
    int info_hdr_size_offset    = 0x000E;
    int image_width_offset      = 0x0012;
    int image_height_offset     = 0x0016;
    int planes_offset           = 0x001A;
    int bits_per_pixel_offset   = 0x001C;
    int compression_type_offset = 0x001E;
    int comp_image_size_offset  = 0x0022;
    int x_pixs_per_meter_offset = 0x0026;
    int y_pixs_per_meter_offset = 0x002A;
    int num_colors_used_offset  = 0x002E;
    int num_imp_colors_offset   = 0x0032;
    int palette_offset          = 0x0036;



}

//makes a new 24 bit depth, no-compression, image with x width and y height, filled with white
BMP_Image::BMP_Image(int x, int y){

}

BMP_Image::~BMP_Image(){

}

void BMP_Image::set_pixel(int x_pos, int y_pos, char r, char g, char b){

}

Generic_BMP_Pixel BMP_Image::get_pixel(int x_pos, int y_pos){

}
