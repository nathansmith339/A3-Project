// Symbol stub
// Symbol is an object that holds a rule pointer and can determine type of TERM or NONT
/* TODOs:
	Parameterized constructor
	Getters/Setters
	Utility Functions
	...
	Add to main driver

*/
//==============================================
// defines
#pragma once
#define TERM 0	// terminal type
#define NONT 1	// non-terminal type
//==============================================
//include guard
#ifndef SYMBOL_H
#define SYMBOL_H

//==============================================
// forwards declared dependencies
class Token;

//==============================================
// include dependencies
#include "Rule.h"
#include "Token.h"
#include "Node.h"
#include <string>
#include <iostream>
using namespace std;




class Symbol {
public:
	void setSym(int asym_ID, string asym_name, int asym_kind, int amtx_ID); //Token *atoken_ID); //constructor function!
	int get_sym_kind();		//getter for sym_kind
	int get_mtx_Id();		//getter for mtx_ID
	Token *get_token_ID();	//getter for token_ID
	Symbol();
	~Symbol();
	// TODO: Parameterized constructor
//private:
	// Members
	int sym_ID;      //postion in symbol array
	string sym_name; //name for symbol
	int sym_kind;	 // TERM (0) or NONT (1)
	int mtx_ID;		 // non_termnial id in LL parser, only used by non terminals
	int token_ID;   //ternimal name, only used by terminals
	
	

	// TODO: Getters and Setters DONE

	// TODO: Utility Functions
	
};
#endif


// implementation
Symbol::Symbol() { }

Symbol::~Symbol() { }


void Symbol::setSym(int asym_ID, string asym_name, int asym_kind, int amtx_ID)
{
	sym_ID = asym_ID;      //postion in symbol array
	sym_name = asym_name ; //name for symbol
	sym_kind = asym_kind;	 // TERM (0) or NONT (1)
	mtx_ID = amtx_ID;		 // non_termnial id in LL parser, only used by non terminals
	//token_ID = atoken_ID;  //ternimal name, only used by terminals
}
int Symbol::get_sym_kind()
{
	// Terminal is 0, Non-Terminal is 1
	return sym_kind;
}
int Symbol::get_mtx_Id()
{
	// non_termnial id in LL parser, only used by non terminals
	return mtx_ID;
}
Token *Symbol::get_token_ID()
{
	//ternimal name, only used by terminals
	//return token_ID;
	return 0;
}
