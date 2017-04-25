#pragma once
// Rule stub
// Rule is used to represent a rule with a LHS rule and RHS expansion
/* TODOs:
	Getters/Setters
	Utility Functions
	...
	Add to main driver
*/

/// A3 Grammar (Rules)
/*
	(perhaps have the rules be CAPS for non-terminal, and lower for terminal)?

	Pgm = kwdprog Block
	Block = brace1 Stmts brace2
	Stmts = Stmt semi Stmts | eps
	Stmt = Astmt | Ostmt | Wstmt | Fstmt
	AStmt = id equal Y
	Y = kwdinput
	Y = E
	Ostmt = kwdprint paren1 Elist paren2
	Wstmt = kwdwhile Pexpr Block
	Fstmt = kwdif Pexpr Block Else2
	Else2 = kwdelseif Pexpr Block Else2
	Else2 = kwdelse Block | eps
	Elist = E Elist2 | eps
	Elist2 = comma Elist
	E = E Opadd T | T
	T = T Opmul F | F
	F = Fatom | Pexpr
	Pexpr = paren1 E paren2
	Fatom = id | int | float | string
	Opadd = plus | minus
	Opmul = aster | slash | caret
*/


#include "Token.h"
#include <iostream>
class Rule {
public:
	Rule();
	~Rule();
	//Rule(Symbol *lhs, int index, int kidcnt, int toktype);
	
	Symbol *mLHS;		// The NONT LHS
	Symbol *mRHS[10];		// Expands into at most 10 rules
	int mIndex;			// index position of rule in Grammar (class)
	int mKids;			// number of kids for this rule (how big RHS is)
	int mTokType;		// Token type for easy access


	// TODO: Getters/Setters


	// TODO: Utility Functions

};


// Implementation
Rule::Rule() {}

Rule::~Rule() {}

//Rule::Rule(Symbol *lhs, int index, int kidcnt, int toktype)
//{
//	mLHS = lhs;
//	mIndex = index;
//	mKids = kidcnt;
//	mTokType = toktype;
//}