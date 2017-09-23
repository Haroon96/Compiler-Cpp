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
		IGNORE
	};

	struct TokenLexeme {
		int token;
		std::string lexeme;
		TokenLexeme(int token, std::string lexeme) : token(token), lexeme(lexeme) {}
	};

	std::vector<TokenLexeme*> analyze(std::ifstream *file);
}