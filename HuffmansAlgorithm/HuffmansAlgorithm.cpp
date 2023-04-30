#include <iostream>
#include "FileWorker.h"
#include "Algorithm.h"

using namespace std;

int main(int argc, char* name[])
{
	Algorithm huffman;
	vector<string> fileNames;

    uint8_t mode = 0;

    if (argc == 5)
    {
        fileNames.emplace_back(name[1]);
        fileNames.emplace_back(name[2]);
        fileNames.emplace_back(name[3]);
        if (*name[4] == '0') {
            mode = 0;
        }
        else {
            mode = 1;
        }
    }
    else
    {
        string text;
        cout << "Enter the mode:\n0 - coding\n1 - decoding\n";
        while (true) {
            cin >> text;
            if (text.length() == 1 && text[0] - '0' >= 0 && text[0] - '0' <= 1)
                break;
            else
                cout << "Invalid mode. Try again!\n";

        }
        mode = text[0] - '0';
        cout << "Enter the name of the input file:";
        cin >> text;
        fileNames.emplace_back(text);
        cout << "Enter the name of the output file:";
        cin >> text;
        fileNames.emplace_back(text);
    }

    if (!mode) {
        huffman.coding(fileNames[0], fileNames[1]);
    }
    else {
        huffman.decoding(fileNames[0], fileNames[1]);
    }
}