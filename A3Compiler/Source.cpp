// A3LexerMain.cpp
// Authors:					Nathan Smith and Jonathan Boe
// 
// Description:				A lexical analyzer that asks for a file name (no spaces) and performs a lexical analysis.
//								While the analysis is done, the results are output to the console in a parenthesized format.
//
//
//
// Developer Notes:
//		- When using printf, remember that %s looks for c-string type, std::string must be converted by use of (str).c_str() for printf to read it
//		- Token index on line is not implemented.
//		- Try using labels for debug statements, they'll be easier to track down
//


#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include "Token.h"
#include "tchar.h"
#include "Symbol.h"
#include "Node.h"
#include "Grammar.h"
#include "Rule.h"
using namespace std;


// FUNCTIONS
void getChar();			// gets next character in input,	!!dequeues the next char!!
void peekNext();		// gets next character in input		!!does not dequeue!!
void addChar();			// adds current char to a lexeme
int lookup(char ch);	// determines what a non-alphanumeric character is
int lex();				// contains the three main states of the program
bool isKWD();			// determines if an identifier is a keyword
bool isEnd(char ch);	// determines if char is designed to end token
void skipBlanks();		// skips whitespace

//new functions for ll parser 
void clear(int x[]);		//clears an array
void birther(stack<Nodes> & pstak, Nodes  x, Rule rule);
void LLparser(stack<int> & int_stack, stack<Nodes> & node_stack);
void PST(Nodes a);


// GLOBALS
const int LEXEME_SIZE = 100;	// determines size of lexeme
int lineCount;					// count of number of lines in source file
char lexeme[LEXEME_SIZE];		// lexeme length is 99 max
char lexLength;					// temp. holds the length of a particular lexeme
char peekedChar;				// holds a peeked character
int peekClass;					// keeps track of peekedChar's type
char nextChar;					// holds the character in question
int charClass;					// keeps track of nextChar's "type"
Token *CurrentToken;			// object that holds the current token being used
Token *TokenList[1000];			// array of tokens, 1000 is more than plenty
int tokenCount = 0;				// count of tokens for use in TokenList

Nodes * root;					//root of tree
Nodes * parentptr;				//points to current parent
Nodes * childptr;				//points to leftmost child
Nodes * holder;

ifstream input_file;
char fileName[100] = "source.txt";
char errorString[100];





/// Used for charClass and peekClass
#define LETTER 0
#define DIGIT 1
#define FLTPT 3
#define SPC 4
#define UNKNOWN 99

//symbols
Symbol list[100];

Symbol PGM, Block, Stmts, Stmt, Astmt, Y, Ostmt, Wstmt, Fstmt,
	Else2, Elist, Elist2, Ex, Tx, Pexpr, Fatom, Opadd, Opmul, F, T, E, dollars;
Symbol kwdprogs, brace1s, brace2s, semis, epss, ids, equals, kwdinputs, kwdprints,
	parens1s, parens2s, kwdwhiles, kwdifs, kwdelseifs, kwdelses, commas, ints,
	floats, strings, pluss, minuss, asters, slashs, carets; 

//Nodes
Nodes nodel[100];

Nodes PGMn, Blockn, Stmtsn, Stmtn, Astmtn, Yn, Ostmtn, Wstmtn, Fstmtn,
	Else2n, Elistn, Elist2n, Exn, Txn, Pexprn, Fatomn, Opaddn, Opmuln, Fn, Tn, En, dollarn;
Nodes kwdprogn, brace1n, brace2n, semin, epsn, idn, equaln, kwdinputn, kwdprintn,
	parens1n, parens2n, kwdwhilen, kwdifn, kwdelseifn, kwdelsen, comman, intn,
	floatn, stringn, plusn, minusn, astern, slashn, caretn;

//Rules
Rule Rules[100];
Rule PGMr, Blockr, Stmtsr, Stmtr, Astmtr, Yr, Ostmtr, Wstmtr, Fstmtr,
	Else2r, Elistr, Exr, Txr, Pexprr, Fatomr, Opaddr, Opmulr, Fr, Tr, Er, dollarr;
Rule kwdprogr, brace1r, brace2r, semir, epsr, idr, equalr, kwdinputr, kwdprintr,
	parens1r, parens2r, kwdwhiler, kwdifr, kwdelseifr, kwdelser, commar, intr,
	floatr, stringr, plusr, minusr, asterr, slashr, caretrc, Stmtsr1, Stmtr1, Stmtr2, Stmtr3, Yr1
	,Else2r1, Else2r2, Elistr1, Elist2r, Fr1,Fatomr1,Fatomr2,Tx1,Ex1r, Opmulr2,Opmulr1, Oppaddr1, Fatomr3;

//stacks
stack<Nodes> node_stack;
stack<int> int_stack;
stack<int> int_stack_temp;
//parser array
int ll_parser[100][100];










