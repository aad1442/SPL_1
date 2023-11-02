#include <iomanip>
#include <fstream>

//#include "header.h"

using namespace std;

struct BitMapHeader{
    char name[2];   /// this should be equal BM
    unsigned int Size{0};   /// os sizeof(int)=4 bytes,
    unsigned short int reserved1{0};    /// this is not required
    unsigned short int reserved2{0};    /// this is not required
    unsigned int imageOffset{0};
};

struct DIBHeader{
    unsigned int headerSize{0};   /// header size
    unsigned int height{0};
    unsigned int width{0};
    unsigned short int colorPlanes{1};
    unsigned short int bitsPerPixel{0};
    unsigned int compression{0};
    unsigned int imageSize{0};
    unsigned int pixelPerMeterX{0}; //pixel per meter along x axis
    unsigned int pixelPerMeterY{0}; //pixel per meter along y axis
    unsigned int colorsInColorTable{0}; //number of colors in color table
    unsigned int importantcolorCount{0}; //number of colors used
};

struct RGB{
    unsigned int blue{0x000000ff};
    unsigned int green{0x0000ff00};
    unsigned int red{0x00ff0000 };
   // unsigned int alpha{0xff000000}; //alpha channel bitmask
    //unsigned int color{0x73524742};    //color space type usually sRGB default
};

//contains bmp color table information
struct bmpColorTable
{
    unsigned int colorDefinition[256];
};


/*struct RGB{           /// pixel information
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    //unsigned char others;

};*/

struct Image{          /// image info
    int height;
    int width;
    struct RGB **rgb;
};