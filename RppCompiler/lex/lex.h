#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace lex {

	enum Token {
		INVALID = 300,
		NUMERIC_CONSTANT,
		IDENTIFIER,
		RELATIONAL_OPERATOR,
		UNKNOWN,
		LITERAL_CONSTANT,
		ADDITION_OPERATOR,
		SUBTRACTION_OPERATOR,
		MULTIPLICATION_OPERATOR,
		DIVISION_OPERATOR,
		ASSIGNMENT_OPERATOR,
		IF_STATEMENT,
		ELSE_STATEMENT,
		WHILE_STATEMENT,
		RETURN_STATEMENT,
		PRINT_STATEMENT,
		GET_STATEMENT,
		DEF_STATEMENT,
		INT_TYPE,
		CHAR_TYPE,
		IGNORE,
		L_BRACE,
		R_BRACE,
		L_SUBSCRIPT_OPERATOR,
		R_SUBSCRIPT_OPERATOR,
		COMMA,
		L_PARENTHESES,
		R_PARENTHESES
	};

	struct TokenLexeme {
		int token;
		std::string lexeme;
		TokenLexeme(int token, std::string lexeme) : token(token), lexeme(lexeme) {}
	};

	std::vector<TokenLexeme*> analyze(std::ifstream *file);
	std::vector<std::string> make_symbol_table(std::vector<TokenLexeme*>);
}