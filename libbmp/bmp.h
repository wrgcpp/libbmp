#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include "def.h"

#define DEBUG(s) (std::cout << s << std::endl);

class Bitmap { 
    // header sizes
    const unsigned int file_header_size = 14;
    const unsigned int bmp_header_size = 40;

    // bmp data
	FileHeader file_header;
	BitmapHeader bitmap_header;
    RGBQuad *palette;

    // file name
    std::string fname;

public:
	Bitmap();
    Bitmap(char const *fname);
    ~Bitmap();

    void setFilename(char const *fname);
    void read();
    void read(char const *fname);

};

#endif //BMP_H
