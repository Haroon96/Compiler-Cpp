#pragma once

#include "../lex/LexicalAnalyzer.h"

enum Token;

class SyntaxAnalyzer {

public:

	SyntaxAnalyzer(LexicalAnalyzer* lex) {
		this->lex = lex;
		this->lookahead = nullptr;
	}
	int getLineNumber();
	void parse();

private:

	LexicalAnalyzer* lex;
	TokenLexeme* lookahead;

	bool match(Token);

	// grammar implementations
	// may throw exceptions
	void start();
	void function_declaration();
	void parameters();
	void variable_declaration();
	void data_type();
	void additional_parameters();
};