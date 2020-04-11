#include "bmp_image.h"
#include "bmp_pixel.h"
#include "helper_functions.h"
#include "debug_settings.h"

#include <stdlib.h> //free and malloc
#include <stdio.h> //printf
#include <math.h> //used for ceil
#include <cstdint> //used for uint8_t

BMP_Image::BMP_Image(char* filename){
    if(DEBUG_LEVEL > 2) printf("Info: Starting to read from file\n");
    char* bmp = readFileBytes(filename);
    if(DEBUG_LEVEL > 2) printf("Info: Done reading from file\n");

    if(bmp[0] == 'B' and bmp[1] == 'M'){
        if(DEBUG_LEVEL > 0) printf("Signature good! This is a .bmp file\n");
    }
    else{
        if(DEBUG_LEVEL > 0) printf("Signature bad! This is not a .bmp file. Quitting\n");
        return;
    }

    int filesize_offset         = 0x0002;
    int data_offset_offset      = 0x000A;
    int info_hdr_size_offset    = 0x000E;
    int image_width_offset      = 0x0012;
    int image_height_offset     = 0x0016;

    int planes_offset           = 0x001A;
    int bits_per_pixel_offset   = 0x001C;
    int compression_type_offset = 0x001E;
    int comp_image_size_offset  = 0x0022;
    int x_pixs_per_meter_offset = 0x0026;
    int y_pixs_per_meter_offset = 0x002A;
    int num_colors_used_offset  = 0x002E;
    int num_imp_colors_offset   = 0x0032;
    int palette_offset          = 0x0036;

    if(DEBUG_LEVEL > 2) printf("Starting to construct from byte array\n");

    file_size                   = constructIntegerFromByteArray(bmp + filesize_offset, 4, true);
    data_offset                 = constructIntegerFromByteArray(bmp + data_offset_offset, 4, true);
    if(DEBUG_LEVEL > 0) printf("\tdata offset = byte %d\n", data_offset);
    info_hdr_size               = constructIntegerFromByteArray(bmp + info_hdr_size_offset, 4, true);
    if(info_hdr_size != 40){
        if(DEBUG_LEVEL > 1) printf("\tWarning, information header (%d bytes) != 40 bytes\n", info_hdr_size);
    }

    image_width                 = constructIntegerFromByteArray(bmp + image_width_offset, 4, true);
    image_height                = constructIntegerFromByteArray(bmp + image_height_offset, 4, true);

    planes                      = (short) constructIntegerFromByteArray(bmp + planes_offset, 2, true);
    if(planes != 1){
        if(DEBUG_LEVEL > 1) printf("\tWarning, # of planes (%d) != 1\n", planes);
    }

    bits_per_pixel              = (short) constructIntegerFromByteArray(bmp + bits_per_pixel_offset, 2, true);
    if(bits_per_pixel != 24){
        if(DEBUG_LEVEL > 1) printf("\tWarning, bits/pixel (%d) != 24\n", bits_per_pixel);
    }

    compression_type            = constructIntegerFromByteArray(bmp + compression_type_offset, 4, true);
    comp_image_size             = constructIntegerFromByteArray(bmp + comp_image_size_offset, 4, true);
    x_pixs_per_meter            = constructIntegerFromByteArray(bmp + x_pixs_per_meter_offset, 4, true);
    y_pixs_per_meter            = constructIntegerFromByteArray(bmp + y_pixs_per_meter_offset, 4, true);
    num_colors_used             = constructIntegerFromByteArray(bmp + num_colors_used_offset, 4, true);
    num_imp_colors              = constructIntegerFromByteArray(bmp + num_imp_colors_offset, 4, true);

    palette = (int*) malloc(sizeof(int) * num_colors_used);

    if(DEBUG_LEVEL > 2) printf("\tStarting to make pixels from byte array\n");

    int scanline_size = ceil((bits_per_pixel * image_width)/32.0f) * 4; //the number of bytes per scanline, including padding up to multiple of 4 bytes
    if(DEBUG_LEVEL > 1) printf("\tscanline width: %d bytes\n", scanline_size);

    int i, j;
    for(j = image_height - 1; j >= 0; j--){ //scanlines are in bottom-to-top order
        for(i = 0; i < image_width; i++){
            //if(DEBUG_LEVEL > 2) printf("Info: adding pixel at position <%d, %d>\n", i, j);
            int row_offset = j * scanline_size;
            int col_offset = i * (bits_per_pixel/8);
            //if(DEBUG_LEVEL > 3) printf("col, row offsets = <%d, %d>\n", row_offset, col_offset);

            uint8_t r, g, b;
            r = bmp[data_offset + row_offset + col_offset + 2];
            g = bmp[data_offset + row_offset + col_offset + 1];
            b = bmp[data_offset + row_offset + col_offset + 0];
            BMP_Pixel_24_bit pixel{r, g, b};
            pixel_data.push_back(pixel);
        }
    }

    if(DEBUG_LEVEL > 2) printf("Done constructing image from byte array\n");
}

