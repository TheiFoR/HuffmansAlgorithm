#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class FileWorker
{
public:
	FileWorker();
	FileWorker(string path);
	~FileWorker();

	void open(bool isRead = true, bool binary = true);
	string read(bool binary = true, uint16_t blockSize = 1);
	void write(string text, bool binary = true);
	void close();

	void setPath(string path);
private:
	fstream* _file;
	string _path;

	uint8_t _mode = 0;
};