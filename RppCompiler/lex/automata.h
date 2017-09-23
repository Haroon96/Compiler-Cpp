#pragma once

namespace lex {
	
	int numeric_constants(char symbol, int& state);
	int identifiers(char symbol, int &state);
	int whitespace(char symbol, int &state);
	int relational_operators(char symbol, int &state);
	int literal_constant(char symbol, int &state);
	int operators(char symbol, int &state);

}