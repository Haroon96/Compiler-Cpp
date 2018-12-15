#pragma once

#include "../lex/LexicalAnalyzer.h"
#include "../translator/Translator.h"
#include <sstream>

enum Token;
enum SymbolType;
struct Symbol;
struct MethodSymbol;

class Parser {

public:
	Parser(LexicalAnalyzer* lex, Translator *translator);
	~Parser();
	int getLineNumber();
	void parse();
	std::ostringstream* getStream();

private:

	LexicalAnalyzer* lex;
	Translator* translator;
	SymbolTable *curr_scope;
	SymbolTable *global_scope;
	TokenLexeme* lookahead;

	int depth;
	std::ostringstream* stream;

	void start_new_scope();
	void end_of_scope();

	void pad(std::string);
	bool match(Token);
	Symbol* locateSymbol(std::string lexeme);
	MethodSymbol *locateMethod(std::string lexeme);

	void increase_depth();
	void decrease_depth();

	std::string process_token_error(Token token);

	// grammar implementations
	// may throw exceptions
	void start();
	void function_declaration();
	int parameters(MethodSymbol *symbol);
	void variable_declaration();
	void variable_declaration_list(SymbolType type);
	void global_variable_declaration();
	void global_variable_declaration_list(SymbolType type);
	void variable_initialization();
	SymbolType data_type();
	void statements(int reset_point = 0);
	void statement();
	void print_statement();
	void get_statement();
	void if_statement();
	void else_statement();
	void while_statement();
	void return_statement();
	void identifier_prefix_statements();
	void method_call(MethodSymbol *symbol);
	void assignment();
	void expression();
	void expression_p();
	void term();
	void term_p();
	void factor();
	void identifier_prefix_factors();
	void boolean_expression();
	void relational_operator();
	void array_declaration();
};