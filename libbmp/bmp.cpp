#include "bmp.h"

Bitmap::Bitmap()
{
    this->palette = NULL;
}

Bitmap::Bitmap(char const *fname)
{
    this->fname = std::string(fname);
    this->palette = NULL;

}

Bitmap::~Bitmap()
{
    delete [] this->palette;
    this->palette = NULL;
}

void Bitmap::setFilename(const char *fname)
{
    this->fname = std::string(fname);
}

void Bitmap::read()
{
    if(!this->fname.length()){
        //TODO: throw error: no file name set
        return;
    }

    // open file
    std::ifstream file;
    file.open(fname, std::ios::binary);
    if(!file.is_open()) {
        //TODO: throw error
        std::cout << "error opening file" << std::endl;
        return;
    }

    // read both headers
    file.read((char *)&this->file_header, sizeof(this->file_header));
    if(file.gcount() != this->file_header_size) {
        //TODO: throw error (wrong file header size)
        DEBUG("wrong file header size");
        return;
    }

    file.read((char *)&this->bitmap_header, sizeof(this->bitmap_header));
    if(file.gcount() != this->bmp_header_size) {
        //TODO: throw error (wrong bitmap header size)
        DEBUG("wrong bitmap header size");
        DEBUG(file.gcount());
        return;
    }

    // check that this is a BMP file
    //int t = 0;
    //t += memcmp(this->file_header.bm, "BM", 2);
    if(memcmp(this->file_header.bm, "BM", 2)){
        //TODO: throw error (not BMP file)
        DEBUG("not BMP file");
        return;
    }

    DEBUG("BMP file");
    DEBUG("Image offset: " << this->file_header.offset);

    unsigned int palette_size = this->file_header.offset - 54;
    if(palette_size) {
        // check palette size
        if(palette_size % 4) {
            // TODO: throw error (wrong palette size or wrong offset)
            return;
        }
        // allocate memory for palette
        this->palette = new RGBQuad[palette_size / sizeof(RGBQuad)];

        // read palette
        DEBUG("reading palette (" << palette_size << " bytes)");

        for(int i = 0; i < palette_size / 4; i++) {
            file.read((char *)&this->palette[i], sizeof(RGBQuad));
        }
    }

    file.close();
}

void Bitmap::read(const char *fname)
{
    this->fname = std::string(fname);
    this->read();
}

