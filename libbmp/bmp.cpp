#include "bmp.h"

bool Bitmap::swapRows(unsigned int num1, unsigned int num2)
{
    if(!this->bitmap) {
        this->errors.push_back("image not loaded");
        return false;
    }

    if(num1 > this->bitmap_header.img_height ||
       num2 > this->bitmap_header.img_height) {
        this->errors.push_back("wrong index");
        return false;
    }

    if(num1 == num2) {
        return true;
    }



    return true;
}

Bitmap::Bitmap()
{
    this->bitmap = NULL;
    this->fsize = 0;
}

Bitmap::Bitmap(char const *fname)
{
    this->fname = std::string(fname);
    this->bitmap = NULL;
    this->fsize = 0;
}

Bitmap::~Bitmap()
{
    delete [] this->bitmap;
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
        //debug(this->bitmap_header.img_width);
        //debug(this->bitmap_header.img_width % 4);
        unsigned int align_bytes = this->bitmap_header.img_width % 4;
        unsigned int bitmap_size = this->bitmap_header.img_width *
                                   this->bitmap_header.img_height;

        // allocate memory fo bitmap
        try {
            // allocate rows
            this->bitmap = new RGB* [this->bitmap_header.img_height];
            for(int i = 0; i < this->bitmap_header.img_height; i++) {
                this->bitmap[i] = NULL;
            }

            // allocate columns
            for(int i = 0; i <this->bitmap_header.img_height; i++) {
                this->bitmap[i] = new RGB[this->bitmap_header.img_width];
            }
        }
        catch(std::bad_alloc &e) {
            // allocation fails, free memory

            for(int i = 0; i < this->bitmap_header.img_height; i++) {
                delete [] this->bitmap[i];
                this->bitmap[i] = NULL;
            }
            delete [] this->bitmap;
            this->bitmap = NULL;

            BMPError error("can\'t allocate memory for bitmap");
            error.setDescription("bitmap size is " + std::to_string(bitmap_size) +
                             " bytes");
            this->errors.push_back(error);
            return false;
        }

        // read bitmap from file
        for(int i = 0; i < this->bitmap_header.img_height; i++) {
            // read each row
            unsigned int file_pos = file.tellg();
            for(int j = 0; j < this->bitmap_header.img_width; j++) {
                // read pixel of current row
                file.read((char *)&this->bitmap[i][j], sizeof(RGB));
            }
            // skip align_bytes
            file.seekg(file.tellg() + std::streampos(align_bytes), file.beg);
        }

        // rotate bitmap
        for(int i = 0; i < this->bitmap_header.img_height; i++) {
            for(int j = 0; j < this->bitmap_header.img_width; j++) {

            }
        }
    }

    return success;
}

bool Bitmap::read(const char *fname)
{
    this->fname = std::string(fname);
    return this->read();
}

void Bitmap::print()
{
    for(in)
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
