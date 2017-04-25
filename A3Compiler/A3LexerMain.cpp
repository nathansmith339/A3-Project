// A3LexerMain.cpp
// Authors:			Nathan Smith and Jonathan Boe
// Description:		A lexical analyzer that asks for a file name (no spaces) and performs a lexical analysis.
//					While the analysis is done, the results are output to the console in a parenthesized format.

#include <iostream>
#include <fstream>
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
char tokenName[15];				// keeps track of a token's type for later
char peekedChar;				// holds a peeked character
int peekClass;					// keeps track of peekedChar's type
char nextChar;					// holds the character in question
int charClass;					// keeps track of nextChar's "type"
int tokenType;					// keeps track of the type of token found
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

	// ask for file
	cout << "Enter input filename (no spaces)\n>";
	cin.getline(fileName, 100);

	// open file
	input_file.open(fileName);
	if (input_file.fail())
	{
		printf("Error opening file. Filename of '%s' doesn't exist.\nExiting.\n", fileName);
	}
	else
	{
		printf("(:lang A3\n");
		getChar(); // grab first char to start off
		do
		{
			// processing
			
			lex_ret = lex();
			if (lex_ret == 0) // unexpected symbol error
			{
				printf("FAILURE: Unexpected symbol '%s' found on line %d.\n", errorString, lineCount);
				break;		// abort LA
			}
		} while (nextChar != EOF);
		if (lex_ret == 1)
		{
			printf(")\nProgram passed lexical analysis\n");
		}
	}

	// "pause" console to see results
	printf("\nPress any key to exit.\n");
	cin.ignore();

	// close file
	input_file.close();
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
		strncpy_s(tokenName, "slash", 15);
		return slash;
		break;

	case '"':
		return t_string;
		strncpy_s(tokenName, "string", 15);
		break;

	case ',':
		strncpy_s(tokenName, "comma", 15);
		return comma;
		break;

	case ';':
		strncpy_s(tokenName, "semi", 15);
		return semi;
		break;

	case '<':
		// possibilities: angle1, ople, opshl
		peekNext();
		if (peekedChar == '=')
		{
			strncpy_s(tokenName, "ople", 15);
			return ople;
		}
		else if (peekedChar == '<')
		{
			strncpy_s(tokenName, "opshl", 15);
			return opshl;
		}
		else
		{
			strncpy_s(tokenName, "angle1", 15);
			return angle1;
		}
		break;

	case '>':
		// possibilities: angle2, opge, opshr
		peekNext();
		if (peekedChar == '=')
		{
			strncpy_s(tokenName, "opge", 15);
			return opge;
		}
		else if (peekedChar == '>')
		{
			strncpy_s(tokenName, "opshr", 15);
			return opshr;
		}
		else
		{
			strncpy_s(tokenName, "angle2", 15);
			return angle2;
		}
		break;

	case '{':
		strncpy_s(tokenName, "brace1", 15);
		return brace1;
		break;

	case '}':
		strncpy_s(tokenName, "brace2", 15);
		return brace2;
		break;

	case '[':
		strncpy_s(tokenName, "bracket1", 15);
		return bracket1;
		break;

	case ']':
		strncpy_s(tokenName, "bracket2", 15);
		return bracket2;
		break;

	case '(':
		strncpy_s(tokenName, "parens1", 15);
		return parens1;
		break;

	case ')':
		strncpy_s(tokenName, "parens2", 15);
		return parens2;
		break;

	case '*':
		strncpy_s(tokenName, "aster", 15);
		return aster;
		break;

	case '^':
		strncpy_s(tokenName, "caret", 15);
		return caret;
		break;

	case ':':
		strncpy_s(tokenName, "colon", 15);
		return colon;
		break;

	case '.':
		strncpy_s(tokenName, "dot", 15);
		return dot;
		break;

	case '=':
		// possibilities: equal, opeq
		peekNext();
		if (peekedChar == '=')
		{
			strncpy_s(tokenName, "opeq", 15);
			return opeq;
		}
		strncpy_s(tokenName, "equal", 15);
		return equal;
		break;

	case '-':
		// possibilities: minus, oparrow
		peekNext();
		if (peekedChar == '>')
		{
			strncpy_s(tokenName, "oparrow", 15);
			return oparrow;
		}
		strncpy_s(tokenName, "minus", 15);
		return minus;
		break;

	case '+':
		strncpy_s(tokenName, "plus", 15);
		return plus;
		break;

	case '!':
		peekNext();
		if (peekedChar == '=')
		{
			strncpy_s(tokenName, "opne", 15);
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
			printf("(:token %d ident :str \"%s\")\n", lineCount, lexeme);
		}
		getChar();
		return 1;
		break;

	case DIGIT:
		// possibilities: t_int, t_float
		lexLength = 0;
		float_point = false;
		tokenType = t_int;
		while (charClass == DIGIT || lookup(nextChar) == dot)
		{
			// ensure only digits/dots are added
			if (charClass == DIGIT || lookup(nextChar) == dot)
			{
				if (nextChar == '.')			// floating point picked up
				{
					if (float_point == false)	// determine if token is int or float
					{
						tokenType = t_float;	// add floating point and toggle
						addChar();
						getChar();
						float_point = true;
						continue;				// floating point shouldn't be the end
					}
					else						// look pal, you can't have two floating points
					{
						tokenType = error;
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

		if (tokenType != t_float)
		{
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
		tokenType = lookup(nextChar);
		if (tokenType == error)
		{
			errorString[0] = nextChar;
			errorString[1] = 0;
			return 0;
		}

		switch (tokenType)
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
			if (tokenType != t_string)
				printf("(:token %d %s)\n", lineCount, tokenName);
		}
		else
		{
			// if end of token isn't there, there's something wrong
			tokenType = error;
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
	tokenType = error;
	
	
	if (lexLength > 6) // keywords aren't longer than 6 characters
		return false;
	else
	{
		// keywords: prog, main, fcn, class, float, int, string, if, elseif, else, while, input, print, new, return
		// Note: !strncmp() means that it DOES match
		if (!strncmp(lexeme, "prog", 10))		tokenType = kwdprog;
		if (!strncmp(lexeme, "main", 10))		tokenType = kwdmain;
		if (!strncmp(lexeme, "fcn", 10))		tokenType = kwdfcn;
		if (!strncmp(lexeme, "class", 10))		tokenType = kwdclass;
		if (!strncmp(lexeme, "float", 10))		tokenType = kwdfloat;
		if (!strncmp(lexeme, "int", 10))		tokenType = kwdint;
		if (!strncmp(lexeme, "string", 10))		tokenType = kwdstring;
		if (!strncmp(lexeme, "if", 10))			tokenType = kwdif;
		if (!strncmp(lexeme, "elseif", 10))		tokenType = kwdelseif;
		if (!strncmp(lexeme, "else", 10))		tokenType = kwdelse;
		if (!strncmp(lexeme, "while", 10))		tokenType = kwdwhile;
		if (!strncmp(lexeme, "input", 10))		tokenType = kwdinput;
		if (!strncmp(lexeme, "print", 10))		tokenType = kwdprint;
		if (!strncmp(lexeme, "new", 10))		tokenType = kwdnew;
		if (!strncmp(lexeme, "return", 10))		tokenType = kwdreturn;
		
		// did any of these match?
		if (tokenType != error)
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
	// The lookup below replaces tokenName inadvertantly, and so we keep the old one
	char old_tokname[40];
	strncpy_s(old_tokname, tokenName, 40);

	// get char type
	int _thisCharType = lookup(ch);
	
	// replace old token name
	strncpy_s(tokenName, old_tokname, 40);

	// space
	if (isspace(ch))
		return true;

	// end of token is normally (almost) anything lookup says isn't error
	if (_thisCharType != error)
		return true;
	else
		return false;
}