int main()
{
	int_stack.push(dollar);

	
	//===================================================================================
	// LETS MAKE A BUNC OF SYMBOLS WHOOO

	//PGM SYMBOL[0] ll (0,1)
	PGM.setSym(0, "PGM", 1, 1);
	list[0] = PGM;

	//kwdprog SYMBOL[1]

	kwdprogs.setSym(1, "kwdprog", 0, -1);
	list[1] = kwdprogs;
	
	//Block SYMBOL[2] ll (0,2)
	//Symbol Block;
	Block.setSym(2, "Block", 1, 2);
	list[2] = Block;
	
	//brace1 SYMBOL[3]

	///===============these need to be attacked to their tokens
	
	brace1s.setSym(3, "brace1", 0, -1);
	list[3] = brace1s;
	
	//Stmts SYMBOL[4] ll(0,3)
	Stmts.setSym(4, "Stmts", 1, 3);
	list[4] = Stmts;
	
	//brace2 SYMBOL[5]
	brace2s.setSym(5, "brace2", 0, -1);
	list[5] = brace2s;
	
	//semi SYMBOL[6]
	semis.setSym(6, "semi", 0, -1);
	list[6] = semis;
	
	//stmt SYMBOL[7] ll(0,21)
	Stmt.setSym(7, "stms", 1, 21);
	list[7] = Stmt;
	
	//Astmt SYMBOL[8] ll(0,4)
	Astmt.setSym(8, "Astmt", 1, 4);
	list[8] = Astmt;
	
	//Ostmt SYMBOL[9] ll(0,6)
	Ostmt.setSym(9, "Ostmt", 1, 6);
	list[9] = Ostmt;
	
	//Wstmt SYMBOl[10] ll(0,7)
	Wstmt.setSym(10, "Wstmt", 1, 7);
	list[10] = Wstmt;
	
	//Fstmt SYMBOL[11] ll(0,8)
	Fstmt.setSym(11, "Fstmt", 1, 8);
	list[11] = Fstmt;

	//id SYMBOL[13]
	ids.setSym(13, "id", 0, -1);
	list[13] = ids;
	
	//equal SYMBOL[14]
	equals.setSym(14, "equal", 0, -1);
	list[14] = equals;
	
	// Y SYMBOL[15] ll(0,5)
	Y.setSym(15, "Y", 1, 5);
	list[15] = Y;
	
	//kwdinput SYMBOL[16]
	kwdinputs.setSym(16, "kwdinput", 0, -1);
	list[16] = kwdinputs;
	
	//E SYMBOL[17] ll(0,20)
	E.setSym(17, "E", 1, 20);
	list[17] = E;
	
	//kwdprint SYMBOL[18] 
	kwdprints.setSym(18, "kwdprint", 0, -1);
	list[18] = kwdprints;
	
	//Elist SYMBOL[19] ll(0,10)
	Elist.setSym(19, "Elist", 1, 10);
	list[19] = Elist;
	
	//kwdwhile SYMBOL[20] ll(0,13)
	kwdwhiles.setSym(20, "kwdwhile", 0, -1);
	list[20] = kwdwhiles;
	
	//Pexpr SYMBOL[21] ll(0,14)
	Pexpr.setSym(21, "Pexpr", 1, 14);
	list[21] = Pexpr;
	
	//kwdif SYMBOL[22] 
	kwdifs.setSym(22, "kwdif", 0, -1);
	list[22] = kwdifs;
	
	//Else2 SYMBOl[23] ll(0,9)
	Else2.setSym(23, "Else2", 1, 9);
	list[23] = Else2;
	
	//kwdelseif SYMBOL[24]
	kwdelseifs.setSym(24, "kwdelseif", 0, -1);
	list[24] = kwdelseifs;
	
	//Elist2 SYMBOL[25] ll(0,11);
	Elist2.setSym(25, "Elist2", 1, 11);
	list[25] = Elist2;
	
	//kwdelse SYMBOL[26] 
	kwdelses.setSym(26, "kwdelse", 0, -1);
	list[26] = kwdelses;
	
	//comma SYMBOL[27]
	commas.setSym(27, "comma", 0, -1);
	list[27] = commas;
	
	//Opadd SYMBOL[28] ll(0,16)
	Opadd.setSym(28, "Opadd", 1, 16);
	list[28] = Opadd;
	
	//T Symbol[29] ll(0,19)
	T.setSym(29, "T", 1, 19);
	list[29] = T;
	
	//Opmul SYMBOL[30] ll(0,17)
	Opmul.setSym(30, "Opmul", 1, 17);
	list[30] = Opmul;
	
	//F SYMBOL[31] ll(0,18)
	F.setSym(31, "F", 1, 18);
	list[31] = F;
	
	//Fatom SYMBOL[32] ll(0,15)
	Fatom.setSym(32, "Fatom", 1, 15);
	
	//int SYMBOL[33]
	ints.setSym(33, "int", 0, -1);
	list[31] = F;
	
	//float SYMBOL[34]
	floats.setSym(34, "float", 0, -1);
	list[31] = floats;
	
	//string SYMBOL[35]
	strings.setSym(35, "string", 0, -1);
	list[35] = strings;
	
	//plus SYMBOL[36]
	pluss.setSym(36, "plus", 0, -1);
	list[36] = pluss;
	
	//minus SYMBOL[37]
	minuss.setSym(37, "minus", 0, -1);
	list[37] = minuss;
	
	//aster SYMBOL[38]
	asters.setSym(38, "aster", 0, -1);
	list[38] = asters;
	
	//slash SYMBOL[39]
	slashs.setSym(39, "slash", 0, -1);
	list[39] = slashs;
	
	//caret SYMBOL[40]
	carets.setSym(40, "caret", 0, -1);
	list[40] = carets;
	
	//parens1 SYMBOL[41]
	parens1s.setSym(41, "parens1", 0, -1);
	list[41] = parens1s;
	
	//parens2 SYMBOL[42]
	parens2s.setSym(42, "parens2", 0, -1);
	list[42] = parens2s;
	
	//dollars SYMBOL[43]
	dollars.setSym(43, "dollar", 0, -1);
	list[43] = dollars;

	//Ex SYMBOL[44]
	Ex.setSym(44, "Ex", 1, 12);
	list[44] = Ex;

	//Tx SYMBOL[45]
	Tx.setSym(45, "Tx", 1, 13);
	list[45] = Tx;

	//eps SYMBOL[46]
	epss.setSym(46, "eps", 0, -1);
	list[46] = epss;

	//=================================================================================

	//=================================================================================
	//lets make the rules array now whooooo
	
	int tempr[10];
	
	clear(tempr);
	//PGM RULES[1] kwdprog block
	tempr[0] = 1;
	tempr[1] = 2;

	PGMr.setRule(1, 0, 2, tempr); //1,2
	Rules[1] = PGMr;
	clear(tempr);
	ll_parser[1][kwdprog] = 1;

	//Block RUlES[2] brace1 stmts brace2
	
	tempr[0] = 3;
	tempr[1] = 4;
	tempr[2] = 5;
	
	Blockr.setRule(2, 2, 3, tempr); //3,4,5
	Rules[2] = Blockr;
	clear(tempr);
	ll_parser[2][brace1] = 2;

	//Stmts RULES[3] stmt semi stmts 
	
	tempr[0] = 7;
	tempr[1] = 6;
	tempr[2] = 4;

	Stmtsr.setRule(3, 4, 3, tempr); //7,6,4
	Rules[3] = Stmtsr;
	clear(tempr);
	ll_parser[3][ident] = 3;
	ll_parser[3][kwdwhile] = 3;
	ll_parser[3][kwdprint] = 3;
	ll_parser[3][kwdif] = 3;


	//Stmts RULES[4] EPS ***************************
	//Rule Stmtsr1;
	tempr[0] = 43;

	Stmtsr1.setRule(4, 4, 1, tempr); //EPS
	Rules[4] = Stmtsr1;
	clear(tempr);
	

	//Stmt RUles[5] ASTMT
	//Rule Stmtr;
	tempr[0] = 8;

	Stmtr.setRule(5, 7, 1, tempr); //8
	Rules[5] = Stmtr;
	clear(tempr);
	ll_parser[21][ident] = 5;

	//Stmt RUles[6] OSTMT
	//Rule Stmtr1;
	tempr[0] = 9;
	
	Stmtr1.setRule(6, 7, 1, tempr); //9
	Rules[6] = Stmtr1;
	clear(tempr);
	ll_parser[21][kwdprint] = 6;

	//Stmt RUles[7] WSTMT
	//Rule Stmtr2;
	tempr[0] = 10;
	
	Stmtr2.setRule(7, 7, 1, tempr); //10
	Rules[7] = Stmtr2;
	clear(tempr);
	ll_parser[21][kwdwhile] = 7;

	//Stmt RUles[8] FSTMT
	//Rule Stmtr3;
	tempr[0] = 11;
	

	Stmtr3.setRule(8, 7, 1, tempr); //11
	Rules[8] = Stmtr3;
	clear(tempr);
	ll_parser[21][kwdif] = 8;

	//Astmt RULES[9] id equal Y **********id*************
	//Rule Astmtr;
	tempr[0] = 13;
	tempr[1] = 14;
	tempr[2] = 15;
	

	Astmtr.setRule(9, 8, 3, tempr); //13,14,15
	Rules[9] = Astmtr;
	clear(tempr);
	ll_parser[4][ident] = 9;

	//Y RULES[10] kwdinput
	//Rule Yr;
	tempr[0] = 16;

	Yr.setRule(10, 15, 1, tempr); //16
	Rules[10] = Yr;
	clear(tempr);
	ll_parser[5][kwdinput] = 10;

	//Y RULES[11] E
	//Rule Yr1;
	tempr[0] = 17;
	
	Yr1.setRule(11, 15, 1, tempr); //17
	Rules[11] = Yr1;
	clear(tempr);
	ll_parser[5][ident] = 11;
	ll_parser[5][t_int] = 11;
	ll_parser[5][t_float] = 11;
	ll_parser[5][t_string] = 11;
	ll_parser[5][parens1] = 11;
	//Ostmt RULES[12] kwdprint Elist paren2
	//Rule Ostmtr;
	tempr[0] = 18;
	tempr[1] = 41;
	tempr[2] = 19;
	tempr[3] = 42;

	Ostmtr.setRule(12, 9, 4, tempr); //18,41,19,42
	Rules[12] = Ostmtr;
	clear(tempr);
	ll_parser[6][kwdprint] = 12;

	//Wstmt RULES[13] kwdwhile Pexpr Block
	//Rule Wstmtr;
	tempr[0] = 20;
	tempr[1] = 21;
	tempr[2] = 2;
	
	Wstmtr.setRule(13, 10, 3, tempr); //20,21,2
	Rules[13] = Wstmtr;
	clear(tempr);
	ll_parser[7][kwdwhile] = 13;

	//Fstmt RULES[14] kwdif Pexpr Block Else2
	//Rule Fstmtr;
	tempr[0] = 22;
	tempr[1] = 21;
	tempr[2] = 2;
	tempr[3] = 23;

	Fstmtr.setRule(14, 11, 4, tempr); //22,21,2,23
	Rules[14] = Fstmtr;
	clear(tempr);
	ll_parser[8][kwdif] = 14;

	//Else2 RULES[15] kwdelseif Pexpr Block Else2
	//Rule Else2r;
	tempr[0] = 24;
	tempr[1] = 21;
	tempr[2] = 2;
	tempr[3] = 23;

	Else2r.setRule(15, 23, 4, tempr); //24,21,2,23
	Rules[15] = Else2r;
	clear(tempr);
	ll_parser[9][kwdelseif] = 15;

	//Else2 RULES[16] kwdelse Block
	//Rule Else2r1;
	tempr[0] = 26;
	tempr[1] = 2;
	
	Else2r1.setRule(16, 23, 2, tempr); //26,2
	Rules[16] = Else2r1;
	clear(tempr);
	ll_parser[9][kwdelse] = 16;

	//Else2 Rules[17] EPS ******************************
	//Rule Else2r2;
	tempr[0] = 43;
	
	Else2r2.setRule(17, 23, 1, tempr); //EPS
	Rules[17] = Else2r2;
	clear(tempr);
	ll_parser[9][eps] = 17;

	//Elist RULES[18] E Elist2
	//Rule Elistr;
	tempr[0] = 17;
	tempr[1] = 25;
	
	Elistr.setRule(18, 19, 2, tempr); //17,25
	Rules[18] = Elistr;
	clear(tempr);
	ll_parser[10][ident] = 18;
	ll_parser[10][t_int] = 18;
	ll_parser[10][t_float] = 18;
	ll_parser[10][t_string] = 18;
	ll_parser[10][parens1] = 18;

	//Elist Rules[19] EPS *********************
	//Rule Elistr1;
	tempr[0] = 43;
	
	Elistr1.setRule(19, 19, 1, tempr); //EPS 43
	Rules[19] = Elistr1;
	clear(tempr);
	ll_parser[10][eps] = 19;

	//Elist2 RULES[20] comma Elist
	//Rule Elist2r;
	tempr[0] = 27;
	tempr[1] = 19;
	
	Elist2r.setRule(20, 25, 2, tempr); //27,19
	Rules[20] = Elist2r;
	clear(tempr);
	ll_parser[11][comma] = 20;

	//F RULES[25] Fatom
	//Rule Fr;
	tempr[0] = 32;
	
	Fr.setRule(25, 31, 1, tempr); //32
	Rules[25] = Fr;
	clear(tempr);
	ll_parser[18][ident] = 25;
	ll_parser[18][t_int] = 25;
	ll_parser[18][t_float] = 25;
	ll_parser[18][t_string] = 25;

	//F RULES[26] Pexpr
	//Rule Fr1;
	tempr[0] = 21;
	
	Fr1.setRule(25, 31, 1, tempr); //21
	Rules[26] = Fr1;
	clear(tempr);
	ll_parser[18][parens1] = 26;

	//Pexpr RULES[27] paren1 E paren2
	//Rule Pexprr;
	tempr[0] = 41;
	tempr[1] = 17;
	tempr[2] = 42;
	
	Pexprr.setRule(27, 21, 3, tempr); //41,17,42
	Rules[27] = Pexprr;
	clear(tempr);
	ll_parser[14][parens1] = 27;

	//Fatom RULES[28] id
	//Rule Fatomr;
	tempr[0] = 13;
	
	Fatomr.setRule(28, 32, 1, tempr); //13
	Rules[28] = Fatomr;
	clear(tempr);
	ll_parser[15][ident] = 28;

	//Fatom RULES[29] int
	//Rule Fatomr1;
	tempr[0] = 33;
	
	Fatomr1.setRule(29, 32, 1, tempr); //33
	Rules[29] = Fatomr1;
	clear(tempr);
	ll_parser[15][t_int] = 29;

	//Fatom RULES[30] float
	//Rule Fatomr2;
	tempr[0] = 34;
	
	Fatomr2.setRule(30, 32, 1, tempr); //34
	Rules[30] = Fatomr2;
	clear(tempr);
	ll_parser[15][t_float] = 30;

	//Fatom RULES[31] string
	//Rule Fatomr3;
	tempr[0] = 35;
	
	Fatomr3.setRule(31, 32, 1, tempr); //35
	Rules[31] = Fatomr3;
	clear(tempr);
	ll_parser[15][t_string] = 31;

	//Opadd RULES[32] plus
	//Rule Opaddr;
	tempr[0] = 36;
	
	Opaddr.setRule(32, 28 , 1, tempr); //36
	Rules[32] = Opaddr;
	clear(tempr);
	ll_parser[16][plus] = 32;

	//Opadd RULES[33] minus
	//Rule Oppaddr1;
	tempr[0] = 37;
	
	Oppaddr1.setRule(33, 28, 1, tempr); //37
	Rules[33] = Oppaddr1;
	clear(tempr);
	ll_parser[16][minus] = 33;

	//Opmul RULES[34] aster
	//Rule Opmulr;
	tempr[0] = 38;
	
	Opmulr.setRule(34, 30, 1, tempr); //38
	Rules[34] = Opmulr;
	clear(tempr);
	ll_parser[17][aster] = 34;

	//Opmul RULES[35] slash
	//Rule Opmulr1;
	tempr[0] = 39;

	Opmulr1.setRule(35, 30 , 1, tempr); //39
	Rules[35] = Opmulr1;
	clear(tempr);
	ll_parser[17][slash] = 35;

	//Opmul RULES[36] caret
	//Rule Opmulr2;
	tempr[0] = 40;
	
	Opmulr2.setRule(36, 30, 1, tempr); //40
	Rules[36] = Opmulr2;
	clear(tempr);
	ll_parser[17][caret] = 36;

	//Ex RULES[37] Opadd T Ex
	//Rule Exr;
	tempr[0] = 28;
	tempr[1] = 29;
	tempr[2] = 44;

	Exr.setRule(37, 44, 3, tempr);
	Rules[37] = Exr;
	clear(tempr);
	ll_parser[12][plus] = 37;
	ll_parser[12][minus] = 37;

	//Ex1 RULES[38] eps
	//Rule Ex1r;
	tempr[0] = 46;

	Ex1r.setRule(38, 44, 1, tempr);
	Rules[38] = Ex1r;
	clear(tempr);
	ll_parser[12][eps] = 38;
	
	//Tx RUlES[39] Opmul F Tx
	//Rule Txr;
	tempr[0] = 30;
	tempr[1] = 31;
	tempr[2] = 45;

	Txr.setRule(39, 45, 3, tempr);
	Rules[39] = Txr;
	clear(tempr);
	ll_parser[13][aster] = 39;
	ll_parser[13][slash] = 39;
	ll_parser[13][caret] = 39;

	//Tx1 RUlES[40] eps
	//Rule Tx1;
	tempr[0] = 46;

	Tx1.setRule(40, 45, 1, tempr);
	Rules[40] = Tx1;
	clear(tempr);
	ll_parser[13][eps] = 40;

	//E RULES[21] T Ex ******
	//Rule Er;
	tempr[0] = 29;
	tempr[1] = 44;

	
	Er.setRule(21, 17, 2, tempr); //17,28,29
	Rules[21] = Er;
	clear(tempr);
	ll_parser[20][ident] = 21;
	ll_parser[20][t_int] = 21;
	ll_parser[20][t_float] = 21;
	ll_parser[20][t_string] = 21;
	ll_parser[20][parens1] = 21;

	//T RULES[23] F Tx*******
	//Rule Tr;
	tempr[0] = 31;
	tempr[1] = 45;
	
	Tr.setRule(23, 29, 2, tempr); //29,30,31
	Rules[23] = Tr;
	clear(tempr);
	ll_parser[19][ident] = 23;
	ll_parser[19][t_int] = 23;
	ll_parser[19][t_float] = 23;
	ll_parser[19][t_string] = 23;
	ll_parser[19][parens1] = 23;
	

	//=========================================================================
	//====================LETS MAKE THE ll PARSER==============================
	//========================FIRST SETS=======================================

	ll_parser[1][0] = 1001;	//PMG
	PGM.token_ID = 1001;
	ll_parser[2][0] = 1002; //Block
	Block.token_ID = 1002;
	ll_parser[3][0] = 1003;	//Stmts
	Stmts.token_ID = 1003;
	ll_parser[4][0] = 1004; //Astmt
	Astmt.token_ID = 1004;
	ll_parser[5][0] = 1005; //Y
	Y.token_ID = 1005;
	ll_parser[6][0] = 1006; //Ostmt
	Ostmt.token_ID = 1006;
	ll_parser[7][0] = 1007; //Wstmt
	Wstmt.token_ID = 1007;
	ll_parser[8][0] = 1008; //Fstmt
	Fstmt.token_ID = 1008;
	ll_parser[9][0] = 1009; //Else2
	Else2.token_ID = 1009;
	ll_parser[10][0] = 1010; //Elist
	Elist.token_ID = 1010;
	ll_parser[11][0] = 1011; //Elist2
	Elist2.token_ID = 1011;
	ll_parser[12][0] = 1012; //Ex
	Ex.token_ID = 1012;
	ll_parser[13][0] = 1013; //Tx
	Tx.token_ID = 1013;
	ll_parser[14][0] = 1014; //Pexpr
	Pexpr.token_ID = 1014;
	ll_parser[15][0] = 1015; //Fatom
	Fatom.token_ID = 1015;
	ll_parser[16][0] = 1016; //Opadd
	Opadd.token_ID = 1016;
	ll_parser[17][0] = 1017; //Opmul
	Opmul.token_ID = 1017;
	ll_parser[18][0] = 1018; //F
	F.token_ID = 1018;
	ll_parser[19][0] = 1019; //T
	T.token_ID = 1019;
	ll_parser[20][0] = 1020; //E
	E.token_ID = 1020;
	ll_parser[21][0] = 1021; //Stmt
	Stmt.token_ID = 1021;

	//===============Time for Terminal ll parser postions==================
	ll_parser[0][1] = kwdprog;
	kwdprogs.token_ID = 10;
	ll_parser[0][2] = brace1;
	brace1s.token_ID = 33;
	ll_parser[0][3] = brace2;
	brace2s.token_ID = 34;
	ll_parser[0][4] = semi;
	semis.token_ID = 7;
	ll_parser[0][5] = eps; //eps
	epss.token_ID = 50;
	ll_parser[0][6] = ident; //id
	ids.token_ID = 2;
	ll_parser[0][7] = equal;
	equals.token_ID = 45;
	ll_parser[0][8] = kwdinput;
	kwdinputs.token_ID = 22;
	ll_parser[0][9] = kwdprint;
	kwdprints.token_ID = 23;
	ll_parser[0][10] = parens1;
	parens1s.token_ID = 37;
	ll_parser[0][11] = parens2;
	parens2s.token_ID = 38;
	ll_parser[0][12] = kwdwhile;
	kwdwhiles.token_ID = 21;
	ll_parser[0][13] = kwdif;
	kwdifs.token_ID = 18;
	ll_parser[0][14] = kwdelseif;
	kwdelseifs.token_ID = 19;
	ll_parser[0][15] = kwdelse;
	kwdelses.token_ID = 20;
	ll_parser[0][16] = comma;
	commas.token_ID = 6;
	ll_parser[0][17] = t_int; 
	ints.token_ID = 3;
	ll_parser[0][18] = t_float;
	floats.token_ID = 4;
	ll_parser[0][19] = t_string;
	strings.token_ID = 5;
	ll_parser[0][20] = plus;
	pluss.token_ID = 47;
	ll_parser[0][21] = minus;
	minuss.token_ID = 46;
	ll_parser[0][22] = aster;
	asters.token_ID = 41;
	ll_parser[0][23] = slash;
	slashs.token_ID = 48;
	ll_parser[0][24] = caret;
	carets.token_ID = 42;
	ll_parser[0][25] = dollar;
	dollars.token_ID = 58;

	//================FOLLOW SETS=========================================
	clear(tempr);
	epsr.setRule(50, 100000,10000,tempr);
	Rules[50] = epsr;
	
	ll_parser[1][dollar] = 50;	//PMG

	ll_parser[2][kwdelseif] = 50; //Block
	ll_parser[2][kwdelse] = 50; //Block
	ll_parser[2][dollar] = 50; //Block
	ll_parser[2][semi] = 50; //Block

	ll_parser[3][brace2] = 50;	//Stmts

	//ll_parser[4][0] = 50; //Astmt

	//ll_parser[5][0] = 50; //Y

	ll_parser[6][semi] = 50; //Ostmt

	ll_parser[7][semi] = 50; //Wstmt

	ll_parser[8][semi] = 50; //Fstmt

	ll_parser[9][semi] = 50; //Else2

	ll_parser[10][parens2] = 50; //Elist
	ll_parser[11][parens2] = 50; //Elist2

	ll_parser[12][parens2] = 50; //Ex
	ll_parser[12][comma] = 50; //Ex
	ll_parser[12][semi] = 50;

	ll_parser[13][plus] = 50; //Tx
	ll_parser[13][minus] = 50; //Tx
	ll_parser[13][parens2] = 50; //Tx
	ll_parser[13][comma] = 50; //Tx
	ll_parser[13][semi] = 50;

	ll_parser[14][aster] = 50; //Pexpr
	ll_parser[14][slash] = 50; //Pexpr
	ll_parser[14][caret] = 50; //Pexpr
	ll_parser[14][plus] = 50; //Pexpr
	ll_parser[14][minus] = 50; //Pexpr
	ll_parser[14][parens2] = 50; //Pexpr
	ll_parser[14][comma] = 50; //Pexpr

	ll_parser[15][aster] = 50; //Fatom
	ll_parser[15][slash] = 50; //Fatom
	ll_parser[15][caret] = 50; //Fatom
	ll_parser[15][plus] = 50; //Fatom
	ll_parser[15][minus] = 50; //Fatom
	ll_parser[15][parens2] = 50; //Fatom
	ll_parser[15][comma] = 50; //Fatom

	ll_parser[16][ident] = 50; //Opadd
	ll_parser[16][t_int] = 50; //Opadd
	ll_parser[16][t_float] = 50; //Opadd
	ll_parser[16][t_string] = 50; //Opadd
	ll_parser[16][parens1] = 50; //Opadd

	ll_parser[17][ident] = 50; //Opmul
	ll_parser[17][t_int] = 50; //Opmul
	ll_parser[17][t_float] = 50; //Opmul
	ll_parser[17][t_string] = 50; //Opmul
	ll_parser[17][parens1] = 50; //Opmul


	ll_parser[18][aster] = 50; //F
	ll_parser[18][slash] = 50; //F
	ll_parser[18][caret] = 50; //F
	ll_parser[18][plus] = 50; //F
	ll_parser[18][minus] = 50; //F
	ll_parser[18][parens2] = 50; //F

	ll_parser[19][plus] = 50; //T
	ll_parser[19][minus] = 50; //T
	ll_parser[19][parens2] = 50; //T
	ll_parser[19][comma] = 50; //T

	ll_parser[20][parens2] = 50; //E
	ll_parser[20][comma] = 50; //E

	ll_parser[21][semi] = 50; //Stmt
	

	//==============================lets make a bunch of nodes==================================================
	//Nodes PGMn;
	PGMn.setNode(0,0);
	list[0] = PGM;
	nodel[0] = PGMn;
	
	//Nodes kwdprogn;
	kwdprogn.setNode(1,0);
	list[1] = kwdprogs;//symbol now can acces the node 
	nodel[1]=kwdprogn;

	//Nodes Blockn;
	Blockn.setNode(2,0);
	list[2] = Block;
	nodel[2]= Blockn;

	//Nodes brace1n
	brace1n.setNode(3,0);
	list[3] = brace1s;
	nodel[3]= brace1n;

	//Nodes Stmtsn
	Stmtsn.setNode(4,0);
	list[4] = Stmts;
	nodel[4]= Stmtsn;

	//Nodes brace2n
	brace2n.setNode(5,0);
	list[5] = brace2s;
	nodel[5]= brace2n;

	//Nodes semin
	semin.setNode(6,0);
	list[6] = semis;
	nodel[6]= semin;

	//Nodes Stmtn
	Stmtn.setNode(7,0);
	list[7] = Stmt;
	nodel[7]= Stmtn;

	//Nodes Astmtn
	Astmtn.setNode(8,0);
	list[8] = Astmt;
	nodel[8]= Astmtn;

	//Nodes Ostmtn
	Ostmtn.setNode(9,0);
	list[9] = Ostmt;
	nodel[9]= Ostmtn;

	//Nodes Wstmtn
	Wstmtn.setNode(10,0);
	list[10] = Wstmt;
	nodel[10]= Wstmtn;

	//Nodes Fstmtn
	Fstmtn.setNode(11,0);
	list[11] = Fstmt;
	nodel[11]= Fstmtn;

	//Nodes idn
	idn.setNode(13,0);
	list[13] = ids; 
	nodel[13]= idn;

	//Nodes equaln
	equaln.setNode(14,0);
	list[14] = equals;
	nodel[14] = equaln;

	//Nodes Yn
	Yn.setNode(15,0);
	list[15] = Y;
	nodel[15]= Yn;

	//Nodes input
	kwdinputn.setNode(16,0);
	list[16] = kwdinputs;
	nodel[16]= kwdinputn;

	//Nodes En
	En.setNode(17,0);
	list[17] = E;
	nodel[17]= En;

	//Nodes kwdprintn
	kwdprintn.setNode(18,0);
	list[18] = kwdprints;
	nodel[18]= kwdprintn;

	//Nodes Elist
	Elistn.setNode(19,0);
	list[19] = Elist;
	nodel[19]= Elistn;

	//Nodes kwdwhilen
	kwdwhilen.setNode(20,0);
	list[20] = kwdwhiles;
	nodel[20]= kwdwhilen;

	//Nodes Pexprn
	Pexprn.setNode(21,0);
	list[21] = Pexpr;
	nodel[21]= Pexprn;

	//Nodes kwdifn
	kwdifn.setNode(22,0);
	list[22] = kwdifs;
	nodel[22]= kwdifn;

	//Nodes Else2n
	Else2n.setNode(23,0);
	list[23] = Else2;
	nodel[23]= Else2n;

	//Nodes kwdelseifn
	kwdelseifn.setNode(24,0);
	list[24] = kwdelseifs;
	nodel[24]= kwdelseifn;

	//Nodes Elist2n
	Elist2n.setNode(25,0);
	list[25] = Elist2;
	nodel[25]= Elist2n;

	//Nodes kwdelsen
	kwdelsen.setNode(26,0);
	list[26] = kwdelses;
	nodel[26]= kwdelsen;

	//Nodes comman
	comman.setNode(27,0);
	list[27] = commas;
	nodel[27]= comman;

	//Nodes Opaddn
	Opaddn.setNode(28,0);
	list[28] = Opadd;
	nodel[28]= Opaddn;

	//Nodes Tn
	Tn.setNode(29,0);
	list[29] = T;
	nodel[29]= Tn;

	//Nodes Opmuln
	Opmuln.setNode(30,0);
	list[30] = Opmul;
	nodel[30]= Opmuln;

	//Nodes Fn
	Fn.setNode(31,0);
	list[31] = F;
	nodel[31]= Fn;

	//Nodes Fatomn
	Fatomn.setNode(32,0);
	list[32] = Fatom;
	nodel[32]= Fatomn;

	//Nodes intn
	intn.setNode(33,0);
	list[33] = ints;
	nodel[33]= intn;

	//Nodes floatn
	floatn.setNode(34,0);
	list[34] = floats;
	nodel[34]= floatn;

	//Nodes stringn
	stringn.setNode(35,0);
	list[35] = strings;
	nodel[35]= stringn;
	 
	//Nodes plusn
	plusn.setNode(36,0);
	list[36] = pluss;
	nodel[36]= plusn;

	//Nodes minusn
	minusn.setNode(37,0);
	list[37] = minuss;
	nodel[37]= minusn;

	//Nodes astern
	astern.setNode(38,0);
	list[38] = asters;
	nodel[38]= astern;

	//Nodes slashn
	slashn.setNode(39,0);
	list[39] = slashs;
	nodel[39]= slashn;

	//Nodes caretn
	caretn.setNode(40,0);
	list[40] = carets;
	nodel[40]= caretn;

	//Nodes parens1n
	parens1n.setNode(41,0);
	list[41] = parens1s;
	nodel[41]= parens1n;

	//Nodes parens1n
	parens2n.setNode(42,0);
	list[42] = parens2s;
	nodel[42]= parens2n;

	//Nodes dollarn
	dollarn.setNode(43,0);
	list[43] = dollars;
	nodel[43] = dollarn;

	//Nodes Exn
	Exn.setNode(44,0);
	list[44] = Ex; 
	nodel[44]= Exn;

	//Nodes Txn
	Txn.setNode(45,0);
	list[45] = Tx;
	nodel[45]= Txn;

	//Nodes epsn
	epsn.setNode(46,0);
	list[46] = epss;
	nodel[46]= epsn;

	//====================================================================



	//=================================================================================
	
	lineCount = 1;
	int lex_ret; // returned value from lexer, success or error

	
	printf("Attempting to open default file: %s...\t", fileName);
	
	// open file
	input_file.open(fileName);
	
	// failure to open default file
	if (input_file.fail())
	{
		printf("Error opening file. Filename of '%s' doesn't exist. Try entering filename.\n", fileName);
		
		// ask for file
		cout << "Enter input filename (no spaces)\n>";
		cin.getline(fileName, 100);
		
		// try again
		input_file.open(fileName);
		
		// okay, this isn't working
		if (input_file.fail())
		{
			printf("Error opening file. Filename of '%s' doesn't exist.\nExiting.\n", fileName);
			printf("\nPress any key to exit.\n");
			cin.ignore();
			return 0;
		}
	}


	printf("Successfully opened file!\n");
	printf("(:lang A3\n");
	getChar(); // grab first char to start off
	do
	{
		// processing
			
		CurrentToken = new Token();
			
		lex_ret = lex();
		if (lex_ret == 0) // unexpected symbol error
		{
			printf("FAILURE: Unexpected symbol '%s' found on line %d.\n", errorString, lineCount);
			break;		// abort LA
		}

		// add token to list (if necessary)
		if (CurrentToken->mType!= error) {
			
			int_stack_temp.push(CurrentToken->mType);
			TokenList[tokenCount++] = CurrentToken;
		}
	} while (nextChar != EOF);
	if (lex_ret == 1)
	{
		printf(")\nProgram passed lexical analysis\n");
	}


DBG:	if (tokenCount > 0)
		{
			cout << "Token Type Name :: Token Type (numerical) :: Token Value (if applicable)\n";
			for (int i = 0; i < tokenCount; i++){
				cout << TokenList[i]->mTokName << " :: " << TokenList[i]->mType << " :: " << TokenList[i]->mValue << endl;
			}
		}

	//============================================================================================================================================
	//=================================================BEGIN LL PARESER===========================================================================
	//============================================================================================================================================
	
	//initialize the node_stack
	node_stack.push(dollarn);
	node_stack.push(PGMn);


	//take input and flip it onto stack
	while(!int_stack_temp.empty()){
		int temporary;
		temporary = int_stack_temp.top();
		int_stack_temp.pop();
		int_stack.push(temporary);
	}


	while(!int_stack.empty()){
		LLparser(int_stack, node_stack); 
	}
	
	cout << "WE DID IT FAM" << endl;

	system("PAUSE");
	return 0;
}

