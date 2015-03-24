#include <iostream>
#include <fstream>
#include "def.h"
#include "bmp.h"

using namespace std;


int main(int argc, char **argv)
{

    Bitmap bm("lena512.bmp");
    bm.read();


//	if (argc != 2){
//		cout << "No BMP file specified, exit." << endl;
//		return -1;
//	}

//	cout << "Opening file \"" << argv[1] << "\"" << endl;

//	ifstream file(argv[1], ios::binary);
//	if(!file.is_open()) {
//		cout << "Error opening file \"" << argv[1] << "\", exit." << endl;
//		return -2;
//	}

//	cout << "File \"" << argv[1] << "\" opened" << endl;
//	cout << "Reading file header... ";

//	file.read((char *)&file_header, sizeof(file_header));

//	if(file.gcount() != FILE_HEADER_SIZE) {
//		cout << "File header size must be "
//			 << FILE_HEADER_SIZE
//			 << " bytes, but "
//			 << file.gcount()
//			 << " bytes read. Exit."
//			 << endl;
//			 goto exit;
//	}

//	cout << "DONE" << endl;

//	cout << "Reading bitmap header... ";
//	file.read((char *)&bitmap_header, sizeof(bitmap_header));

//	if(file.gcount() != BITMAP_HEADER_SIZE) {
//		cout << "Bitmap header size must be "
//			 << BITMAP_HEADER_SIZE
//			 << " bytes, but "
//			 << file.gcount()
//			 << " bytes read. Exit."
//			 << endl;
//			 goto exit;
//	}

//	cout << "DONE" << endl;

//	cout << "Code\t\t" << file_header.bm[0] << file_header.bm[1] << endl;
//	cout << "File size\t" << file_header.size << endl;
//	cout << "Bitmap offset\t\t" << file_header.offset << endl;
	
//	cout << "Bitmap header size\t" << bitmap_header.bitmap_header_size << endl;
//	cout << "Image width\t" << bitmap_header.img_width << endl;
//	cout << "Image height\t" << bitmap_header.img_height << endl;
//	cout << "Plane count\t" << bitmap_header.plane_count << endl;
//	cout << "Bit per pixel\t" << bitmap_header.bit_per_pixel << endl;
 
//exit:
//	file.close();

	return 0;
}
