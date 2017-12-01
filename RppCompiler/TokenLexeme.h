#pragma once

#include <string>
#include "constants.h"

struct TokenLexeme {
public:
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
