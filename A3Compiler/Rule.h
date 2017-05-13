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
	Ex = Opadd T Ex | eps
	E = T Ex//=======lre
	Tx = Opmul F Tx |eps
	T = F Tx //=======lre
	F = Fatom | Pexpr
	Pexpr = paren1 E paren2
	Fatom = id | int | float | string
	Opadd = plus | minus
	Opmul = aster | slash | caret
*/

#ifndef RULE_H
#define RULE_H
#include "Token.h"
#include "Symbol.h"
#include <iostream>

class Symbol;
class Rule {
public:
	void setRule(int arule_ID, int alhs_sym_ID, int arhs_count, int arhs_sym_ID[]);
	int get_rule_ID();
	int get_lhs_sym_ID();
	

	~Rule();
	Rule();
//private:
	int rule_ID;			//position of rule in the rule array
	int lhs_sym_ID;			//postiion of LHS in symbol array
	int rhs_sym_ID[10];		//position of RHS symbols in symbol array
	int rhs_count;			//how many symbols are on the RHS


	// TODO: Getters/Setters DONE


	// TODO: Utility Functions

};
#endif

// Implementation

Rule::Rule() { }
Rule::~Rule() { }

void Rule::setRule(int arule_ID, int alhs_sym_ID, int arhs_count, int arhs_sym_ID[])
{
	//i left the rhs_sym_array our for ... reasons
	rule_ID = arule_ID;
	lhs_sym_ID = alhs_sym_ID;
	rhs_count = arhs_count;
	for(int i = 0; i < 10; i++){
		rhs_sym_ID[i] = arhs_sym_ID[i];
	}
		

}
int Rule::get_rule_ID()
{
	return rule_ID;
}
int Rule::get_lhs_sym_ID()
{
	return lhs_sym_ID;
}