void getChar()
{
	// grab character
	nextChar = input_file.get();

	// count lines
	if (nextChar == '\n')
		lineCount++;

	// determine type
	if (!input_file.eof())
	{
		if (isdigit(nextChar))
			charClass = DIGIT;
		else if (isalpha(nextChar))
			charClass = LETTER;
		else
			charClass = UNKNOWN;
	}
	else
		charClass = EOF; // already defined as -1
}

int lookup(char ch)
{
	switch (ch)
	{
	case '/':
		// possibilities: comment, slash
		peekNext();
		if (peekedChar == '/')
		{
			// it's a comment
			return comment;
		}
		CurrentToken->mTokName = "slash";
		return slash;
		break;

	case '"':
		return t_string;
		CurrentToken->mTokName = "string";
		break;
	
	case '$':
		return dollar;
		CurrentToken->mTokName = "dollar";

	case ',':
		CurrentToken->mTokName = "comma";
		return comma;
		break;

	case ';':
		CurrentToken->mTokName = "semi";
		return semi;
		break;

	case '<':
		// possibilities: angle1, ople, opshl
		peekNext();
		if (peekedChar == '=')
		{
			CurrentToken->mTokName = "ople";
			return ople;
		}
		else if (peekedChar == '<')
		{
			CurrentToken->mTokName = "opshl";
			return opshl;
		}
		else
		{
			CurrentToken->mTokName = "angle1";
			return angle1;
		}
		break;

	case '>':
		// possibilities: angle2, opge, opshr
		peekNext();
		if (peekedChar == '=')
		{
			CurrentToken->mTokName = "opge";
			return opge;
		}
		else if (peekedChar == '>')
		{
			CurrentToken->mTokName = "opshr";
			return opshr;
		}
		else
		{
			CurrentToken->mTokName = "angle2";
			return angle2;
		}
		break;

	case '{':
		CurrentToken->mTokName = "brace1";
		return brace1;
		break;

	case '}':
		CurrentToken->mTokName = "brace2";
		return brace2;
		break;

	case '[':
		CurrentToken->mTokName = "bracket1";
		return bracket1;
		break;

	case ']':
		CurrentToken->mTokName = "bracket2";
		return bracket2;
		break;

	case '(':
		CurrentToken->mTokName = "parens1";
		return parens1;
		break;

	case ')':
		CurrentToken->mTokName = "parens2";
		return parens2;
		break;

	case '*':
		CurrentToken->mTokName = "aster";
		return aster;
		break;

	case '^':
		CurrentToken->mTokName = "caret";
		return caret;
		break;

	case ':':
		CurrentToken->mTokName = "colon";
		return colon;
		break;

	case '.':
		CurrentToken->mTokName = "dot";
		return dot;
		break;

	case '=':
		// possibilities: equal, opeq
		peekNext();
		if (peekedChar == '=')
		{
			CurrentToken->mTokName = "opeq";
			return opeq;
		}
		CurrentToken->mTokName = "equal";
		return equal;
		break;

	case '-':
		// possibilities: minus, oparrow
		peekNext();
		if (peekedChar == '>')
		{
			CurrentToken->mTokName = "oparrow";
			return oparrow;
		}
		CurrentToken->mTokName = "minus";
		return minus;
		break;

	case '+':
		CurrentToken->mTokName = "plus";
		return plus;
		break;

	case '!':
		peekNext();
		if (peekedChar == '=')
		{
			CurrentToken->mTokName = "opne";
			return opne;
		}
		else
			return error;
		break;

	default:
		return error;

	}
	return 0;
}

