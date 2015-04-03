#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <list>
#include <vector>
#include <exception>
#include "def.h"
#include "bmperror.h"

#define debug(s) (std::cout << s << std::endl);

class Bitmap { 
    // header sizes
    const unsigned int file_header_size = 14;
    const unsigned int bmp_header_size = 40;

    // bmp data
	FileHeader file_header;
	BitmapHeader bitmap_header;
    RGB **bitmap;
    //std::vector<std::vector<RGB> > bitmap;

    // file name
    std::string fname;

    // file size
    unsigned int fsize;

    // image sizes
    unsigned int height;
    unsigned int width;

    // errors list
    std::list<BMPError> errors;

    bool swapRows(unsigned int num1, unsigned int num2);

public:
	Bitmap();
    Bitmap(char const *fname);
    ~Bitmap();

    // handle functions
    void setFilename(char const *fname);
    bool read();
    bool read(char const *fname);
    void print();

    // error handle functions
    bool hasErrors();
    BMPError getLastError();

};

#endif //BMP_H
