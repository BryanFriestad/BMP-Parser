#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdint> //for uint8_t

#include "bmp_pixel.h"
#include "bmp_image.h"
#include "helper_functions.h"

using namespace std;

int main()
{

    BMP_Image image{"images/stripes.bmp"};
    image.print_stats();
    image.set_pixel(0, 0, 255, 0, 0);
    printf("\n\n");
    //image.print_image_data();
    //printf("\n\n");
    image.writeImageToFile("images/stripes2.bmp");


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
