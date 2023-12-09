#include<bits/stdc++.h>
#include<iostream>
#include<fstream>
#include<time.h>
#include <cstdlib>
//#include "header.h"
#include<time.h>
#include <thread>
#include<sstream>

using namespace std;

struct BMPSignature{
    unsigned char name[2];   /** BMP image signature, equal to=BM **/
};

struct BitMapHeader{
    unsigned int Size{0};   /** sizeOf(int)=4 bytes**/
    unsigned short int reserved1{0};    /** this is not required **/
    unsigned short int reserved2{0};    /** this is not required **/
    unsigned int imageOffset{0};
};

struct DIBHeader{
    unsigned int headerSize{0};   /** header size **/
    unsigned int height{0};
    unsigned int width{0};
    unsigned short int colorPlanes{1};
    unsigned short int bitsPerPixel{0};
    unsigned int compression{0};
    unsigned int imageSize{0};
    unsigned int pixelPerMeterX{0};     /** pixel per meter along x axis **/
    unsigned int pixelPerMeterY{0};     /** pixel per meter along y axis **/
    unsigned int colorsInColorTable{0};     /** number of colors in color table **/
    unsigned int importantcolorCount{0};    /** number of colors used **/
};

struct RGB{
    unsigned int blue{0x000000ff};
    unsigned int green{0x0000ff00};
    unsigned int red{0x00ff0000};
    unsigned int alpha{0xff000000}; /** alpha channel bitmask **/
    unsigned int color{0x73524742};    /** color space type usually sRGB default **/
};


/** contains bmp color table information **/
struct bmpColorTable
{
    unsigned int colorDefinition[256];
};

struct Image{          /** image info if needed **/
    int height;
    int width;
    struct RGB **rgb;
};

int checkingImageFormat(string fileName);
int checkingTextFile(string imageFile,string textFile);
string addImageFileExtension(string imageFileName);
string addTextFileExtension(string textFileName);
string hidingData(string imageFile,string textFile);
vector<int> textToBinary(string textFile);
void extractingData(string imageFile);
vector<int> decimalToBinary(int decimalValue);
int binaryToDecimal(int binArray[],int length);
void openingImage(string fileName);
void processInputText(string textFile);


