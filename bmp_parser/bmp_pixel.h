#ifndef BMP_PIXEL_H_INCLUDED
#define BMP_PIXEL_H_INCLUDED

class Generic_BMP_Pixel{
private:
    char pixel_depth;
public:
    Generic_BMP_Pixel(char p_depth);
    char get_pixel_depth(){ return pixel_depth; }
};

class BMP_Pixel_24_bit : public Generic_BMP_Pixel{
private:
    char red;
    char green;
    char blue;
public:
    BMP_Pixel_24_bit();
    BMP_Pixel_24_bit(char r, char g, char b);
    void set_color(char r, char g, char b);
    char get_red() { return red; }
    char get_green() { return green; }
    char get_blue() { return blue; }
};

class BMP_Pixel_1_bit : public Generic_BMP_Pixel{
private:
    bool colorIndex;
};

#endif // BMP_PIXEL_H_INCLUDED
