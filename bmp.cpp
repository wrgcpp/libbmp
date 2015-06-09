#include "bmp.h"

bool Bitmap::swapRows(unsigned int num1, unsigned int num2)
{
    if(!this->bitmap) {
        return false;
    }

    if(num1 >= this->bitmap_header.img_height ||
       num2 >= this->bitmap_header.img_height) {
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
        this->isError = false;
        throw(std::runtime_error("no memory needed for image 0x0px"));
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
        return true;
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
    this->isError = false;
}

Bitmap::Bitmap(char const *fname)
{
    this->fname = std::string(fname);
    this->bitmap = NULL;
    this->fsize = 0;
    this->width = 0;
    this->height = 0;
    this->isError = false;
}

Bitmap::~Bitmap()
{
    this->freeBitmapMem();
    this->bitmap = NULL;
}

void Bitmap::setFilename(const char *fname)
{
    this->fname = std::string(fname);
    this->isError = false;
}

void Bitmap::read()
{
    // check that filename is specified
    if(!this->fname.length()) {
        this->isError = true;
        throw(std::logic_error("no filename specified"));
    }

    // open file
    std::ifstream file;
    file.open(fname, std::ios::binary);
    if(!file.is_open()) {
        this->isError = true;
        throw(std::runtime_error("can't open file " + this->fname));
    }

    // calculate file size
    file.seekg(0, file.end);
    this->fsize = file.tellg();
    file.seekg(0, file.beg);

    // read file header
    file.read((char *)&this->file_header, sizeof(this->file_header));
    if(!file.good()) {
        this->isError = true;
        throw(std::runtime_error("unexpected end of file " + this->fname));
    }

    // read bmp header
    file.read((char *)&this->bitmap_header, sizeof(this->bitmap_header));
    if(!file.good()) {
        this->isError = true;
        throw(std::runtime_error("unexpected end of file " + this->fname));
    }

    // check that this is BMP file
    if(memcmp(this->file_header.bm, "BM", 2)) {
        this->isError = true;
        throw(std::logic_error("not a BMP file"));
    }

    // check that this is 24 bit image
    // files with another color depth are unsupported
    if(this->bitmap_header.bit_count != 24) {
        this->isError = true;
        throw(std::logic_error("only 24 bit BMP images are supported"));
    }

    // check that image is not compressed
    // compressed images are unsupported
    if(this->bitmap_header.compress_type) {
        this->isError = true;
        throw(std::logic_error("compressed images are unsupported"));
    }

    // check that file size coincides
    if(this->file_header.size != this->fsize) {
        this->isError = true;
        throw(std::logic_error("real file size and specified file size are different"));
    }

    // if verification is done read the bitmap
    this->width = this->bitmap_header.img_width;
    this->height = this->bitmap_header.img_height;
    unsigned int align_bytes = this->width % 4;

    // allocate memory fo bitmap
    if(!this->allocBitmapMem()) {
        this->isError = true;
        throw(std::runtime_error("can't allocate memory for bitmap"));
    }

    // read bitmap from file
    for(int i = 0; i < this->height; i++) {
        // read each row
        for(int j = 0; j < this->width; j++) {
            // read pixel of current row
            file.read((char *)&this->bitmap[i][j], sizeof(RGB));
            if(!file.good()) {
                this->isError = true;
                throw(std::runtime_error("error reading bitmap from file. unexpected end of file"));
            }
        }
        // skip align_bytes
        file.seekg(file.tellg() + std::streampos(align_bytes), file.beg);
        if(!file.good()) {
            this->isError = true;
            throw(std::runtime_error("error reading bitmap from file. unexpected end of file"));
        }
    }

    // rotate bitmap
    for(unsigned int i = 0; i < this->height / 2; i++) {
        this->swapRows(i, this->height-1 - i);
    }

    this->isError = false;
}

void Bitmap::read(const char *fname)
{
    this->fname = std::string(fname);
    this->read();
}

void Bitmap::write()
{
    if(!this->fname.length()) {
        this->isError = true;
        throw(std::logic_error("no filename specified"));
    }

    // open file
    std::ofstream file;
    file.open(fname, std::ios::binary);
    if(!file.is_open()) {
        this->isError = true;
        throw(std::runtime_error("can't open file " + this->fname));
    }

    // calculate file size
    // align bytes are located at the end of each row,
    // because a row length must be a multiple of 4
    unsigned int align_bytes = this->width % 4;
    this->file_header.size = (this->width * sizeof(RGB) + align_bytes) * this->height;

    // write file header
    file.write((char *)&this->file_header, sizeof(this->file_header));
    if(!file.good()) {
        this->isError = true;
        throw(std::runtime_error("error writing file header to file " + this->fname));
    }

    // write bmp header
    file.write((char *)&this->bitmap_header, sizeof(this->bitmap_header));
    if(!file.good()) {
        this->isError = true;
        throw(std::runtime_error("error writing bitmap header to file " + this->fname));
    }

    // write rotated bitmap to file
    for(int i = this->height - 1; i >= 0; i--) {
        for(int j = 0; j < this->width; j++) {
            file.write((char *)&this->bitmap[i][j], sizeof(RGB));
            if(!file.good()) {
                this->isError = true;
                throw(std::runtime_error("error writing bitmap to file. file is now corrupted"));
            }
        }
        // write align bytes to file
        char zero_byte = 0;
        for(int j = 0; j < align_bytes; j++) {
            file.write((char *)&zero_byte, sizeof(char));
        }
        if(!file.good()) {
            this->isError = true;
            throw(std::runtime_error("error writing bitmap to file. file is now corrupted"));
        }
    }

    this->isError = false;
}

void Bitmap::write(const char *fname)
{
    this->fname = std::string(fname);
    this->write();
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

bool Bitmap::errorOccured()
{
    return this->isError;
}

void Bitmap::cleanErrorFlag()
{
    this->isError = false;
}