int lex()
{
	bool float_point = false;
	lexLength = 0;
	skipBlanks();
	switch (charClass)
	{
	case LETTER:
		// possibilities: ident, kwd
		while (charClass != UNKNOWN && charClass != EOF)
		{
			if (charClass != UNKNOWN && charClass != EOF)
			{
				addChar();
				peekNext();
				if (isEnd(peekedChar))
				{
					// must be end of "word"
					break;
				}
			}
			// otherwise get next char
			getChar();

			// allows underscore into identifier
			if (nextChar == '_')
			{
				addChar();
				getChar();
			}
		}

		// Determine type of letter token: keyword or identifier
		// is this a keyword?
		if (isKWD())
		{
			CurrentToken->mTokName = string("kwd").append(lexeme);  // similar to: "kwd" + "prog"
			printf("(:token %d kwd%s)\n", lineCount, lexeme);
		}
		else	// an identifier
		{
			// however, "_" is NOT an identifier
			if (!strncmp(lexeme, "_", lexLength))
			{
				errorString[0] = '_';
				errorString[1] = 0;
				return 0;
			}
			CurrentToken->mTokName = "ident";
			CurrentToken->mType = ident;
			CurrentToken->mValue = lexeme;
			printf("(:token %d ident :str \"%s\")\n", lineCount, lexeme);
		}
		getChar();

		
		return 1;
		break;

	case DIGIT:
		// possibilities: t_int, t_float
		lexLength = 0;
		float_point = false;
		CurrentToken->mType = t_int;
		while (charClass == DIGIT || lookup(nextChar) == dot)
		{
			// ensure only digits/dots are added
			if (charClass == DIGIT || lookup(nextChar) == dot)
			{
				if (nextChar == '.')//'.'			// floating point picked up
				{
					if (float_point == false)	// determine if token is int or float
					{
						CurrentToken->mType = t_float;	// add floating point and toggle
						addChar();
						getChar();
						float_point = true;
						continue;
													// floating point shouldn't be the end
					}
					else						// look pal, you can't have two floating points
					{
						CurrentToken->mType = error;
						errorString[0] = nextChar;
						errorString[1] = 0;
						return 0;
					}
					
				}
				else
				{
					// char is a digit
					addChar();
				}
			}

			// check for end of number
			peekNext();
			if (peekClass == UNKNOWN && !isEnd(peekedChar))
			{
				// not sure what peeked char is
				errorString[0] = peekedChar;
				return 0;
			}
			else if (isEnd(peekedChar) && peekClass != FLTPT)
			{
				// end of token
				break;
			}

			getChar();
		}

		CurrentToken->mValue = lexeme;		// puts found lexeme value into token
		CurrentToken->mTokName = "t_float";	// swapped to "int" if type is int

		// Type identification: int or float
		if (CurrentToken->mType != t_float)
		{
			CurrentToken->mTokName = "t_int";
			printf("(:token %d int :str \"%s\")\n", lineCount, lexeme);
		}
		else
		{
			printf("(:token %d float :str \"%s\")\n", lineCount, lexeme);
		}
		getChar();
		break;
		
	case UNKNOWN:
		// possibilities: comment, t_string, comma, semi, delimiter, punct, other
		CurrentToken->mType = lookup(nextChar);
		if (CurrentToken->mType == error)
		{
			errorString[0] = nextChar;
			errorString[1] = 0;
			return 0;
		}

		switch (CurrentToken->mType)
		{
		case comment:
			// skip over comment
			while (nextChar != '\n')
				getChar();
			CurrentToken->mValue = lexeme;
			CurrentToken->mTokName = "comment";
			printf("(:token %d string :str \"%s\")\n", lineCount, lexeme);
			return 1;
			break;

		case t_string:
			// try to find beginning and end
			getChar(); // ignore first '"'
			while (nextChar != '"')
			{
				addChar();
				getChar();
			}
			CurrentToken->mValue = lexeme;
			CurrentToken->mTokName = "t_string";
			printf("(:token %d string :str \"%s\")\n", lineCount, lexeme);
			getChar(); // ignore last '"'
			return 1;
			break;

		// single-char delims
		case comma:
			addChar();
			break;

		case semi:
			addChar();
			break;

		case angle1:
			addChar();
			break;

		case angle2:
			addChar();
			break;

		case bracket1:
			addChar();
			break;

		case bracket2:
			addChar();
			break;

		case parens1:
			addChar();
			break;

		case parens2:
			addChar();
			break;

		case aster:
			addChar();
			break;

		case caret:
			addChar();
			break;

		case colon:
			addChar();
			break;

		case dot:
			addChar();
			break;

		case equal:
			addChar();
			break;

		// multi-char delims
		case oparrow:
			addChar(); // add -
			getChar();
			addChar(); // add >
			break;

		case opeq:
			addChar(); // add =
			getChar();
			addChar(); // add =
			break;

		case opne:
			addChar(); // add !
			getChar();
			addChar(); // add =
			break;

		case ople:
			addChar(); // add <
			getChar();
			addChar(); // add =
			break;

		case opge:
			addChar(); // add >
			getChar();
			addChar(); // add =
			break;

		case opshl:
			addChar(); // add <
			getChar();
			addChar(); // add <
			break;

		case opshr:
			addChar(); // add >
			getChar();
			addChar(); // add >
			break;
		}

		// ensure end of token
		peekNext();
		if (isEnd(peekedChar) || isalnum(peekedChar) || peekClass == EOF || peekClass == SPC)
		{
			//lookup(nextChar);
			if (CurrentToken->mType != t_string)
			{
				// This is a special case to check before confirming lexeme, we have to update the lexeme now
				strncpy_s(lexeme, CurrentToken->mTokName.c_str(), _TRUNCATE);
				printf("(:token %d %s)\n", lineCount, CurrentToken->mTokName.c_str());
			}
		}
		else
		{
			// if end of token isn't there, there's something wrong
			CurrentToken->mType = error;
			errorString[0] = nextChar;
			errorString[1] = 0;
			return 0;
		}
		getChar();
		break;
	}
	return 1;  // return success
}

