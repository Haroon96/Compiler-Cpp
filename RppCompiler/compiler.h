#pragma once

#include <string>

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
std::string getTokenName(Token t);

struct TokenLexeme {
	TokenLexeme(Token token, std::string lexeme) : token(token), lexeme(lexeme) {}
	Token getToken() {
		if (this == nullptr) {
			throw std::exception("Unexpected end of file");
		}
		return token;
	}
	std::string getLexeme() {
		if (this == nullptr) {
			throw std::exception("Unexpected end of file");
		}
		return lexeme;
	}
private:
	Token token;
	std::string lexeme;
};
