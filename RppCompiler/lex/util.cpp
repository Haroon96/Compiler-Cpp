#include "util.h"

bool isDigit(char c) {
	return (c >= '0' && c <= '9');
}

bool isLetter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool isOperator(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>');
}

bool isWhitespace(char c) {
	return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == -1);
}

bool isSubscript(char c) {
	return c == '[' || c == ']';
}
