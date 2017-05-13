#pragma once
// Grammar class holds A3 grammar rules inside an array.
/* TODOs:
Parameterized constructor
Getters/Setters
Utility Functions
...
Add to main driver
*/

#ifndef GRAMMAR_H
#define GRAMMAR_H
#include "Rule.h"
class Grammar {
	Grammar();
	~Grammar();

	Rule mRules[100];
	
	// TODO: Parameterized Constructor

	// TODO: Getters/Setters
	
	Rule getRule(int index);		// returns the rule at specified index

	// TODO: Utility Functions

};
#endif 

Grammar::Grammar() {};

Grammar::~Grammar() {};

Rule Grammar::getRule(int index)
{
	// Params: index. Returns Rule object at specified index
	return mRules[index];
}