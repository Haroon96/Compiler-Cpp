#pragma once

enum Token;

namespace automata {
	Token numeric_constants(char symbol, int& state);
	Token identifiers(char symbol, int &state);
	Token whitespace(char symbol, int &state);
	Token relational_operators(char symbol, int &state);
	Token literal_constant(char symbol, int &state);
	Token special_symbols(char symbol, int &state);
	Token if_stmt(char symbol, int &state);
	Token else_stmt(char symbol, int &state);
	Token while_stmt(char symbol, int &state);
	Token return_stmt(char symbol, int &state);
	Token print_stmt(char symbol, int &state);
	Token get_stmt(char symbol, int &state);
	Token def_stmt(char symbol, int &state);
	Token int_t(char symbol, int &state);
	Token char_t(char symbol, int &state);
}