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

    BMP_Image image{"images/guac.bmp"};
    image.print_stats();
    printf("\n\n");
    BMP_Image gray_image = grayscale_colors(image);
    //image.print_image_data();
    //printf("\n\n");
    gray_image.writeImageToFile("images/gray_guac.bmp");


/*
    BMP_Image image{900, 1200};
    //image.print_image_data();
    int i,j;
    for(j = 0; j < 1200; j++){
        for(i = 0; i < 900; i++){
            image.set_pixel(i, j, 255, 255, 0);
        }
    }

    printf("\n");
    //image.print_image_data();
    image.writeImageToFile("images/custom_test.bmp");
*/

    return 0;
}
