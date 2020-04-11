#include "image_editor.h"

BMP_Image invert_colors(BMP_Image i){
    BMP_Image new_i{i.get_image_width(), i.get_image_height()};

    int i, j;
    for(j = 0; j < i.get_image_height(); j++){
        for(i = 0; i < i.get_image_width(); i++){
            BMP_Pixel_1_bit* p = i.get_pixel(i, j);
            new_i.set_pixel(i, j, 255 - p->get_red(), 255 - p->get_green(), 255 - p->get_blue());
        }
    }

    return new_i;
};