//makes a new 24 bit depth, no-compression, image with x width and y height, filled with white
BMP_Image::BMP_Image(int x, int y){

    image_width                 = x;
    image_height                = y;
    bits_per_pixel              = 24;

    planes                      = 1;
    compression_type            = 0;
    comp_image_size             = 0;

    int scanline_size = ceil((bits_per_pixel * image_width)/32.0f) * 4; //the number of bytes per scanline, including padding up to multiple of 4 bytes
    if(DEBUG_LEVEL > 2) printf("Info: scanline width: %d bytes\n", scanline_size);

    int i;
    for(i = 0; i < image_height*image_width; i++){
        BMP_Pixel_24_bit p{255, 255, 255};
        pixel_data.push_back(p);
    }

    file_size                   = 54 + (scanline_size * image_height);
    data_offset                 = 54;
    info_hdr_size               = 40;

    x_pixs_per_meter            = 3780;
    y_pixs_per_meter            = 3780;
    num_colors_used             = 0;
    num_imp_colors              = 0;

    palette = (int*) malloc(sizeof(int) * num_colors_used);
}

BMP_Image::~BMP_Image(){
    free(palette);
}

void BMP_Image::writeImageToFile(char* filename){
    if(DEBUG_LEVEL > 2) printf("Info: Writing image to file\n");

    if(DEBUG_LEVEL > 2) printf("\tInfo: manually allocating memory (%d bytes)\n", file_size);
    //construct byte array
    uint8_t* byte_arr = (uint8_t*) malloc(sizeof(uint8_t) * file_size);

    if(DEBUG_LEVEL > 2) printf("\tInfo: starting header\n");
    //signature
    byte_arr[0] = 'B';
    byte_arr[1] = 'M';

    //filesize
    byte_arr[2] = (file_size >> 0) & 0xFF;
    byte_arr[3] = (file_size >> 8) & 0xFF;
    byte_arr[4] = (file_size >> 16) & 0xFF;
    byte_arr[5] = (file_size >> 24) & 0xFF;

    //reserved
    byte_arr[6] = 0;
    byte_arr[7] = 0;
    byte_arr[8] = 0;
    byte_arr[9] = 0;

    //data offset = 54 bytes
    byte_arr[10] = (54 >> 0) & 0xFF; //little byte first
    byte_arr[11] = (54 >> 8) & 0xFF;
    byte_arr[12] = (54 >> 16) & 0xFF;
    byte_arr[13] = (54 >> 24) & 0xFF; //big byte last

    if(DEBUG_LEVEL > 2) printf("\tInfo: starting information header\n");
    //info HDR size
    byte_arr[14] = (40 >> 0) & 0xFF;
    byte_arr[15] = (40 >> 8) & 0xFF;
    byte_arr[16] = (40 >> 16) & 0xFF;
    byte_arr[17] = (40 >> 24) & 0xFF;

    //image width
    byte_arr[18] = (image_width >> 0) & 0xFF;
    byte_arr[19] = (image_width >> 8) & 0xFF;
    byte_arr[20] = (image_width >> 16) & 0xFF;
    byte_arr[21] = (image_width >> 24) & 0xFF;

    //image height
    byte_arr[22] = (image_height >> 0) & 0xFF;
    byte_arr[23] = (image_height >> 8) & 0xFF;
    byte_arr[24] = (image_height >> 16) & 0xFF;
    byte_arr[25] = (image_height >> 24) & 0xFF;

    //planes
    byte_arr[26] = (planes >> 0) & 0xFF;
    byte_arr[27] = (planes >> 8) & 0xFF;

    //bits/pixel
    byte_arr[28] = (bits_per_pixel >> 0) & 0xFF;
    byte_arr[29] = (bits_per_pixel >> 8) & 0xFF;

    //compression method
    byte_arr[30] = (compression_type >> 0) & 0xFF;
    byte_arr[31] = (compression_type >> 8) & 0xFF;
    byte_arr[32] = (compression_type >> 16) & 0xFF;
    byte_arr[33] = (compression_type >> 24) & 0xFF;

    //compressed size
    byte_arr[34] = (comp_image_size >> 0) & 0xFF;
    byte_arr[35] = (comp_image_size >> 8) & 0xFF;
    byte_arr[36] = (comp_image_size >> 16) & 0xFF;
    byte_arr[37] = (comp_image_size >> 24) & 0xFF;

    //x pixels/meter
    byte_arr[38] = (x_pixs_per_meter >> 0) & 0xFF;
    byte_arr[39] = (x_pixs_per_meter >> 8) & 0xFF;
    byte_arr[40] = (x_pixs_per_meter >> 16) & 0xFF;
    byte_arr[41] = (x_pixs_per_meter >> 24) & 0xFF;

    //y pixels/meter
    byte_arr[42] = (y_pixs_per_meter >> 0) & 0xFF;
    byte_arr[43] = (y_pixs_per_meter >> 8) & 0xFF;
    byte_arr[44] = (y_pixs_per_meter >> 16) & 0xFF;
    byte_arr[45] = (y_pixs_per_meter >> 24) & 0xFF;

    //colors used
    byte_arr[46] = (num_colors_used >> 0) & 0xFF;
    byte_arr[47] = (num_colors_used >> 8) & 0xFF;
    byte_arr[48] = (num_colors_used >> 16) & 0xFF;
    byte_arr[49] = (num_colors_used >> 24) & 0xFF;

    //important colors
    byte_arr[50] = (num_imp_colors >> 0) & 0xFF;
    byte_arr[51] = (num_imp_colors >> 8) & 0xFF;
    byte_arr[52] = (num_imp_colors >> 16) & 0xFF;
    byte_arr[53] = (num_imp_colors >> 24) & 0xFF;

    if(DEBUG_LEVEL > 2) printf("\tInfo: starting palette\n");
    //color palette

    if(DEBUG_LEVEL > 2) printf("\tInfo: starting pixel data\n");
    //pixel data
    int scanline_size = ceil((bits_per_pixel * image_width)/32.0f) * 4;
    if(DEBUG_LEVEL > 2) printf("\tInfo: scanline width: %d bytes\n", scanline_size);

    int padding_bytes = scanline_size - ((bits_per_pixel * image_width)/8); //number of bytes per row of 0's
    if(DEBUG_LEVEL > 2) printf("\tInfo: padding bytes: %d bytes\n", padding_bytes);

    int index = 54;
    int i, j;
    for(j = image_height - 1; j >= 0; j--){ //scanlines are in bottom-to-top order
        for(i = 0; i < image_width; i++){
            BMP_Pixel_24_bit* pixel = get_pixel(i, j);
            if(pixel == nullptr){
                if(DEBUG_LEVEL > 0) printf("\tError: invalid pixel, quitting write at index = %d\n", index);
                return;
            }
            byte_arr[index++] = pixel->get_blue(); //BLUE is stored first
            byte_arr[index++] = pixel->get_green();
            byte_arr[index++] = pixel->get_red(); //RED is stored last

            if(index > 660 && index < 670){
                    printf("index == %d\n", index);
                    if(DEBUG_LEVEL > 2) printf("\tInfo: adding pixel <%d, %d> to byte array\n", i, j);
                    printf("(%d, %d) = <R: %u, G: %u, B: %u>\n", i, j, pixel->get_red(), pixel->get_green(), pixel->get_blue());
            }
        }
        for(i = 0; i < padding_bytes; i++){
            byte_arr[index++] = 0;
        }
    }
    if(index == file_size){
        writeFileBytes(filename, byte_arr, file_size);
    }
    else{
        if(DEBUG_LEVEL > 0) printf("\tFinal index does not match file_size (%d): %d\n", file_size, index);
    }
    free(byte_arr);
    if(DEBUG_LEVEL > 2) printf("Info: Done writing image to file\n");
}

