#include "SyntaxAnalyzer.h"
#include "../compiler.h"

int SyntaxAnalyzer::getLineNumber() {
	return lex->getLineNumber();
}

// may throw exception
void SyntaxAnalyzer::parse() {

	lookahead = lex->nextToken();

	if (lookahead == nullptr) {
		throw std::exception("An error while parsing file. No tokens found.");
	}
	while (lex->hasNextToken()) {
		start();
	}
}

bool SyntaxAnalyzer::match(Token token) {
	if (lookahead->token != token) {
		throw std::exception(std::string("Expected " + getTokenName(token) + " but encountered " + getTokenName(lookahead->token) + " instead").c_str());
	}
	lookahead = lex->nextToken();
	return true;
}

void SyntaxAnalyzer::start() {
	if (lookahead->token == DEF_STATEMENT) {
		function_declaration();
	} else if (lookahead->token == INT_TYPE || lookahead->token == CHAR_TYPE) {
		variable_declaration();
	} else {
		throw std::exception("Expected variable or function declaration");
	}
}

void SyntaxAnalyzer::function_declaration() {
	match(DEF_STATEMENT);
	match(IDENTIFIER);
	match(L_PARENTHESES);
	parameters();
	match(R_PARENTHESES);
}

void SyntaxAnalyzer::parameters() {
	
	if (lookahead->token != R_PARENTHESES) {
		variable_declaration();
		additional_parameters();
	}
}

void SyntaxAnalyzer::variable_declaration() {
	data_type();
	match(IDENTIFIER);
}

void SyntaxAnalyzer::data_type() {
	if (lookahead->token == INT_TYPE) {
		match(INT_TYPE);
	} else if (lookahead->token == CHAR_TYPE) {
		match(CHAR_TYPE);
	} else {
		throw std::exception("Invalid data type");
	}
}

void SyntaxAnalyzer::additional_parameters() {
	if (lookahead->token == COMMA) {
		match(COMMA);
		parameters();
	}
}
