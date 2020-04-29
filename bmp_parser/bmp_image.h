#ifndef BMP_IMAGE_H_INCLUDED
#define BMP_IMAGE_H_INCLUDED

#include "bmp_pixel.h"
#include <vector>

class BMP_Image{
private:
    //HDR
    int file_size;
    int data_offset;

    //INFO HDR
    int info_hdr_size;
    int image_width;
    int image_height;
    short planes;
    short bits_per_pixel; //options are 1, 4, 8, 16, 24
    int compression_type; //0 = no compression, 1 = 8b RLE encoding, 2 = 4b RLE encoding
    int comp_image_size; //compressed image size. This should be 0 if compression type is 0
    int x_pixs_per_meter; //the number of x pixels per meter 3780 standard
    int y_pixs_per_meter; //the number of y pixels per meter 3780 standard
    int num_colors_used;
    int num_imp_colors; //the # of important colors

    //Color Palette
    int* palette; //unused unless bits_per_pixel is 1 or 4. Each 4 bytes is arranged as [RED, GREEN, BLUE, reserved]

    //Pixel Data
    std::vector<BMP_Pixel_24_bit> pixel_data;

public:
    BMP_Image(char* filename); //used to load a bmp from a file
    BMP_Image(int x, int y); //makes a new 24 bit depth, no-compression, image with x width and y height, filled with white
    ~BMP_Image();
    void set_pixel(int x_pos, int y_pos, uint8_t r, uint8_t g, uint8_t b);
    void set_pixel_red(int x_pos, int y_pos, uint8_t r);
    void set_pixel_green(int x_pos, int y_pos, uint8_t g);
    void set_pixel_blue(int x_pos, int y_pos, uint8_t b);
    BMP_Pixel_24_bit* get_pixel(int x_pos, int y_pos);
    void writeImageToFile(char* filename);
    void print_image_data();
    void print_stats();
    int get_image_width(){ return image_width; }
    int get_image_height(){ return image_height; }
};

#endif // BMP_IMAGE_H_INCLUDED