//Image is indexed with (0, 0) at the top left corner of the image. X is horizontal and Y is vertical
void BMP_Image::set_pixel(int x_pos, int y_pos, uint8_t r, uint8_t g, uint8_t b){
    BMP_Pixel_24_bit* pixel = get_pixel(x_pos, y_pos);
    if(pixel != nullptr){
        pixel->set_color(r, g, b);
    }
}

//Image is indexed with (0, 0) at the top left corner of the image. X is horizontal and Y is vertical
BMP_Pixel_24_bit* BMP_Image::get_pixel(int x_pos, int y_pos){
    if(x_pos < 0 || x_pos >= image_width){
        if(DEBUG_LEVEL > 0) printf("x_pos out of bounds: <%d, %d>\n", x_pos, y_pos);
        return nullptr;
    }
    if(y_pos < 0 || y_pos >= image_height){
        if(DEBUG_LEVEL > 0) printf("y_pos out of bounds: <%d, %d>\n", x_pos, y_pos);
        return nullptr;
    }

    int index = x_pos + (y_pos*image_width);
    return (&pixel_data.at(index));
}

void BMP_Image::print_image_data(){
    printf("File Size: %u bytes\n", file_size);
    printf("(Width, Height) = (%u, %u)\n", image_width, image_height);

    int i;
    for(i = 0; i < image_height*image_width; i++){
        if(bits_per_pixel == 24){
            BMP_Pixel_24_bit p = pixel_data[i];
            printf("(%d, %d) = <R: %u, G: %u, B: %u>\n", i%image_width, i/image_width, p.get_red(), p.get_green(), p.get_blue());
        }
    }
}

void BMP_Image::print_stats(){
    printf("File Size: %u bytes\n", file_size);
    printf("(Width, Height) = (%u, %u)\n", image_width, image_height);
    printf("Bits/pixel = %d bits\n", bits_per_pixel);

    printf("Info header size = %d bytes\n", info_hdr_size);
    printf("# of Planes = %d\n", planes);
    printf("Compression Type = %d\n", compression_type);
    printf("Compressed Image size = %d bytes\n", comp_image_size);
    printf("Pixels per meter = <x: %d, y: %d>\n", x_pixs_per_meter, y_pixs_per_meter);
    printf("Number of colors used = %d\n", num_colors_used);
    printf("Number of important colors = %d\n", num_imp_colors);
}
