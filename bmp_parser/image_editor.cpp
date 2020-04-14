#include "image_editor.h"

BMP_Image invert_colors(BMP_Image im){
    BMP_Image new_i{im.get_image_width(), im.get_image_height()};

    int i, j;
    for(j = 0; j < im.get_image_height(); j++){
        for(i = 0; i < im.get_image_width(); i++){
            BMP_Pixel_24_bit* p = im.get_pixel(i, j);
            new_i.set_pixel(i, j, 255 - p->get_red(), 255 - p->get_green(), 255 - p->get_blue());
        }
    }

    return new_i;
};
