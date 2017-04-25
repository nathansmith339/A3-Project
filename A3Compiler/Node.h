// Node stub
// Node is used to wrap symbols for use on the parsing stack

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

	Symbol *mSymbol;		// Symbol object pointer
	Node* mKids[10];		// Array of child nodes, 1-indexed (zero not used)
	int mKidCnt;			// Number of child nodes

	// TODO: Getters/Setters
	int getSymbolType();
	Node* getKid(int index);
	bool addKid(Node* kid)

	// TODO: Utility Functions


};

// implementation
Node::Node() { }

Node::~Node() { }

int Node::getSymbolType() 
{
	// Returns TERM (0) or NONT (1)
	return mSymbol->getType();
}

Node* Node::getKid(int index)
{
	// Returns kid at specified index
	return mKids[index];
}

bool Node::addKid(Node* kid) 
{
	// Adds kid to current node at next available spot, returns true if successful
	if (mKidCnt > 9)
	{
		// too many kids, can't do it
		return false;
	}
	else
	{
		// Increments mKidCnt and adds the requested kid to the array
		mKids[++mKidCnt] = kid;
		return true;
	}
}