#include "image_editor.h"
#include "debug_settings.h"

#include <cstdint> //for uint8_t
#include <stdio.h> //for printf
#include <cmath> //for pow

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
  * This will encode data from the array into the image given, *bits_per_comp* at a time, starting with the most significant bits of the byte array
  * im: The original image
  * message: the message that you want to encode, as a byte array
  * msg_len: the length of the message in bytes
  * bits_per_comp: the number of bits to encode in each component (color_channel/pixel)
  * per_color_channel: true if you are encoding values in each color channle, false if only encoding in red channel (least significant)
  */
BMP_Image encode_message(BMP_Image im, uint8_t* message, int msg_len, int bits_per_comp, bool per_color_channel){

    BMP_Image new_i{im.get_image_width(), im.get_image_height()};

    if(bits_per_comp > 8){
        if(DEBUG_LEVEL > 0) printf("Error: too many bits per component (%d)\n", bits_per_comp);
        return new_i;
    }
    else if(bits_per_comp < 1){
        if(DEBUG_LEVEL > 0) printf("Error: invalid bits per component (%d). Must be greater than 0.\n", bits_per_comp);
        return new_i;
    }

    if(msg_len*8 > im.get_image_width() * im.get_image_height()){
        if(DEBUG_LEVEL > 0) printf("Error: message is too long\n");
        return new_i; //the message is too long
    }

    //copy the whole picture
    int i, j;
    for(j = 0; j < im.get_image_height(); j++){
        for(i = 0; i < im.get_image_width(); i++){
            new_i.set_pixel(i, j, im.get_pixel(i, j));
        }
    }

    int bits_per_pixel = per_color_channel ? (3*bits_per_comp) : bits_per_comp; //the number of bits to encode per pixel
    uint8_t data_bitmask = pow(2, bits_per_comp) - 1;
    int data_bit = 0;
    while(data_bit < msg_len*8){
        //step 1: get the data to encode
        uint8_t data_byte = message[data_bit/8];
        int data_to_encode_start_bit = data_bit % 8;
        int shift_left_amt = 8 - (data_to_encode_start_bit + bits_per_comp);
        uint8_t data_to_encode = data_byte >> shift_left_amt;
        //TODO: there is an issue here when the bits need to be grabbed from two bytes
        data_to_encode &= data_bitmask;

        //step 2: get the pixel we are encoding
        int pixel_num = data_bit / bits_per_pixel;
        int pixel_x = pixel_num % im.get_image_width();
        int pixel_y = pixel_num / im.get_image_width();
        BMP_Pixel_24_bit* pixel_ptr = im.get_pixel(pixel_x, pixel_y);

        //step 3: determine which color channel we are encoding into
        int channel;
        if(per_color_channel){
            channel = (data_bit / bits_per_comp) % 3;
        }
        else{
            channel = 2; //the red channel
        }

        //step 4: encode the data
        if(channel == 0){
            uint8_t old_value = pixel_ptr->get_blue();
            uint8_t new_value = (old_value & ~data_bitmask) | data_to_encode;
            new_i.set_pixel_blue(pixel_x, pixel_y, new_value);
        }
        else if(channel == 1){
            uint8_t old_value = pixel_ptr->get_green();
            uint8_t new_value = (old_value & ~data_bitmask) | data_to_encode;
            new_i.set_pixel_green(pixel_x, pixel_y, new_value);
        }
        else if(channel == 2){
            uint8_t old_value = pixel_ptr->get_red();
            uint8_t new_value = (old_value & ~data_bitmask) | data_to_encode;
            new_i.set_pixel_red(pixel_x, pixel_y, new_value);
        }

        //step 5: advance to the next component
        data_bit += bits_per_comp;
    }

    /* old code
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
    */

    return new_i;
}
