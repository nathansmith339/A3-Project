// Symbol stub
// Symbol is an object that holds a rule pointer and can determine type of TERM or NONT
/* TODOs:
	Parameterized constructor
	Getters/Setters
	Utility Functions
	...
	Add to main driver

*/



#pragma once
#define TERM 0	// terminal type
#define NONT 1	// non-terminal type
#include "Rule.h"

class Symbol {
public:
	Symbol();
	~Symbol();
	// TODO: Parameterized constructor

	// Members
	int mType;		// TERM (0) or NONT (1)
	Rule *mRule;	// Specific rule
	// maybe have copy of RHS here?
	// char *mRHScpy[100];

	// TODO: Getters and Setters
	
	int getType();

	// TODO: Utility Functions
	
};


// implementation
Symbol::Symbol() { }

Symbol::~Symbol() { }


int Symbol::getType()
{
	// Terminal is 0, Non-Terminal is 1
	return mType;
}