#ifndef BMP_IMAGE_H_INCLUDED
#define BMP_IMAGE_H_INCLUDED

class BMP_Image{
private:
    //HDR
    int file_size;

    //INFO HDR
    int info_hdr_size;
    int image_width;
    int image_height;
    short planes;
    short bits_per_pixel; //options are 1, 4, 8, 16, 24
    int compression_type; //0 = no compression, 1 = 8b RLE encoding, 2 = 4b RLE encoding
    int comp_image_size; //compressed image size. This should be 0 if compression type is 0
    int x_pixs_per_meter; //the number of x pixels per meter
    int y_pixs_per_meter; //the number of y pixels per meter
    int num_colors_used;
    int num_imp_colors; //the # of important colors

    //Color Palette
    int* palette; //unused unless bits_per_pixel is 1 or 4. Each 4 bytes is arranged as [RED, GREEN, BLUE, reserved]

    //Pixel Data


public:

};

#endif // BMP_IMAGE_H_INCLUDED
