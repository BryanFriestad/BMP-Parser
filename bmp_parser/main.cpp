#include <iostream>
#include <vector>
#include <stdio.h>

#include "bmp_pixel.h"
#include "bmp_image.h"

using namespace std;

int main()
{
    //BMP_Image image{"test.bmp"};
    //image.print_image_data();
    //image.print_stats();

    BMP_Image image{3, 3};
    image.print_image_data();
    image.set_pixel(1, 1, 4, 4, 4);
    printf("\n");
    image.print_image_data();

    return 0;
}
