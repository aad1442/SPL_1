#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>

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

// Function to print binary pixel values of a BMP image
void printBMPBinary(const string& imageFile) {
    ifstream inputFile(imageFile, ios::binary);

    if (!inputFile) {
        cerr << "Unable to open the file: " << imageFile << endl;
        return;
    }

    BitMapHeader bitmapHeader;
    DIBHeader dibHeader;
    RGB pixel;

    // Read BMP header and DIB header
    inputFile.read(reinterpret_cast<char*>(&bitmapHeader), sizeof(bitmapHeader));
    inputFile.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));

    int height = dibHeader.height;
    int width = dibHeader.width;

    // Move to the pixel data
    inputFile.seekg(bitmapHeader.imageOffset, ios::beg);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Read pixel data
            inputFile.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));

            // Print binary values of the pixel's color channels
            cout << "Pixel at (" << i << ", " << j << "): ";
            cout << bitset<8>(pixel.red) << " ";
            cout << bitset<8>(pixel.green) << " ";
            cout << bitset<8>(pixel.blue) << endl;
        }
    }

    inputFile.close();
}

int main() {
    string imageFile = "BMP1.bmp";  // Replace with the path to your BMP image
    printBMPBinary(imageFile);

    return 0;
}