bool isKWD()
{
	CurrentToken->mType = error;
	
	
	if (lexLength > 6) // keywords aren't longer than 6 characters
		return false;
	else
	{
		// keywords: prog, main, fcn, class, float, int, string, if, elseif, else, while, input, print, new, return
		// Note: !strncmp() means that it DOES match
		if (!strncmp(lexeme, "prog", 10))		CurrentToken->mType = kwdprog;
		if (!strncmp(lexeme, "main", 10))		CurrentToken->mType = kwdmain;
		if (!strncmp(lexeme, "fcn", 10))		CurrentToken->mType = kwdfcn;
		if (!strncmp(lexeme, "class", 10))		CurrentToken->mType = kwdclass;
		if (!strncmp(lexeme, "float", 10))		CurrentToken->mType = kwdfloat;
		if (!strncmp(lexeme, "int", 10))		CurrentToken->mType = kwdint;
		if (!strncmp(lexeme, "string", 10))		CurrentToken->mType = kwdstring;
		if (!strncmp(lexeme, "if", 10))			CurrentToken->mType = kwdif;
		if (!strncmp(lexeme, "elseif", 10))		CurrentToken->mType = kwdelseif;
		if (!strncmp(lexeme, "else", 10))		CurrentToken->mType = kwdelse;
		if (!strncmp(lexeme, "while", 10))		CurrentToken->mType = kwdwhile;
		if (!strncmp(lexeme, "input", 10))		CurrentToken->mType = kwdinput;
		if (!strncmp(lexeme, "print", 10))		CurrentToken->mType = kwdprint;
		if (!strncmp(lexeme, "new", 10))		CurrentToken->mType = kwdnew;
		if (!strncmp(lexeme, "return", 10))		CurrentToken->mType = kwdreturn;
		
		// did any of these match?
		if (CurrentToken->mType != error)
			return true;
	}
	return false;
}

