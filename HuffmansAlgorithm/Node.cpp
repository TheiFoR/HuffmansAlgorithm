#include "Node.h"

Node::Node() {
	letter = 0;
	letterFrequency = 0;
	Node* leftNode = 0;
	Node* rightNode = 0;
}

Node::Node(char val) {
	letter = val;
	letterFrequency = 0;
	Node* leftNode = 0;
	Node* rightNode = 0;
}

Node::~Node() {
	deleteNode(this);
}

void Node::deleteNode(Node* _root)
{
	if (_root != nullptr) {
		deleteNode(_root->leftNode);
		deleteNode(_root->rightNode);
		delete _root;
	}
}

Node* Node::getNode(char letter, int letterFrequency, Node* leftNode, Node* rightNode)
{
	Node* node = new Node();

	node->letter = letter;
	node->letterFrequency = letterFrequency;
	node->leftNode = leftNode;
	node->rightNode = rightNode;

	return node;
}
