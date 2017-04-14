// Node stub
/* TODOs:
	Parameterized Constructor
	Getters/Setters
	Utility Functions
	...
	Add to main driver

*/

#pragma once
#include "Symbol.h"

class Node {
	Node();
	~Node();
	// TODO: Parameterized Constructor


	Symbol *m_symbol;
	Node* m_kids;
	int m_kidcnt;

	// TODO: Getters/Setters
	// int getSymbolType()


	// TODO: Utility Functions


};

// implementation
Node::Node() { }

Node::~Node() { }

// int Node::getSymbolType() { }