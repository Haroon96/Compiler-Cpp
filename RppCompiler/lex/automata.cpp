#include "automata.h"
#include "util.h"
#include "lex.h"

int lex::numeric_constants(char symbol, int & state) {
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

int lex::identifiers(char symbol, int & state) {
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

int lex::whitespace(char symbol, int & state) {
	if (isWhitespace(symbol)) {
		return IGNORE;
	} else {
		return INVALID;
	}
}

int lex::relational_operators(char symbol, int & state) {
	// < > <= >= ==

	switch (state) {
	case 0:
		if (symbol == '>' || symbol == '<') {
			state = 1;
		} else if (symbol == '=') {
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

int lex::literal_constant(char symbol, int & state) {
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
		if (isWhitespace(symbol)) {
			return LITERAL_CONSTANT;
		}
		break;
	}
	return INVALID;
}

int lex::operators(char symbol, int & state) {
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
	}
	return INVALID;
}

int lex::if_stmt(char symbol, int & state) {
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
			state = 3;
		} else {
			state = -1;
		}
		break;
	case 3:
		if (isWhitespace(symbol)) {
			return IF_STATEMENT;
		}
		break;
	}
	return INVALID;
}

int lex::else_stmt(char symbol, int & state) {
	return INVALID;
}

int lex::while_stmt(char symbol, int & state) {
	return INVALID;
}

int lex::return_stmt(char symbol, int & state) {
	return INVALID;
}

int lex::print_stmt(char symbol, int & state) {
	return INVALID;
}

int lex::get_stmt(char symbol, int & state) {
	return INVALID;
}

int lex::int_t(char symbol, int & state) {
	return INVALID;
}

int lex::char_t(char symbol, int & state) {
	return INVALID;
}

