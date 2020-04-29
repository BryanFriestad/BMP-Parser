#include "image_editor.h"
#include "debug_settings.h"

#include <cstdint> //for uint8_t
#include <stdio.h> //for printf

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

BMP_Image grayscale_colors(BMP_Image im){
    BMP_Image new_i{im.get_image_width(), im.get_image_height()};

    int i, j;
    for(j = 0; j < im.get_image_height(); j++){
        for(i = 0; i < im.get_image_width(); i++){
            BMP_Pixel_24_bit* p = im.get_pixel(i, j);
            int sum = p->get_red() + p->get_green() + p->get_blue();
            uint8_t avg = sum / 3;
            new_i.set_pixel(i, j, avg, avg, avg);
        }
    }

    return new_i;
}

/**
  * im: The original image
  * message: the message that you want to encode, as a byte array
  * msg_len: the length of the message in bytes
  * bits_per_comp: the number of bits to encode in each component (color_channel/pixel)
  * per_color_channel: true if you are encoding values in each color channle, false if only encoding in red channel (least significant)
  */
BMP_Image encode_message(BMP_Image im, uint8_t* message, int msg_len, int bits_per_comp){

    BMP_Image new_i{im.get_image_width(), im.get_image_height()};

    if(msg_len*8 > im.get_image_width() * im.get_image_height()){
        if(DEBUG_LEVEL > 0) printf("Error: message is too long\n");
        return new_i; //the message is too long
    }

    int encoded_bytes = 0;
    int bit;
    while(encoded_bytes < msg_len){
        for(bit = 0; bit < 8; bit++){
            uint8_t bitmask = 0x1 << bit;
            int pixel_num = (encoded_bytes*8) + bit;
            int pixel_x = pixel_num % im.get_image_width();
            int pixel_y = pixel_num / im.get_image_width();
            BMP_Pixel_24_bit* pixel_ptr = im.get_pixel(pixel_x, pixel_y);

            //get bit to encode
            //check if it's 0 or 1
            //appropriately encode LSB of red component
            uint8_t new_red = (message[encoded_bytes] & bitmask) ? (pixel_ptr->get_red() | 0x01) : (pixel_ptr->get_red() & 0xFE);
            new_i.set_pixel(pixel_x, pixel_y, new_red, pixel_ptr->get_green(), pixel_ptr->get_blue());
        }
        encoded_bytes++;
    }

    return new_i;
}
