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

enum SymbolType {
	FUNCTION,
	VARIABLE,
	UNDECLARED
};

std::string getTokenName(Token t);
std::string getSymbolType(SymbolType t);

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

struct Symbol {
	Symbol(std::string name) :name(name), offset(-1), type(UNDECLARED) {
	}
	void setOffset(int offset) {
		this->offset = offset;
	}
	void setType(SymbolType type) {
		this->type = type;
	}
	std::string getName() {
		return name;
	}
	int getOffset() {
		return offset;
	}
	SymbolType getType() {
		return type;
	}
private:
	std::string name;
	int offset;
	SymbolType type;
};
