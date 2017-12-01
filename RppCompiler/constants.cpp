#include "constants.h"

std::string getTokenName(Token t) {
	static std::string tokens[] = {
		"INVALID",
		"NUMERIC_CONSTANT",
		"IDENTIFIER",
		"RELATIONAL_OPERATOR",
		"UNKNOWN",
		"LITERAL_CONSTANT",
		"ADDITION_OPERATOR",
		"SUBTRACTION_OPERATOR",
		"MULTIPLICATION_OPERATOR",
		"DIVISION_OPERATOR",
		"ASSIGNMENT_OPERATOR",
		"IF_STATEMENT",
		"ELSE_STATEMENT",
		"WHILE_STATEMENT",
		"RETURN_STATEMENT",
		"PRINT_STATEMENT",
		"GET_STATEMENT",
		"DEF_STATEMENT",
		"INT_TYPE",
		"CHAR_TYPE",
		"IGNORE",
		"L_BRACE",
		"R_BRACE",
		"L_SUBSCRIPT_OPERATOR",
		"R_SUBSCRIPT_OPERATOR",
		"COMMA",
		"L_PARENTHESES",
		"R_PARENTHESES"
	};
	return tokens[t - 300];
}

std::string getSymbolType(SymbolType t) {
	static std::string symbol_types[] = {
		"METHOD",
		"INT_VAR",
		"CHAR_VAR"
	};
	return symbol_types[t];
}