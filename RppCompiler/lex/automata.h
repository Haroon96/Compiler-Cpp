#pragma once

namespace lex {
	
	int numeric_constants(char symbol, int& state);
	int identifiers(char symbol, int &state);
	int whitespace(char symbol, int &state);
	int relational_operators(char symbol, int &state);
	int literal_constant(char symbol, int &state);
	int special_symbols(char symbol, int &state);

	// TODO
	int if_stmt(char symbol, int &state);
	int else_stmt(char symbol, int &state);
	int while_stmt(char symbol, int &state);
	int return_stmt(char symbol, int &state);
	int print_stmt(char symbol, int &state);
	int get_stmt(char symbol, int &state);
	int def_stmt(char symbol, int &state);
	int int_t(char symbol, int &state);
	int char_t(char symbol, int &state);
}