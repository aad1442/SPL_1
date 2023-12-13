#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <time.h>
#include <thread>
#include <sstream>
#include "Astegno.cpp"
#include "serverRun.cpp"
#include "Ahashchecking.cpp"
#include "Asha.cpp"

int main()
{

    bool continueLoop = true;
    while (continueLoop)
    {
        cout << endl
             << endl
             << "What do you want to do ?" << endl;
        cout << "1.Data Hiding," << endl;
        cout << "2.Data Extracting" << endl;
        cout << "0.Exit" << endl;

        int choice;
        cout << endl
             << "Enter your choice:";
        cin >> choice;

        if (choice == 1)
        {
            string imageFileName;
            string textFileName;
            string extendedTextFileName;

            cout << "provide image file name";
            cout << "\n";

            cin >> imageFileName;
            string extendedImageFileName = addImageFileExtension(imageFileName);
            openingImage(extendedImageFileName);

            cout << "Write Here :";

            extendedTextFileName = "input.txt";
            processInputText(extendedTextFileName);
            /** checking opening issue of image file**/
            ifstream inputFile;
            inputFile.open(extendedImageFileName, ios::binary);

            if (!inputFile)
            {
                cout << "couldn't find the image file in storage\n";
                cout << "redirecting to the option menu\n\n";
                continue;
            }

            inputFile.close();

            if (!checkingImageFormat(extendedImageFileName))
            {
                cout << "sorry, the image format is not correct.\n";
                cout << "redirecting to the option menu.\n\n";
                continue;
            }

            /** checking if the text file is compatible for steganography **/ // correction

            ifstream inputfile1;
            inputfile1.open(extendedTextFileName, ios::binary);

            if (!inputfile1)
            {
                cout << "couldn't load the text file\n";
                cout << "redirecting to the option menu\n\n";
                continue;
            }

            if (!checkingTextFile(extendedImageFileName, extendedTextFileName))
            {
                cout << "not possible to hide the text file within provided image file.";
                cout << "redirecting to the option menu.\n\n";
                continue;
            }

            inputfile1.close();

            cout << "Do you want to create a hash of your input ? 1.Yes, 2.No" << endl
                 << ":";
            int h;
            cin >> h;
            if (h == 1)
            {
                string hashResult = generateSHA512Hash("input.txt", "hashOfInput.txt");
            }

            string stegoImage;
            if ((stegoImage = hidingData(extendedImageFileName, extendedTextFileName)) != " ")
            {
                puts("stego image is ready");
                cout << "\n\n";
            }
            else
            {
                puts("failed to create stego image");
                continue;
            }

            // Sleep(4000);
            // openingImage(stegoImage);

            int press;

            cout << endl
                 << "Press 1 to send the file: ";
            cin >> press;
            if (press == 1)
            {

                if (fork() == 0)
                {
                    // Child process for server
                    runServer();
                    exit(0);
                }

                if (fork() == 0)
                {
                    // Child process for client
                    runClient();
                    exit(0);
                }

                // Parent process waits for both children to finish
                wait(nullptr);
                wait(nullptr);
            }
            else
            {
                cout << "Invalid option";
                cout << "\n\n";

                return EXIT_FAILURE;
            }
        }
        else if (choice == 2)
        {
            string stegoImageFileName;

            cout << "provide stego image file name";
            cout << "\n";

            cin >> stegoImageFileName;
            string extendedStegoImageFileName = addImageFileExtension(stegoImageFileName);

            ifstream inputFile;

            inputFile.open(extendedStegoImageFileName, ios::binary);

            if (!inputFile)
            {
                cout << "couldn't find the image file in storage\n";
                cout << "redirecting to the option menu\n\n";
                continue;
            }

            inputFile.close();

            if (!checkingImageFormat(extendedStegoImageFileName))
            {
                cout << "sorry, the image format is not correct.\n";
                cout << "redirecting to the option menu.\n\n";
                continue;
            }

            extractingData(extendedStegoImageFileName);
            int h;

            cout << "Do you want to generate hash of the message? 1.Yes, 2.No " << endl
                 << ":";
            cin >> h;
            if (h == 1)
            {
                string hashResult = generateSHA512Hash("hidden_msg.txt", "hashOfOutput.txt");
            }

            cout << "Do you want to check you message correctness ? 1.Yes, 2.No" << endl
                 << ":";
            int c;
            cin >> c;
            if (c == 1)
            {
                if (compareFiles("hashOfInput.txt", "hashOfOutput.txt"))
                {
                    std::cout << "Both files have the same content." << std::endl;
                }
                else
                {
                    std::cout << "The contents of the files are different." << std::endl;
                }
            }

            continueLoop = false;
        }
        else if (choice == 0)
        {
            continueLoop = false;
        }
        else
        {
            cout << "Invalid option";
            cout << "\n\n";

            return EXIT_FAILURE;
        }
    }
}