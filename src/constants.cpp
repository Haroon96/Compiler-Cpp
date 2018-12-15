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

std::string getReadableOpCode(int i) {
	std::string OpCodeReadable[] = {
		"ADD",
		"SUB",
		"MUL",
		"DIV",
		"MOV",
		"OUT",
		"IN",
		"IF_GE_GOTO",
		"IF_LE_GOTO",
		"IF_L_GOTO",
		"IF_G_GOTO",
		"IF_E_GOTO",
		"IF_NE_GOTO",
		"GOTO",
		"PARAM",
		"SET",
		"RET",
		"MOV_TO_ARR",
		"MOV_FROM_ARR",
		"LABEL",
		"CHAR_OUT",
		"CALL",
		"ALLOC",
		"UNUSED",
		"CALL_CLEANUP",
		"CALL_CLEANUP_NO_RESULT",
		"EXIT"
	};
	return OpCodeReadable[i];
}