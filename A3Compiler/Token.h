// Token stub
/* TODOs:
	Utility Functions
	...

*/
#ifndef TOKEN_H
#define TOKEN_H
#pragma once
#include "Symbol.h"
#include <string>
using std::string;

#define comment 1
#define ident 2
#define t_int 3
#define t_float 4
#define t_string 5

// unpaired delimiters
#define comma 6
#define semi 7

// keywords
#define kwdprog 10
#define kwdmain 11
#define kwdfcn 12
#define kwdclass 13
#define kwdfloat 15
#define kwdint 16
#define kwdstring 17
#define kwdif 18
#define kwdelseif 19
#define kwdelse 20
#define kwdwhile 21
#define kwdinput 22
#define kwdprint 23
#define kwdnew 24
#define kwdreturn 25

// paired delimiters
#define angle1 31
#define angle2 32
#define brace1 33
#define brace2 34
#define bracket1 35
#define bracket2 36
#define parens1 37
#define parens2 38

// other punctuation
#define aster 41
#define caret 42
#define colon 43
#define dot 44
#define equal 45
#define minus 46
#define plus 47
#define slash 48

// multi-char oprerators
#define oparrow 51
#define opeq 52
#define opne 53
#define ople 54
#define opge 55
#define opshl 56
#define opshr 57


// other
#define error 98
#define t_eof 0
#define dollar 58
//#define kwdid 59
#define eps   50

class Token {
public:
	Token();
	~Token();

	string mTokName;		// print name of the token's type
	string mValue;			// token's value (if applicable)
	int mType;				// type of the token, types in define

	// TODO: Utility Functions


};
#endif
// implementation
Token::Token()
{
	mTokName = "error";
	mValue = "";
	mType = error;
}

Token::~Token() { }
