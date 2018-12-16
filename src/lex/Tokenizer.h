#pragma once

#include <string>
#include <vector>
#include <fstream>

struct TokenLexeme;

class Tokenizer {
public:

	Tokenizer(std::ifstream *file);

	TokenLexeme* nextToken();
	bool hasNextToken();

	std::ostringstream* getStream();
	int getLineNumber();
private:
	std::ifstream *file;
	std::ostringstream *tlStream;
	int line_no;

	static const char *padding;
};