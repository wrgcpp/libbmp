#ifndef BMP_H
#define BMP_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <list>
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

    // file name
    std::string fname;

    // file size
    unsigned int fsize;

    // errors list
    std::list<BMPError> errors;

public:
	Bitmap();
    Bitmap(char const *fname);
    ~Bitmap();

    // handle functions
    void setFilename(char const *fname);
    bool read();
    bool read(char const *fname);

    // error handle functions
    bool hasErrors();
    BMPError getLastError();

};

#endif //BMP_H
