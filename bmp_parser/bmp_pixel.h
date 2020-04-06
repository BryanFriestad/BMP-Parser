#ifndef BMP_PIXEL_H_INCLUDED
#define BMP_PIXEL_H_INCLUDED

class Generic_BMP_Pixel{
private:
    unsigned char pixel_depth;
public:
    Generic_BMP_Pixel(unsigned char p_depth);
    unsigned char get_pixel_depth(){ return pixel_depth; }
};

class BMP_Pixel_24_bit : public Generic_BMP_Pixel{
private:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
public:
    BMP_Pixel_24_bit();
    BMP_Pixel_24_bit(unsigned char r, unsigned char g, unsigned char b);
    void set_color(unsigned char r, unsigned char g, unsigned char b);
    unsigned char get_red() { return red; }
    unsigned char get_green() { return green; }
    unsigned char get_blue() { return blue; }
};

class BMP_Pixel_1_bit : public Generic_BMP_Pixel{
private:
    bool colorIndex;
};

#endif // BMP_PIXEL_H_INCLUDED
