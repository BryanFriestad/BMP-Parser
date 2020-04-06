#include <iostream>
#include <vector>
#include <stdio.h>

#include "bmp_pixel.h"
#include "bmp_image.h"

using namespace std;

int main()
{
    BMP_Image image{"rick.bmp"};
    image.print_image_data();
    return 0;
}
