#include <iostream>
#include <fstream>
#include "def.h"
#include "bmp.h"

using namespace std;


int main(int argc, char **argv)
{

    Bitmap bm("test.bmp");
    if(!bm.read()) {
        while(bm.hasErrors()) {
            cout << bm.getLastError().text() << endl;
        }
    } else {
        cout << "OK" << endl;
        //bm.print();
        bm.write("test2.bmp");
    }

	return 0;
}
