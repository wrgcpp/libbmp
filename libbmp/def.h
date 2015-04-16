#ifndef DEF_H
#define DEF_H

#include <inttypes.h>
#include <cstddef>

struct FileHeader {
	uint8_t bm[2];
	uint32_t size;
	uint16_t reserved[2];
	uint32_t offset;
} __attribute__((packed));

struct BitmapHeader {
	uint32_t bitmap_header_size;
	uint32_t img_width;
	uint32_t img_height;
	uint16_t plane_count;
    uint16_t bit_count;
	uint32_t compress_type;
	uint32_t compressed_size;
	uint32_t horizontal_resolution;
	uint32_t vertical_resolution;
	uint32_t color_count;
	uint32_t important_colors_count;
} __attribute__((packed));

struct RGB {
	uint8_t blue;
	uint8_t green;
	uint8_t red;
} __attribute__((packed));


#endif // DEF_H
