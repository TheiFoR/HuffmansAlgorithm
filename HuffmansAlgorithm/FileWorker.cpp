#include "FileWorker.h"

FileWorker::FileWorker()
{
	_file = new fstream();
}

FileWorker::FileWorker(string path)
{
	_file = new fstream();

	replace(path.begin(), path.end(), '\\', '/');
	path.erase(std::remove(path.begin(), path.end(), '\"'), path.end());

	_path = path;
}

FileWorker::~FileWorker()
{
	delete _file;
}

void FileWorker::open(bool isRead, bool binary)
{
	if (binary) {
		_file->open(_path, (isRead ? std::ios::in : std::ios::out) | ios::binary);
	}
	else {
		_file->open(_path, isRead ? std::ios::in : std::ios::out);
	}
}

std::string FileWorker::read(bool binary, uint16_t blockSize)
{
	string result;
	
	if (!_file->is_open()) {
		_mode = 1;
		open(1, binary);
	}

	if (_file->is_open())
	{
		if (blockSize == 1) {
			while (!_file->eof()) {
				result += _file->get();
			}
		}
		else
		{
			char* buffer = new char[blockSize + 1];
			buffer[blockSize] = 0;

			_file->seekg(0, ios::end);
			size_t fileSize = _file->tellg();
			_file->seekg(0);

			int numberOfCompleteBlocks = fileSize / blockSize;

			for (int i = 0; i < numberOfCompleteBlocks; ++i) {
				_file->read(buffer, blockSize);
				result.append(buffer, blockSize);
			}

			numberOfCompleteBlocks = fileSize % blockSize;

			_file->read(buffer, numberOfCompleteBlocks);
			buffer[numberOfCompleteBlocks] = 0;

			result.append(buffer, numberOfCompleteBlocks);
			
			delete[] buffer;

			if (!result.empty() && result[result.size() - 1] == -1)
				result.erase(prev(result.end()));
		}
	}

	if (_mode) {
		_mode = 0;
		_file->close();
	}

	return result;
}

void FileWorker::write(string text, bool binary)
{
	if (!_file->is_open()) {
		_mode = 1;
		open(0, binary);
	}

	if (_file->is_open())
	{
		*_file << text;
	}

	if (_mode) {
		_mode = 0;
		_file->close();
	}
}

void FileWorker::close()
{
	_file->close();
}

void FileWorker::setPath(string path)
{
	replace(path.begin(), path.end(), '\\', '/');
	path.erase(std::remove(path.begin(), path.end(), '\"'), path.end());

	_path = path;
}
