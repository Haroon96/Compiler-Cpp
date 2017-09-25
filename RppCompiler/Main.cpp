#include <iostream>
#include "lex/LexicalAnalyzer.h"
using namespace std;



std::string lookup[] = {
	"INVALID",
	"NUMERIC_CONSTANT",
	"IDENTIFIER",
	"RELATIONAL_OPERATOR",
	"UNKNOWN",
	"LITERAL_CONSTANT",
	"ADDITION_OPERATOR",
	"SUBTRACTION_OPERATOR",
	"MULTIPLICATION_OPERATOR",
	"DIVISION_OPERATOR",
	"ASSIGNMENT_OPERATOR",
	"IF_STATEMENT",
	"ELSE_STATEMENT",
	"WHILE_STATEMENT",
	"RETURN_STATEMENT",
	"PRINT_STATEMENT",
	"GET_STATEMENT",
	"DEF_STATEMENT",
	"INT_TYPE",
	"CHAR_TYPE",
	"IGNORE",
	"L_BRACE",
	"R_BRACE",
	"L_SUBSCRIPT_OPERATOR",
	"R_SUBSCRIPT_OPERATOR",
	"COMMA",
	"L_PARENTHESES",
	"R_PARENTHESES"
};


void write(LexicalAnalyzer &lex) {
	ofstream words("words.txt");


	while (lex.hasNextToken()) {
		TokenLexeme* tl = lex.nextToken();
		if (tl != nullptr) {
			words << tl->lexeme << "\t\t";
			words << lookup[tl->token - 300] << endl;
		}
	}


	words.close();

	ofstream table("table.txt");

	for (std::vector<std::string>::iterator start = lex.getIdTblStart(), end = lex.getIdTblEnd(); start != end; ++start) {
		table << *start << endl;
	}

	table.close();
}


int main(int argc, char *argv[]) {

	if (argc < 2) {
		cerr << "Source file name not specified" << endl;
		return 1;
	}

	ifstream src(argv[1]);

	if (!src.is_open()) {
		cerr << "Invalid source file name specified" << endl;
		return 1;
	}

	LexicalAnalyzer lex(&src);

	write(lex);

	src.close();
	return 0;
}

