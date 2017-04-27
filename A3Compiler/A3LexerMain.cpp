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
#include "Token.h"
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

ifstream input_file;
char fileName[100] = "source.txt";
char errorString[100];





/// Used for charClass and peekClass
#define LETTER 0
#define DIGIT 1
#define FLTPT 3
#define SPC 4
#define UNKNOWN 99



int main()
{
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
		if (CurrentToken->mTokName != "error")
			TokenList[tokenCount++] = CurrentToken;

	} while (nextChar != EOF);
	if (lex_ret == 1)
	{
		printf(")\nProgram passed lexical analysis\n");
	}


DBG:	if (tokenCount > 0)
			for (int i = 0; i < tokenCount; i++)
				cout << TokenList[i]->mTokName << " :: " << TokenList[i]->mType << endl;

	// "pause" console to see results
	printf("\nPress any key to exit.\n");
	cin.ignore();

	// close file
	input_file.close();

	// TODO: pass to parser here

	// end parsing

	// delete dynamic pointers in array
	for (int i = 0; i < tokenCount; i++)
		delete TokenList[i];

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
				if (nextChar == '.')			// floating point picked up
				{
					if (float_point == false)	// determine if token is int or float
					{
						CurrentToken->mType = t_float;	// add floating point and toggle
						addChar();
						getChar();
						float_point = true;
						continue;				// floating point shouldn't be the end
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
		CurrentToken->mTokName = "float";	// swapped to "int" if type is int

		// Type identification: int or float
		if (CurrentToken->mType != t_float)
		{
			CurrentToken->mTokName = "int";
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
			CurrentToken->mTokName = lexeme;
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
		peekClass = EOF; // already defined as -1
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