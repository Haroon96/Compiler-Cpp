#include <iostream>
#include "lex/lex.h"
using namespace std;


void test(std::vector<lex::TokenLexeme*> tlp) {


	char *values[] = { 
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


	for (int i = 0; i < tlp.size(); ++i) {
		cout << "\t";
		cout << tlp[i]->lexeme << "\t\t";
		cout << values[tlp[i]->token - 300] << endl;
	}

}

int main(int argc, char *argv[]) {
	/*
	if (argc < 2) {
		cerr << "Source file name not specified" << endl;
		return 1;
	}

	ifstream src(argv[1]);*/

	ifstream src("test.txt");

	if (!src.is_open()) {
		cerr << "Invalid source file name specified" << endl;
		return 1;
	}

	std::vector<lex::TokenLexeme*> tlp = lex::analyze(&src);


	for (int j = 0; j < tlp.size(); ++j) {
		if (tlp[j]->token == lex::IDENTIFIER) {
			cout << tlp[j]->lexeme << endl;
		}
	}

	std::vector<std::string> ids = lex::make_symbol_table(tlp);

	for (int i = 0; i < ids.size(); ++i) {
		cout << i << " " << ids[i] << endl;
	}

	for (int j = 0; j < tlp.size(); ++j) {
		if (tlp[j]->token == lex::IDENTIFIER) {
			cout << tlp[j]->lexeme << endl;
		}
	}

//	test(tlp);

	src.close();
	return 0;
}

