#pragma once
class Node
{
public:
	Node();
	Node(char val);
	~Node();

	Node* getNode(char letter = 0, int letterFrequency = 0, Node* leftNode = nullptr, Node* rightNode = nullptr);
	void deleteNode(Node* _root);

	char letter;
	unsigned int letterFrequency;
	Node* leftNode;
	Node* rightNode;
};