#pragma once
/* TODOs:
	Parameterized constructor
	Getters/Setters
	Utility Functions
	...
	Add to main driver

*/


class Rule {
	Rule();
	~Rule();
	// TODO: Parameterized Constructor
	
	char RHS;		// should always be one char
	char LHS[100];	// 100 is plenty of room
	// maybe add token type for easy access?


	// TODO: Getters/Setters


	// TODO: Utility Functions
};


// implementation
Rule::Rule() {}

Rule::~Rule() {}