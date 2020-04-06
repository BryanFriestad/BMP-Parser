#include "bmp_image.h"
#include "bmp_pixel.h"
#include "helper_functions.h"
#include "debug_settings.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <memory>
#include <stdio.h>

BMP_Image::BMP_Image(char* filename){
    char* bmp = readFileBytes(filename);

    if(bmp[0] == 'B' and bmp[1] == 'M'){
        if(DEBUG_LEVEL > 0) printf("Signature good! This is a .bmp file\n");
    }
    else{
        if(DEBUG_LEVEL > 0) printf("Signature bad! This is not a .bmp file. Quitting\n");
        return;
    }

    int filesize_offset         = 0x0002;
    int data_offset_offset      = 0x000A;
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

    file_size                   = constructIntegerFromByteArray(bmp + filesize_offset, 4, true);
    int data_offset             = constructIntegerFromByteArray(bmp + data_offset_offset, 4, true);
    info_hdr_size               = constructIntegerFromByteArray(bmp + info_hdr_size_offset, 4, true);
    image_width                 = constructIntegerFromByteArray(bmp + image_width_offset, 4, true);
    image_height                = constructIntegerFromByteArray(bmp + image_height_offset, 4, true);

    planes                      = (short) constructIntegerFromByteArray(bmp + planes_offset, 2, true);
    bits_per_pixel              = (short) constructIntegerFromByteArray(bmp + bits_per_pixel_offset, 2, true);

    compression_type            = constructIntegerFromByteArray(bmp + compression_type_offset, 4, true);
    comp_image_size             = constructIntegerFromByteArray(bmp + comp_image_size_offset, 4, true);
    x_pixs_per_meter            = constructIntegerFromByteArray(bmp + x_pixs_per_meter_offset, 4, true);
    y_pixs_per_meter            = constructIntegerFromByteArray(bmp + y_pixs_per_meter_offset, 4, true);
    num_colors_used             = constructIntegerFromByteArray(bmp + num_colors_used_offset, 4, true);
    num_imp_colors              = constructIntegerFromByteArray(bmp + num_imp_colors_offset, 4, true);

    palette = (int*) malloc(sizeof(int) * num_colors_used);

    int i;
    for(i = 0; i < (image_width * image_height * 3); i+=3){
        unsigned char r, g, b;
        r = bmp[data_offset + i + 2];
        g = bmp[data_offset + i + 1];
        b = bmp[data_offset + i + 0];
        BMP_Pixel_24_bit pixel{r, g, b};
        pixel_data.push_back(pixel);
    }
}

//makes a new 24 bit depth, no-compression, image with x width and y height, filled with white
BMP_Image::BMP_Image(int x, int y){

}

BMP_Image::~BMP_Image(){
    free(palette);
}

void BMP_Image::set_pixel(int x_pos, int y_pos, char r, char g, char b){

}

BMP_Pixel_24_bit BMP_Image::get_pixel(int x_pos, int y_pos){

}

void BMP_Image::print_image_data(){
    printf("File Size: %u\n", file_size);
    printf("(Height, Width) = (%u, %u)\n", image_height, image_width);

    int i;
    for(i = 0; i < image_height*image_width; i++){
        if(bits_per_pixel == 24){
            BMP_Pixel_24_bit p = pixel_data[i];
            printf("<R: %u, G: %u, B: %u>\n", p.get_red(), p.get_green(), p.get_blue());
        }
    }
}
