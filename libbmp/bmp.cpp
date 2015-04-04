#include "bmp.h"

bool Bitmap::swapRows(unsigned int num1, unsigned int num2)
{
    if(!this->bitmap) {
        this->errors.push_back("image not loaded");
        return false;
    }

    if(num1 >= this->bitmap_header.img_height ||
       num2 >= this->bitmap_header.img_height) {
        this->errors.push_back("wrong index");
        return false;
    }

    if(num1 == num2) {
        return true;
    }

    for(int i = 0; i < this->bitmap_header.img_width; i++) {
        RGB tmp = this->bitmap[num1][i];
        this->bitmap[num1][i] = this->bitmap[num2][i];
        this->bitmap[num2][i] = tmp;
    }

    return true;
}

bool Bitmap::allocBitmapMem()
{
    if(!this->height || !this->width) {
        // no memory needed for image 0x0px
        this->errors.push_back("no memory needed for image 0x0");
        return false;
    }

    try {
        // allocate rows
        this->bitmap = new RGB* [this->height];
        for(int i = 0; i < this->height; i++) {
            this->bitmap[i] = NULL;
        }

        // allocate columns
        for(int i = 0; i <this->height; i++) {
            this->bitmap[i] = new RGB[this->width];
        }
    }
    catch(std::bad_alloc &e) {
        // allocation fails, free memory
        this->freeBitmapMem();
        this->errors.push_back("can\'t allocate memory for bitmap");
        return false;
    }

    return true;
}

void Bitmap::freeBitmapMem()
{
    for(int i = 0; i < this->height; i++) {
        delete [] this->bitmap[i];
        this->bitmap[i] = NULL;
    }
    delete [] this->bitmap;
    this->bitmap = NULL;
}

Bitmap::Bitmap()
{
    this->bitmap = NULL;
    this->fsize = 0;
    this->width = 0;
    this->height = 0;
}

Bitmap::Bitmap(char const *fname)
{
    this->fname = std::string(fname);
    this->bitmap = NULL;
    this->fsize = 0;
    this->width = 0;
    this->height = 0;
}

Bitmap::~Bitmap()
{
    this->freeBitmapMem();
    this->bitmap = NULL;
}

void Bitmap::setFilename(const char *fname)
{
    this->fname = std::string(fname);
}

bool Bitmap::read()
{
    if(!this->fname.length()) {
        this->errors.push_back(BMPError("no file name specified"));
        return false;
    }

    // open file
    std::ifstream file;
    file.open(fname, std::ios::binary);
    if(!file.is_open()) {
        this->errors.push_back(BMPError("error opening file"));
        return false;
    }

    // calculate file size
    file.seekg(0, file.end);
    this->fsize = file.tellg();
    file.seekg(0, file.beg);

    // read file header
    file.read((char *)&this->file_header, sizeof(this->file_header));
    if(!file.good()) {
        BMPError e("error reading file \"" + fname + "\"");
        if(file.eof()) {
            e.setDescription("unexpected end of the file");
        }
        this->errors.push_back(e);
        return false;
    }

    // read bmp header
    file.read((char *)&this->bitmap_header, sizeof(this->bitmap_header));
    if(!file.good()) {
        BMPError e("error reading file \"" + fname + "\"");
        if(file.eof()) {
            e.setDescription("unexpected end of the file");
        }
        this->errors.push_back(e);
        return false;
    }

    // check that this is BMP file
    if(memcmp(this->file_header.bm, "BM", 2)) {
        this->errors.push_back(BMPError("not a BMP file"));
        return false;
    }

    bool success = true;

    // check that this is 24 bit image
    // files with another color depth are unsupported
    if(this->bitmap_header.bit_count != 24) {
        std::string errmsg = "unsupported color depth - " +
                std::to_string(this->bitmap_header.bit_count) +
                "bit";
        std::string errdesc = "only 24 bit images are supported";
        this->errors.push_back(BMPError(errmsg, errdesc));
        success = false;
    }

    // check that image is not compressed
    // compressed images are unsupported
    if(this->bitmap_header.compress_type) {
        this->errors.push_back("compressed images are unsupported");
        success = false;
    }

    // check that file size coincides
    if(this->file_header.size != this->fsize) {
        this->errors.push_back("file size mismatch");
        success = false;
    }

    // if verification is done read the bitmap
    if(success) {
        this->width = this->bitmap_header.img_width;
        this->height = this->bitmap_header.img_height;
        unsigned int align_bytes = this->width % 4;
        unsigned int bitmap_size = this->width * this->height;

        // allocate memory fo bitmap
        if(!this->allocBitmapMem()) {
            return false;
        }

        // read bitmap from file
        for(int i = 0; i < this->height; i++) {
            // read each row
            for(int j = 0; j < this->width; j++) {
                // read pixel of current row
                file.read((char *)&this->bitmap[i][j], sizeof(RGB));
            }
            // skip align_bytes
            file.seekg(file.tellg() + std::streampos(align_bytes), file.beg);
        }

        // rotate bitmap
        for(unsigned int i = 0; i < this->height / 2; i++) {
            this->swapRows(i, this->height-1 - i);
        }
    }

    return success;
}

bool Bitmap::read(const char *fname)
{
    this->fname = std::string(fname);
    return this->read();
}

bool Bitmap::write()
{
    if(!this->fname.length()) {
        this->errors.push_back(BMPError("no file name specified"));
        return false;
    }

    // open file
    std::ofstream file;
    file.open(fname, std::ios::binary);
    if(!file.is_open()) {
        this->errors.push_back(BMPError("error opening file"));
        return false;
    }

    // calculate file size
    unsigned int align_bytes = this->width % 4;
    this->file_header.size = (this->width * sizeof(RGB) + align_bytes) * this->height;

    // write file header
    file.write((char *)&this->file_header, sizeof(this->file_header));
    if(!file.good()) {
        BMPError e("error writing file \"" + fname + "\"");
        this->errors.push_back(e);
        return false;
    }

    // write bmp header
    file.write((char *)&this->bitmap_header, sizeof(this->bitmap_header));
    if(!file.good()) {
        BMPError e("error writing file \"" + fname + "\"");
        this->errors.push_back(e);
        return false;
    }

    // write rotated bitmap to file
    for(int i = this->height - 1; i >= 0; i--) {
        for(int j = 0; j < this->width; j++) {
            file.write((char *)&this->bitmap[i][j], sizeof(RGB));
        }
        // write align bytes to file
        char zero_byte = 0;
        for(int j = 0; j < align_bytes; j++) {
            file.write((char *)&zero_byte, sizeof(char));
        }
    }

    return true;
}

bool Bitmap::write(const char *fname)
{
    this->fname = std::string(fname);
    return this->write();
}

void Bitmap::print()
{
    for(int i = 0; i < this->height; i++) {
        for(int j = 0; j < this->width; j++) {
            RGB px = this->bitmap[i][j];
            if(px.blue && px.green && px.red) {
                std::cout << "█";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

bool Bitmap::hasErrors()
{
    return !this->errors.empty();
}

BMPError Bitmap::getLastError()
{
    BMPError e = this->errors.back();
    this->errors.pop_back();
    return e;
}
