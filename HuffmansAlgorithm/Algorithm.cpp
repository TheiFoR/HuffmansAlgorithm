#include "Algorithm.h"

#define BUFFER_BLOCK_SIZE 256

void Algorithm::coding(string inputFileName, string outputFileName)
{
	_input = read(inputFileName);
	fillingInProbability();
	initializingTree();
	creatingTree();
	creatingCodeBook();
	encodingIntoBinaryCode();
	writeToFile(outputFileName);
}

void Algorithm::decoding(string inputFileName, string outputFileName)
{
	string s1 = _output;
	_input = read(inputFileName);
	string s = _codeBook;
	_codeBook = read("CodeBook.txt");
	restoringTree(_root, _codeBook);
	decodeText(_input);
	writeToFile(outputFileName);
}

void Algorithm::encode(Node* root, string str, unordered_map<char, string>& huffmanCode, ofstream* codeBook)
{
	if (root == nullptr) {
		*codeBook << '-';
		_codeBook += '-';
		return;
	}

	if (!root->leftNode && !root->rightNode) {
		_huffmanCode[root->letter] = str;
	}

	*codeBook << (root->letter);
	_codeBook += (root->letter);
	encode(root->leftNode, str + "0", _huffmanCode, codeBook);
	encode(root->rightNode, str + "1", _huffmanCode, codeBook);
}

void Algorithm::decode(Node* root, int& index, string str, string& result)
{
	if (_root == nullptr) {
		return;
	}

	if (!_root->leftNode && !_root->rightNode)
	{
		result += _root->letter;
		return;
	}

	index++;

	if (str[index] == '0')
		decode(_root->leftNode, index, str, result);
	else
		decode(_root->rightNode, index, str, result);
}

void Algorithm::restoringTree(Node*& root, string codeBook)
{
	static int index = 0;
	if (codeBook.size() - 1 == index)
		return;

	static int specialPosition = -1;

	char value = codeBook[index++];
	if (specialPosition == -1) {
		int indexSpecialPosition = codeBook.find("---", index);
		int indexSpecialPosition1 = codeBook.find("-----", index);
		if (indexSpecialPosition == indexSpecialPosition1) {
			specialPosition = indexSpecialPosition + 2;
		}
		else if (indexSpecialPosition != -1) {
			specialPosition = indexSpecialPosition;
		}
	}

	if (value == '-' && specialPosition != index - 1)
		return;

	root = root->getNode(value);
	restoringTree(root->leftNode, codeBook);
	restoringTree(root->rightNode, codeBook);
}

string Algorithm::read(string path, bool binary){
	FileWorker file(path);
	string textFromFile;

	textFromFile = file.read(binary, BUFFER_BLOCK_SIZE);

	return textFromFile;
}

void Algorithm::fillingInProbability()
{
	for (char ch : _input) {
		++_letterProbability[ch];
	}
}

void Algorithm::initializingTree()
{
	for (auto pair : _letterProbability) {
		_initialTree.emplace(_root->getNode(pair.first, pair.second));
	}
}

void Algorithm::creatingTree()
{
	int letterFrequencyAmount;
	while (_initialTree.size() > 1)
	{
		Node* left = _initialTree.top();
		_initialTree.pop();

		Node* right = _initialTree.top();
		_initialTree.pop();

		letterFrequencyAmount = left->letterFrequency + right->letterFrequency;
		_initialTree.emplace(_root->getNode('\0', letterFrequencyAmount, left, right));
	}
}

void Algorithm::creatingCodeBook()
{
	_root = _initialTree.top();
	ofstream* codeBookFile = new ofstream();
	codeBookFile->open("CodeBook.txt", ios::binary);
	if (codeBookFile->is_open()) {
		encode(_root, "", _huffmanCode, codeBookFile);
	}
	codeBookFile->close();
}

void Algorithm::encodingIntoBinaryCode()
{
	size_t i = 0;
	char byteBuffer = 0;
	string tempResult;

	for (char ch : _input) {
		tempResult += _huffmanCode[ch];
		while (tempResult.size() - (8 * i) > 7) {
			byteBuffer = 0;
			for (int j = 0; j < 8; ++j) {
				byteBuffer += ((tempResult[(i * 8 + j)] - '0') << (7 - j));
			}
			++i;
			_output += byteBuffer;
		}
	}
	tempResult += "1";
	byteBuffer = 0;
	for (int j = 0; j < tempResult.size() - (8 * i); ++j) {
		byteBuffer += ((tempResult[i * 8 + j] - '0') << (7 - j));
	}
	_output += byteBuffer;
}

void Algorithm::writeToFile(string path)
{
	FileWorker file(path);
	file.write(_output);
}

string Algorithm::getBinaryCode(string text)
{
	string binaryRow;
	bitset<8> byte;
	for (char ch : text) {
		byte = ch;
		binaryRow += byte.to_string();
	}
	return binaryRow;
}

void Algorithm::decodeText(string binaryText)
{
	unsigned int index = 0;
	unsigned int tempCount = 0;
	unsigned int count = 0;
	unsigned int end = 0;

	string result;
	Node* tempRoot = _root;

	size_t textSize = binaryText.size();
	result.reserve(textSize);

	while ((binaryText[textSize - 1] & 0b00000001 << (end)) >> (end) != 0b00000001) {
		++end;
	}
	while (index < textSize) {
		tempCount = 7 - count;
		if (index == textSize - 0b00000001) {
			if (count == 0b00001000 - end) {
				break;
			}
		}
		if ((binaryText[index] & (0b00000001 << tempCount)) >> tempCount) {
			if (!tempRoot->rightNode) {
				result += tempRoot->letter;
				tempRoot = _root;
			}
			else
			{
				tempRoot = tempRoot->rightNode;
				++count;
			}
		}
		else {
			if (!tempRoot->leftNode) {
				result += tempRoot->letter;
				tempRoot = _root;
			}
			else
			{
				tempRoot = tempRoot->leftNode;
				++count;
			}
		}

		if (count == 0b00001000) {
			++index;
			count = 0b0;
		}
	}
	_output = result;
}
