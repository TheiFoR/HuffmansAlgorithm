#pragma once

#include "Node.h"
#include "FileWorker.h"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <bitset>

using namespace std;

class Algorithm
{
private:
	struct NodeCompare
	{
		bool operator()(Node* leftNode, Node* rightNode)
		{
			return leftNode->letterFrequency > rightNode->letterFrequency;
		}
	};
	unordered_map<char, unsigned int> _letterProbability;
	priority_queue<Node*, vector<Node*>, NodeCompare> _initialTree;
	unordered_map<char, string> _huffmanCode;

	string _input;
	string _codeBook;
	string _output;
	Node* _root;

	void encode(Node* root, string str, unordered_map<char, string>& huffmanCode, ofstream* codeBookFileName);
	void decode(Node* root, int& index, string str, string& result);

	void restoringTree(Node*& root, string codeBook);
	string read(string fileName, bool binaty = true);
	void fillingInProbability();
	void initializingTree();
	void creatingTree();
	void creatingCodeBook();
	void encodingIntoBinaryCode();
	void writeToFile(string fileName);

	string getBinaryCode(string text);
	void decodeText(string binaryText);
public:
	void coding(string inputFileName, string outputFileName);
	void decoding(string inputFileName, string outputFileName);
};