void peekNext()
{
	peekedChar = input_file.peek();

	// Determine a type for convenience
	if (!input_file.eof())
	{
		if (isdigit(peekedChar))
			peekClass = DIGIT;
		else if (isalpha(peekedChar))
			peekClass = LETTER;
		else if (peekedChar == '.')
			peekClass = FLTPT;
		else if (isspace(peekedChar))
			peekClass = SPC;
		else
		{
			peekClass = UNKNOWN;
		}
	}
	else
		peekClass = EOF; // already defined as -1	else

}

void addChar()
{
	// Adding a char requires at least two spaces: one for char itself, other for terminal
	// Hence, the LEXEME_SIZE - 2
	if (lexLength < (LEXEME_SIZE - 2)) 
	{
		lexeme[lexLength++] = nextChar; // auto incremented
		lexeme[lexLength] = 0;			// string terminal
	}
	else
	{
		cout << "lexeme too large.\n";
	}
}

void skipBlanks()
{
	// repeatedly grabs char until it isn't whitespace
	while (isspace(nextChar))
		getChar();
}

bool isEnd(char ch)
{
	// Designed to "beautify" the lexer's if statements
	// The lookup below replaces CurrentToken->mTokName inadvertantly, and so we keep the old one
	string old_tokname;
	old_tokname = CurrentToken->mTokName;

	// get char type
	int _thisCharType = lookup(ch);
	
	// replace old token name
	CurrentToken->mTokName = old_tokname;

	// space
	if (isspace(ch))
		return true;

	// end of token is normally (almost) anything lookup says isn't error
	if (_thisCharType != error)
		return true;
	else
		return false;
}

