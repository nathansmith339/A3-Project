// Symbol stub
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
	Symbol();
	~Symbol();
	// TODO: Parameterized constructor

	int m_type;	// filled w/ TERM or NONT
	Rule* m_rule;
	// maybe have copy of RHS here?
};


// implementation
Symbol::Symbol() { }

Symbol::~Symbol() { }