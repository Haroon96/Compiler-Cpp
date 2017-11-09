#pragma once

#include "../lex/LexicalAnalyzer.h"
#include "../translator/Translator.h"
#include <sstream>

enum Token;

class SyntaxAnalyzer {

public:

	SyntaxAnalyzer(LexicalAnalyzer* lex, Translator *translator);
	int getLineNumber();
	void parse();
	std::ostringstream* getStream();

private:

	LexicalAnalyzer* lex;
	Translator* translator;
	TokenLexeme* lookahead;

	int depth;
	std::ostringstream* stream;

	void pad(std::string);
	bool match(Token);

	void increase_depth();
	void decrease_depth();

	char* process_token_error(Token token);

	// grammar implementations
	// may throw exceptions
	void start();
	void function_declaration();
	void parameters();
	void variable_declaration();
	void variable_initialization();
	void data_type();
	void statements();
	void statement();
	void print_statement();
	void get_statement();
	void if_statement();
	void else_statement();
	void while_statement();
	void return_statement();
	void identifier_prefix_statements();
	void method_call();
	void assignment();
	void expression();
	void expression_p();
	void term();
	void term_p();
	void factor();
	void data_element();
	void boolean_expression();
	void relational_operator();
	void array_declaration();
};