//=================================================================================
//==================================NEW STUFF===========================
//=================================================================================
void clear(int x[]){		//clears an array
	for(int i = 0; i < 10; i++){
		x[i] = -1;
	}
}
//=============================================================================
// this is the driver for the ll parser
// it evaluates the stack and outputs the correct ll pareser rule 
//==============================================================================
void LLparser(stack<int> & int_stack, stack<Nodes> & node_stack){ //takes input stack as argument 
	//2 stacks one of NODES and one of tokens
	
	Nodes top_node;//token on top of Node stack
	Symbol node_sym;//symbol associated with node on top of stack
	top_node = node_stack.top();
	node_sym = list[top_node.sym_ID];

	int top_int;//int "aka" token on top of Node stack 
	top_int  = int_stack.top();
	int ll_nont_cord;//takes top_int and finds it in ll parser x

	int iii;
	iii = 0;
	if(node_sym.sym_kind == 1){

		while (iii <= 100){ //find the nont for the terminal in ll parser
			if(ll_parser[iii][0] == node_sym.token_ID){
				ll_nont_cord = iii;
				break;
			}
			iii++;
		}
	}

	//================DO WE NEED TO FIND THE NON-T ASWELL ....MAYBE!==============


	
	if (node_sym.sym_kind == 0) {  //check for terminal symbol
		if(node_sym.token_ID == top_int) { //if it matches current input POP IT!
			int_stack.pop();
			node_stack.pop();
		}
	}
	else if (top_int == 1){
		int_stack.pop();
	}
	else if (ll_parser[ll_nont_cord][top_int] != error){ // I THNK THIS IS BACKAWRDS
		//llparser returns appropriate rule
		int returned = ll_parser[ll_nont_cord][top_int];
		Rule rule_ret = Rules[returned];

		//make children of x based of rule returned
		//pops parent off stack but still has pointer pointing to it
		//also pops children onto stack in reverse order
		birther(node_stack, top_node , rule_ret);
	}
	else {
		 error;
	}
}

