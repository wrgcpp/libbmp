#include "bmp.h"

Bitmap::Bitmap()
{
    //this->palette = NULL;
    this->fsize = 0;
}

Bitmap::Bitmap(char const *fname)
{
    this->fname = std::string(fname);
    //this->palette = NULL;
    this->fsize = 0;
}

Bitmap::~Bitmap()
{
    //delete [] this->palette;
    //this->palette = NULL;
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
        unsigned int align = this->bitmap_header.img_width % 4;
        unsigned int aligned_width = this->bitmap_header.img_width *
                                     sizeof(RGBQuad) + align;
        debug(aligned_width);
    }

    return success;
}

bool Bitmap::read(const char *fname)
{
    this->fname = std::string(fname);
    return this->read();
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