int checkingImageFormat(string imageFile)
{
    ifstream inputFile;
    inputFile.open(imageFile.c_str(),ios::binary);


    struct BMPSignature bmpsignature;
    struct BitMapHeader bitmapheader;
    struct DIBHeader dibheader;

    inputFile.read((char *)&bmpsignature,sizeof(bmpsignature));
    inputFile.read((char *)&bitmapheader,sizeof(bitmapheader));
    inputFile.read((char *)&dibheader,sizeof(dibheader));

    inputFile.close();


    /** case 01 **/
    /** checking whether a bmp image or not **/

    if(bmpsignature.name[0]!='B' or bmpsignature.name[1]!='M')
    {
        return 0;
    }


    /** case 02 **/
    /** checking colorplanes, bitsperpixel and compression value.
        for bmp 24 bit color,
            -> colorplanes=1,
            -> bitsperpixel=24,
            -> compression value=0
    **/

    if(dibheader.colorPlanes!=1 or dibheader.bitsPerPixel!=24 or dibheader.compression!=0)
    {
        return 0;
    }

    return 1;
}
int checkingTextFile(string imageFile,string textFile)
{
    /** gaining image height and width **/
    ifstream inputFile;
    inputFile.open(imageFile.c_str(),ios::binary);

    struct BMPSignature bmpsignature;
    struct BitMapHeader bitmapheader;
    struct DIBHeader dibheader;

    inputFile.read((char *)&bmpsignature,sizeof(bmpsignature));
    inputFile.read((char *)&bitmapheader,sizeof(bitmapheader));
    inputFile.read((char *)&dibheader,sizeof(dibheader));

    int imageHeight=dibheader.height;
    int imageWidth=dibheader.width;


    /** text file section **/

    FILE *fp1=fopen(textFile.c_str(),"r");
    if(fp1==NULL)
    {
        printf("Error opening file %s", textFile.c_str());
        return 0;
    }

    /** checking text file size **/
    long long int textFileSize;
    int countCharacter=0;
    char ch;

    while((ch=fgetc(fp1)!=EOF))
    {
        countCharacter++;
    }

    fclose(fp1);


    textFileSize=countCharacter*8;

    /** checking available space in bits for hiding these text file.**/

    if((imageHeight * imageWidth *3)>(textFileSize+32))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
string addImageFileExtension(string imageFileName)
{
    imageFileName+='.';
    imageFileName+='b';
    imageFileName+='m';
    imageFileName+='p';

    return imageFileName;
}
string addTextFileExtension(string textFileName)
{
    textFileName+='.';
    textFileName+='t';
    textFileName+='x';
    textFileName+='t';

    return textFileName;
}
string hidingData(string imageFile,string textFile)
{
    /** text file to binary stream **/

    vector<int> binaryStream=textToBinary(textFile);
    //cout<<binaryStream.size()<<"\n";
    /* for(int i=1;i<=binaryStream.size();i++)
     {
         cout<<binaryStream[i-1]<<" ";
         if(i%8==0) puts("");
     }*/


    /** image read and copy information **/

    struct BitMapHeader bitmapheader;
    struct DIBHeader dibheader;
    struct RGB rgb;
    struct bmpColorTable bmpcolortable;
    struct BMPSignature bmpsignature;

    ifstream inputFile;
    ofstream outputFile;

    // modification
    string outputImage="stegoBMP";
    for(int i=imageFile.size()-1;i>-1;i--)
    {
        if(imageFile[i]=='.')
        {
            outputImage+=imageFile[i-1];
            break;
        }
    }

    // including extension
    outputImage+='.';
    outputImage+='b';
    outputImage+='m';
    outputImage+='p';

    inputFile.open(imageFile,ios::binary);
    outputFile.open(outputImage,ios::binary);

    inputFile.read((char *)&bmpsignature,sizeof(bmpsignature));
    inputFile.read((char *)&bitmapheader,sizeof(bitmapheader));
    inputFile.read((char *)&dibheader,sizeof(dibheader));
    inputFile.read((char *)&rgb,sizeof(rgb));

    outputFile.write((char *)&bmpsignature,sizeof(bmpsignature));
    outputFile.write((char *)&bitmapheader,sizeof(bitmapheader));
    outputFile.write((char *)&dibheader,sizeof(dibheader));
    outputFile.write((char *)&rgb,sizeof(rgb));

    int height=dibheader.height;
    int width=dibheader.width;


    int*** pixels = new int** [height];
    for(int i = 0; i<height; i++)
    {
        pixels[i] = new int* [width];
        for(int j = 0; j<width; j++)
        {
            pixels[i][j] = new int[3];
        }
    }

    inputFile.seekg(bitmapheader.imageOffset,ios::beg);
    outputFile.seekp(bitmapheader.imageOffset,ios::beg);

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            unsigned char color[3];
            inputFile.read(reinterpret_cast<char*>(color),3);
            pixels[i][j][0] = static_cast<float>(color[2]); //red
            pixels[i][j][1] = static_cast<float>(color[1]); //green
            pixels[i][j][2] = static_cast<float>(color[0]); //blue
        }
    }


    /** doing pixel decimal value to bin and embedding text binary value together **/
    int p=0;
    int sizeOfStream=binaryStream.size();
    vector<int>flagStreams=decimalToBinary(sizeOfStream);
    bool ok=false;


    /** at first embedding the text file size **/

    int a,b,c,d;

    for(a=0; a<height; a++)
    {
        for(b=0; b<width; b++)
        {
            d=0;
            while(d<3)
            {
                if(p==32)
                {
                    ok=true;
                    break;
                }
                int quotient=pixels[a][b][d];
                //cout<<"#"<<pixels[a][b][d]<<"\n";
                int remainder;
                int binary[8];
                int k=7;
                while(quotient)
                {
                    remainder=quotient%2;
                    binary[k]=remainder;
                    k--;
                    quotient/=2;
                }

                while(k>=0)
                {
                    binary[k]=0;
                    k--;
                }

                binary[7]=flagStreams[p];
                p++;

                /** bin to int and embedding**/
                int power=0;
                pixels[a][b][d]=0;
                for(int t=0; t<8; t++)
                {
                    pixels[a][b][d]+=(binary[t]*pow(2,(7-t)));
                }

                d++;
            }
            if(ok)
                break;
        }
        if(ok)
            break;
    }

    /** for data **/
    p=0;
    bool cnt=true;
    ok=false;

    for(int i=a; i<height; i++)
    {
        for(int j=b; j<width; j++)
        {
            if(cnt==true and d!=0)
            {
                c=d;
                cnt=false;
            }
            else
            {
                c=0;
            }
            while(c<3)
            {
                if(p==sizeOfStream)
                {
                    ok=true;
                    break;
                }
                int quotient=pixels[i][j][c];
                //cout<<"#hiding "<<quotient;
                int remainder;
                int binary[8];
                int k=7;
                while(quotient)
                {
                    remainder=quotient%2;
                    binary[k]=remainder;
                    k--;
                    quotient/=2;
                }

                while(k>=0)
                {
                    binary[k]=0;
                    k--;
                }

                binary[7]=binaryStream[p];
                p++;

                /** bin to int and embedding **/
                int power=0;
                pixels[i][j][c]=0;

                for(int t=0; t<8; t++)
                {
                    pixels[i][j][c]+=(binary[t]*pow(2,(7-t)));
                }

                //cout<<"  new #hiding "<<pixels[a][b][c]<<"\n";
                c++;
            }
            if(ok)
                break;
        }
        if(ok)
            break;
    }


    /** writing the image **/
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            unsigned char color[3];
            color[0] = static_cast<unsigned char>(pixels[i][j][2]);
            color[1] = static_cast<unsigned char>(pixels[i][j][1]);
            color[2] = static_cast<unsigned char>(pixels[i][j][0]);
            outputFile.write(reinterpret_cast<char*>(color),3);
        }
    }

    inputFile.read((char*)&bmpcolortable,sizeof(bmpcolortable));
    outputFile.write((char*)&bmpcolortable,sizeof(bmpcolortable));

    inputFile.close();
    outputFile.close();

    return outputImage;
}
void extractingData(string imageFile)
{
    /** image read and copy information **/

    struct BitMapHeader bitmapheader;
    struct DIBHeader dibheader;
    struct RGB rgb;
    struct bmpColorTable bmpcolortable;
    struct BMPSignature bmpsignature;

    ifstream inputFile;

    inputFile.open(imageFile,ios::binary);

    inputFile.read((char *)&bmpsignature,sizeof(bmpsignature));
    inputFile.read((char *)&bitmapheader,sizeof(bitmapheader));
    inputFile.read((char *)&dibheader,sizeof(dibheader));
    inputFile.read((char *)&rgb,sizeof(rgb));


    int height=dibheader.height;
    int width=dibheader.width;

    int*** pixels2 = new int** [height];
    for(int i = 0; i<height; i++)
    {
        pixels2[i] = new int* [width];
        for(int j = 0; j<width; j++)
        {
            pixels2[i][j] = new int[3];
        }
    }

    inputFile.seekg(bitmapheader.imageOffset,ios::beg);

    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width; j++)
        {
            unsigned char color[3];
            inputFile.read(reinterpret_cast<char*>(color),3);
            pixels2[i][j][0] = static_cast<float>(color[2]); //red
            pixels2[i][j][1] = static_cast<float>(color[1]); //green
            pixels2[i][j][2] = static_cast<float>(color[0]); //blue
        }
    }


    /** doing pixel decimal value to binary and extracting info **/
    int p=0;

    bool ok=false;

    int a,b,c,d;
    int tempBin[32];
    for(a=0; a<height; a++)
    {
        for(b=0; b<width; b++)
        {
            d=0;
            while(d<3)
            {
                if(p==32)
                {
                    ok=true;
                    break;
                }
                int quotient=pixels2[a][b][d];
               // cout<<"*"<<quotient<<"\n";
                int remainder;
                int binary[8];
                int k=7;
                while(quotient)
                {
                    remainder=quotient%2;
                    binary[k]=remainder;
                    k--;
                    quotient/=2;
                }

                while(k>=0)
                {
                    binary[k]=0;
                    k--;
                }

                tempBin[p]=binary[7];
                p++;

                /** bin to int, leaving the stego image as it is **/
                int power=0;
                pixels2[a][b][d]=0;
                //cout<<"show now"<<" "<<pixels[i][j][c]<<"\n";
                for(int t=0; t<8; t++)
                {
                    pixels2[a][b][d]+=(binary[t]*pow(2,(7-t)));
                }

                d++;
            }
            if(ok)
                break;
        }
        if(ok)
            break;
    }

    int countOfBits=binaryToDecimal(tempBin,32);
   // cout<<countOfBits<<" size \n";
    //cout<<"a b d"<<a<<b<<" "<<d<<"\n";

   // cout<<"show "<<countOfBits<<"\n";
    if(countOfBits<=0 or countOfBits>((height*width*3)-32))
        cout<<"No message is hidden in this image\n\n";

    else{
    /** retrieving data **/
    bool cnt=true;
    ok=false;
    p=0;
    vector<int>binaryValue;
    int octet=0;
    countOfBits/=8;
    int decimalValueArray[countOfBits]={0};
    int track=0;
    vector<char>storeCharacter;
    countOfBits*=8;


    for(int i=a; i<height; i++)
    {
        for(int j=b; j<width; j++)
        {
            if(cnt==true and d!=0)
            {
                c=d;
                cnt=false;
            }
            else
            {
                c=0;
            }
            while(c<3)
            {
                if(p==countOfBits)
                {
                    ok=true;
                    break;
                }

                int quotient=pixels2[i][j][c];
              //  cout<<"#extracting "<<quotient<<" ";
                int remainder;
                int binary[8];
                int k=7;
                while(quotient)
                {
                    remainder=quotient%2;
                    binary[k]=remainder;
                    k--;
                    quotient/=2;
                }

                while(k>=0)
                {
                    binary[k]=0;
                    k--;
                }

                binaryValue.push_back(binary[7]);
                p++;
                octet++;

                /** keep the stego image as it is **/
                pixels2[i][j][c]=0;

                for(int t=0; t<8; t++)
                {
                    pixels2[i][j][c]+=(binary[t]*pow(2,(7-t)));
                }

                if(octet==8)
                {
                    octet=0;
                    /*for(int t=0;t<binaryValue.size();t++)
                    {
                        cout<<binaryValue[t]<<" ";
                    }
                    puts("");*/

                    for(int t=0; t<8; t++)
                    {
                        decimalValueArray[track]+=(binaryValue[t]*pow(2,(7-t)));
                    }

                    //cout<<decimalValueArray[track]<<" value\n";
                    char ch=decimalValueArray[track];
                    storeCharacter.push_back(ch);
                    track++;
                    binaryValue.clear();
                }
                //cout<<" vector size "<<binaryValue.size()<<"\n";
                c++;
            }
            if(ok)
                break;
        }
        if(ok)
            break;
    }

    string hiddenMessage;

    for(int i=0; i<storeCharacter.size(); i++)
    {
        hiddenMessage+=storeCharacter[i];
    }

    puts("The hidden message : ");

    cout<<hiddenMessage<<"\n";
    cout<<"\n\n";
    }
}
vector<int> decimalToBinary(int decimalValue)
{
  //  cout<<decimalValue<<"\n";
    int storeBianry[32];
    int k=31;
    int remainder;

    while(decimalValue)
    {
        remainder=decimalValue%2;
        storeBianry[k]=remainder;
        decimalValue/=2;
        k--;
    }

    while(k>=0)
    {
        storeBianry[k]=0;
        k--;
    }

    /*for(int i=0;i<32;i++)
        cout<<storeBianry[i]<<" ";
        puts("");*/

    vector<int>result;

    for(int i=0;i<32;i++)
        result.push_back(storeBianry[i]);

    return result;
}
int binaryToDecimal(int binArray[],int length)
{
    int decimalValue=0;

   /* for(int i=0;i<length;i++)
        cout<<binArray[i]<<" ";*/

    for(int t=0; t<length; t++)
    {
        if(binArray[t])
            decimalValue+=(binArray[t]*pow(2,(31-t)));
    }

    return decimalValue;
}
void openingImage(string fileName){

// Replace "path/to/animated.gif" with the actual path to the animated GIF file.
     string file_path = "C:\\Users\\ASUS\\Desktop\\DailyPractice\\practice\\AAAAAa\\1_9dec23\\SPL-01-\\a\\";
     //C:\Users\ASUS\Desktop\BMPpic
     //C:\Users\ASUS\Desktop\Daily Practice\practice\AAAAAa\1_9dec23\SPL-01-\a

    // Build the command to open the file with the Windows Photos app.
    string command = "start " + file_path+fileName;

    // Use the system command to execute the command and open the file with the Windows Photos app.
    system(command.c_str());
}
void processInputText(string textFile)
{
    FILE *fp=fopen(textFile.c_str(),"w");
    if(fp==NULL)
    {
        printf("Error in creating file %s", textFile.c_str());
        return exit(1);
    }

    getchar();

    string text;
    getline(cin,text);

   // char* s;
    //s=text;

    const char* s = text.c_str();

    while(*s)
    {
        if(fputc(*s,fp)==EOF)
        {
            cout<<"error in writing file";
            cout<<"\n";
            EXIT_FAILURE;
        }
        s++;
    }

    fclose(fp);

}
vector<int> textToBinary(string textFile)
{
    /** file opening **/
    FILE *fp1=fopen(textFile.c_str(),"r");

    /** checking text file size **/
    long long int textFileSize;

    string text;
    int countCharacter=0;
    char ch;
    while((ch=fgetc(fp1)!=EOF))
    {
       countCharacter++;
    }

    textFileSize=countCharacter*8;

    /*cout<<"text file size in bits"<<"\n";
    cout<<"size: "<<textFileSize<<"\n";
    cout<<"total character: "<<countCharacter<<"\n";*/


    /** section of conversion **/
    rewind(fp1);
   // cout<<ftell(fp)<<"\n";
    int decimal=0;
    int index=0;
    int quotient;
    int remainder;
    int i;
    vector<int>bits;

    while((ch=fgetc(fp1))!=EOF)
    {
        decimal=ch;
        quotient=decimal;
        i=7;
        int tempArray[8];

        while(quotient)
        {
            remainder=quotient%2;
            tempArray[i]=remainder;
            i--;
            quotient/=2;
        }

        while(i>=0)
        {
            tempArray[i]=0;
            i--;
        }

       //cout<<ch<<" ";
        for(int t=0;t<8;t++)
        {
            bits.push_back(tempArray[t]);
        }
    }

    return bits;
}


