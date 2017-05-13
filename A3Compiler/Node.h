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
#ifndef NODE_H
#define NODE_H
#include "Symbol.h"

class Symbol;
class Nodes {
public:
	Nodes(Nodes *a, int asym_id, int akid_count) // new shit
	{
		kid_count = akid_count;
		sym_ID = asym_id;
		curr = a;
	}
	void setNode(int asym_ID, int akid_count);
	int get_sym_ID();
	int get_kid_count();
	Nodes();
	~Nodes();
//private:
	int sym_ID;		//position of symbol in symbol array
	Nodes *kids[10];	//pointer array that points to children
	int kid_count;	//how many kids node has
	Nodes *curr;
};
#endif 
// implementation
Nodes::Nodes() { }

Nodes::~Nodes() { }

void Nodes::setNode(int aasym_ID, int akid_count)
{
	sym_ID = aasym_ID;		//position of symbol in symbol array
	kid_count = akid_count; //kid count
	//possibly set kidcount to zero?
}
int Nodes::get_sym_ID()
{
	//position of symbol in symbol array
	return sym_ID;
}
int Nodes::get_kid_count()
{
	//how many kids node has0
	return kid_count;
}
/**
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
**/