#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdint> //for uint8_t

#include "bmp_pixel.h"
#include "bmp_image.h"
#include "helper_functions.h"
#include "image_editor.h"

using namespace std;

int main()
{
/*
    BMP_Image image{"images/guac.bmp"};
    image.print_stats();
    printf("\n\n");
    BMP_Image gray_image = grayscale_colors(image);
    //image.print_image_data();
    //printf("\n\n");
    gray_image.writeImageToFile("images/gray_guac.bmp");
*/


    BMP_Image image{"images/guac.bmp"};
    image.print_stats();
    printf("\n");
    uint8_t text[13] = "Hello World!";
    image = encode_message(image, text, 12, 1);
    image.writeImageToFile("images/guac_steg_test.bmp");

    return 0;
}
