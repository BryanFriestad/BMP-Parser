#ifndef BMP_PIXEL_H_INCLUDED
#define BMP_PIXEL_H_INCLUDED

#include <cstdint>

class Generic_BMP_Pixel{
private:
    uint8_t pixel_depth;
public:
    Generic_BMP_Pixel(uint8_t p_depth);
    uint8_t get_pixel_depth(){ return pixel_depth; }
};

class BMP_Pixel_24_bit : public Generic_BMP_Pixel{
private:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
public:
    BMP_Pixel_24_bit();
    BMP_Pixel_24_bit(uint8_t r, uint8_t g, uint8_t b);
    void set_color(uint8_t r, uint8_t g, uint8_t b);
    uint8_t get_red() { return red; }
    uint8_t get_green() { return green; }
    uint8_t get_blue() { return blue; }
};

class BMP_Pixel_1_bit : public Generic_BMP_Pixel{
private:
    bool colorIndex;
};

#endif // BMP_PIXEL_H_INCLUDED
