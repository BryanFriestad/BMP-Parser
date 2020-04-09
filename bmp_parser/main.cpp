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
    /*
    uint8_t text[] = "Hello World/r/n";
    writeFileBytes("text_test.txt", text, 13);
    */

/*
    BMP_Image image{"test.bmp"};
    image.print_image_data();
    image.writeImageToFile("test_copy.bmp");
*/


    BMP_Image image{3, 3};
    image.print_image_data();
    image.set_pixel(1, 1, 4, 4, 4);
    printf("\n");
    image.print_image_data();
    image.writeImageToFile("custom_test.bmp");


    return 0;
}
