#include "automata.h"
#include "util.h"
#include "../constants.h"

Token automata::numeric_constants(char symbol, int & state) {
	switch (state) {
	case 0:
		if (isDigit(symbol)) {
			state = 1;
		}
		break;
	case 1:
		if (!isDigit(symbol) && !isLetter(symbol)) {
			state = 2;
			return NUMERIC_CONSTANT;
		}
		break;
	}
	return INVALID;
}

Token automata::identifiers(char symbol, int & state) {
	switch (state) {
	case 0:
		if (isLetter(symbol)) {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (!(isDigit(symbol) || isLetter(symbol))) {
			state = 2;
			return IDENTIFIER;
		}
		break;
	}
	return INVALID;
}

Token automata::whitespace(char symbol, int & state) {
	if (isWhitespace(symbol)) {
		return IGNORE;
	} else {
		return INVALID;
	}
}

Token automata::relational_operators(char symbol, int & state) {
	// < > <= >= ==

	switch (state) {
	case 0:
		if (symbol == '>' || symbol == '<') {
			state = 1;
		} else if (symbol == '=' || symbol == '!') {
			state = 3;
		}
		break;
	case 1:
		if (symbol == '=') {
			state = 2;
		} else if (isWhitespace(symbol) || isLetter(symbol) || isDigit(symbol)) {
			return RELATIONAL_OPERATOR;
		}
		break;
	case 2:
		if (isWhitespace(symbol) || isLetter(symbol) || isDigit(symbol)) {
			return RELATIONAL_OPERATOR;
		}
		break;
	case 3:
		if (symbol == '=') {
			state = 2;
		}
		break;
	}

	return INVALID;
}

Token automata::literal_constant(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == '\'') {
			state = 1;
		}
		break;
	case 1:
		if (isLetter(symbol)) {
			state = 2;
		}
		break;
	case 2:
		if (symbol == '\'') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (isWhitespace(symbol) || isOperator(symbol)) {
			return LITERAL_CONSTANT;
		}
		break;
	}
	return INVALID;
}

Token automata::special_symbols(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == '+') {
			state = 1;
		} else if (symbol == '-') {
			state = 2;
		} else if (symbol == '*') {
			state = 3;
		} else if (symbol == '/') {
			state = 4;
		} else if (symbol == '=') {
			state = 5;
		} else if (symbol == '{') {
			state = 6;
		} else if (symbol == '}') {
			state = 7;
		} else if (symbol == '[') {
			state = 8;
		} else if (symbol == ']') {
			state = 9;
		} else if (symbol == ',') {
			state = 10;
		} else if (symbol == '(') {
			state = 11;
		} else if (symbol == ')') {
			state = 12;
		}
		break;
	case 1:
		if (!isOperator(symbol))
			return ADDITION_OPERATOR;
		break;
	case 2:
		if (!isOperator(symbol))
			return SUBTRACTION_OPERATOR;
		break;
	case 3:
		if (!isOperator(symbol))
			return MULTIPLICATION_OPERATOR;
		break;
	case 4:
		if (!isOperator(symbol))
			return DIVISION_OPERATOR;
		break;
	case 5:
		if (!isOperator(symbol))
			return ASSIGNMENT_OPERATOR;
		break;
	case 6:
		return L_BRACE;
	case 7:
		return R_BRACE;
	case 8:
		return L_SUBSCRIPT_OPERATOR;
	case 9:
		return R_SUBSCRIPT_OPERATOR;
	case 10:
		return COMMA;
	case 11:
		return L_PARENTHESES;
	case 12:
		return R_PARENTHESES;
	}
	return INVALID;
}


Token automata::if_stmt(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'i') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'f') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (isWhitespace(symbol)) {
			return IF_STATEMENT;
		}
		break;
	}
	return INVALID;
}

Token automata::else_stmt(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'e') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'l') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 's') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (symbol == 'e') {
			state = 4;
		} else {
			state = -1;
		}
		break;
	case 4:
		if (isWhitespace(symbol)) {
			return ELSE_STATEMENT;
		}
		break;
	}
	return INVALID;
}

Token automata::while_stmt(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'w') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'h') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 'i') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (symbol == 'l') {
			state = 4;
		} else {
			state = -1;
		}
		break;
	case 4:
		if (symbol == 'e') {
			state = 5;
		} else {
			state = -1;
		}
		break;
	case 5:
		if (isWhitespace(symbol)) {
			return WHILE_STATEMENT;
		}
		break;
	}
	return INVALID;
}

Token automata::return_stmt(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'r') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'e') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 't') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (symbol == 'u') {
			state = 4;
		} else {
			state = -1;
		}
		break;
	case 4:
		if (symbol == 'r') {
			state = 5;
		} else {
			state = -1;
		}
		break;
	case 5:
		if (symbol == 'n') {
			state = 6;
		} else {
			state = -1;
		}
		break;
	case 6:
		if (isWhitespace(symbol)) {
			return RETURN_STATEMENT;
		}
		break;
	}
	return INVALID;
}

Token automata::print_stmt(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'p') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'r') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 'i') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (symbol == 'n') {
			state = 4;
		} else {
			state = -1;
		}
		break;
	case 4:
		if (symbol == 't') {
			state = 5;
		} else {
			state = -1;
		}
		break;
	case 5:
		if (isWhitespace(symbol)) {
			return PRINT_STATEMENT;
		}
		break;
	}
	return INVALID;
}

Token automata::get_stmt(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'g') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'e') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 't') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (isWhitespace(symbol)) {
			return GET_STATEMENT;
		}
		break;
	}
	return INVALID;
}

Token automata::def_stmt(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'd') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'e') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 'f') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (isWhitespace(symbol)) {
			return DEF_STATEMENT;
		}
		break;
	}
	return INVALID;
}

Token automata::int_t(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'i') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'n') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 't') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (isWhitespace(symbol) || isSubscript(symbol)) {
			return INT_TYPE;
		}
		break;
	}
	return INVALID;
}

Token automata::char_t(char symbol, int & state) {
	switch (state) {
	case 0:
		if (symbol == 'c') {
			state = 1;
		} else {
			state = -1;
		}
		break;
	case 1:
		if (symbol == 'h') {
			state = 2;
		} else {
			state = -1;
		}
		break;
	case 2:
		if (symbol == 'a') {
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (symbol == 'r') {
			state = 4;
		} else {
			state = -1;
		}
		break;
	case 4:
		if (isWhitespace(symbol) || isSubscript(symbol)) {
			return CHAR_TYPE;
		}
		break;
	}
	return INVALID;
}