//=======================================================================
// This function will take the Rule the LL parser outputs
// it will use the parent node and the Rule to create children
// and modify the stack, while keeping parent & child connected via ptrs
//=======================================================================
void birther(stack<Nodes> & pstak, Nodes  x, Rule rule) 
{
	x.kid_count=0;
	if(rule.rule_ID == 50){
		pstak.pop();
	}
	else{

		//first we have global pointer parentptr = & x
		//then we pop stack
		pstak.pop();
		//LOOP{
		int i=0;
		int b=0;
		while(rule.rhs_sym_ID[i]!= -1) {
			i++;
		}
		//starting from the far right 
		for(int j = i; j > 0; j--)
		{
			//make a new node with proper symbol
			//make a switch to make new varrs idk im stuck help pls
			int  sym_id;
			sym_id=rule.rhs_sym_ID[j-1];
		
			//now we look for the correct sym id in our nodes
			
			for(int t = 0; t <100; t++){
				if(nodel[t].sym_ID == sym_id){
					childptr = &nodel[t];

				}
			}
			//then we point global pointer childptr to the (next) rightmost child  

			//and point parents child point slot to it
			// we need to pass by value here the values are getting reset
			
			//next slot for next loop iter
			
			//pop child onto stack
			pstak.push(*childptr);
		}
		//parentptr= new Nodes;
	}
	//PST(*parentptr);

}


void PST(Nodes  a){
	cout << "( " << a.sym_ID << " ) --> ";
	for(int i = 0; i <a.kid_count; i++)	{
		cout << "( " << a.kids[i]->sym_ID << " )";
	}cout<< endl;
}