int main()
{

    cout<<"**********************************************";
    cout<<"**************************************************************************\n\n";
    cout<<"\t\t\t\t\t\t  HashSteganoValult  \t\t\t\t";
    cout<<"\n\n";
    cout<<"**********************************************";
    cout<<"**************************************************************************\n\n";

    cout<<"\t\t\t\t\t   Here are my project features:\n";
    cout<<"\t\t\t\t\t  _______________________________";
    cout<<"\n\n\n";

    cout<<"          \t--------------------------\t\t\t";
    cout<<"        --------------------------\n";
    cout<<"          \t|      Invisible Ink     |\t\t\t";
    cout<<"        | Animated Frame Encoder | \n";
    cout<<"          \t--------------------------\t\t\t";
    cout<<"        --------------------------\n";

    cout<<"             \t   |                  |\t\t\t\t\t\t     |\n";
    cout<<"             \t   |                  |\t\t\t\t\t\t     |\n";
    cout<<"             \t   |                  |\t\t\t\t\t\t     |\n";
    cout<<"             \t   |                  |\t\t\t\t\t\t     |\n";

    cout<<"          ---------------      ";
    cout<<" -------------------\t\t\t";
    cout<<"    --------------------\n";
    cout<<"          | Data Hiding |     ";
    cout<<"  | Data Extracting |\t\t\t";
    cout<<"    |    GIF Encoder   |\n";
    cout<<"          ---------------      ";
    cout<<" -------------------\t\t\t";
    cout<<"    --------------------\n";

    bool continueLoop=true;

    while(continueLoop)
    {
        cout<<"\nSelect an option to proceed: \n\n";
        cout<<"press 1 for part I:  Invisible Ink\n";
        cout<<"press 2 for part II: Animated Frame Encoder\n";
        cout<<"press 0 to quit\n";

        int option;
        cin>>option;

        if(option==1)
        {
            cout<<"press 1 for Data Hiding\n";
            cout<<"press 2 for Data Extracting\n";
            int choice;
            cin>>choice;

            if(choice==1)
            {
                string imageFileName;
                string textFileName;
                string extendedTextFileName;

                cout<<"provide image file name";
                cout<<"\n";

                cin>>imageFileName;
                string extendedImageFileName=addImageFileExtension(imageFileName);
                openingImage(extendedImageFileName);
                cout<<"image file opened successfully.   "<<endl;
                // option for text file
                bool looping=true;

                while(looping)
                {
                    cout<<"How do you want to provide hidden data?";
                    cout<<"\n";
                    cout<<"\t--------------------\t\t\t---------------------------\n";
                    cout<<"\t|     Write Now    |\t\t\t|  Provide Text File Name |\n";
                    cout<<"\t--------------------\t\t\t---------------------------\n";
                    cout<<"\n";
                    cout<<"press 1 for Writing Now";
                    cout<<"\n";
                    cout<<"press 2 for Providing Text File Name";
                    cout<<"\n";

                    int pressingValue;
                    cin>>pressingValue;

                    if(pressingValue==1)
                    {
                        extendedTextFileName="t.txt";
                        processInputText(extendedTextFileName);
                    }
                    else if(pressingValue==2)
                    {
                        cin>>textFileName;
                        extendedTextFileName=addTextFileExtension(textFileName);
                    }
                    else
                    {
                        continue;
                    }
                    looping=false;
                }

                /** checking opening issue of image file**/
                ifstream inputFile;
                inputFile.open(extendedImageFileName,ios:: binary);

                if(!inputFile)
                {
                    cout<<"couldn't find the image file in storage\n";
                    cout<<"redirecting to the option menu\n\n";
                    continue;
                }

                inputFile.close();


                if(!checkingImageFormat(extendedImageFileName))
                {
                    cout<<"sorry, the image format is not correct.\n";
                    cout<<"redirecting to the option menu.\n\n";
                    continue;
                }

                // openingImage(extendedImageFileName);

                /** checking if the text file is compatible for steganography **/   // correction

                ifstream inputfile1;
                inputfile1.open(extendedTextFileName,ios:: binary);

                if(!inputfile1)
                {
                    cout<<"couldn't load the text file\n";
                    cout<<"redirecting to the option menu\n\n";
                    continue;
                }


                if(!checkingTextFile(extendedImageFileName,extendedTextFileName))
                {
                    cout<<"not possible to hide the text file within provided image file.";
                    cout<<"redirecting to the option menu.\n\n";
                    continue;
                }

                inputfile1.close();

                string stegoImage;
                if((stegoImage=hidingData(extendedImageFileName,extendedTextFileName))!=" ")
                {
                    puts("stego image is ready");
                    cout<<"\n\n";
                }
                else
                {
                    puts("failed to create stego image");
                    continue;
                }

               // Sleep(4000);
                openingImage(stegoImage);

            }
            else if(choice==2)
            {
                string stegoImageFileName;

                cout<<"provide stego image file name";
                cout<<"\n";

                cin>>stegoImageFileName;
                string extendedStegoImageFileName=addImageFileExtension(stegoImageFileName);

                ifstream inputFile;

                inputFile.open(extendedStegoImageFileName,ios:: binary);

                if(!inputFile)
                {
                    cout<<"couldn't find the image file in storage\n";
                    cout<<"redirecting to the option menu\n\n";
                    continue;
                }

                inputFile.close();


                if(!checkingImageFormat(extendedStegoImageFileName))
                {
                    cout<<"sorry, the image format is not correct.\n";
                    cout<<"redirecting to the option menu.\n\n";
                    continue;
                }

                extractingData(extendedStegoImageFileName);
                //Sleep(25000);

            }
        }
        
        else if(option==0)
        {
            continueLoop=false;
        }
        else
        {
            cout<<"Invalid option";
            cout<<"\n\n";

            return EXIT_FAILURE;
        }
    }
}