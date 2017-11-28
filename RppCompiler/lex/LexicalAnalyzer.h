#pragma once

#include <string>
#include <vector>
#include <fstream>

struct TokenLexeme;
struct Symbol;
class SymbolTable;

class LexicalAnalyzer {
public:

	LexicalAnalyzer(std::ifstream *file, SymbolTable *symbolTable);

	TokenLexeme* nextToken();
	bool hasNextToken();

	std::ostringstream* getStream();
	int getLineNumber();
private:

	SymbolTable *symbolTable;
	std::ifstream *file;
	std::ostringstream *tlStream;
	int lineNumber;

	static const char *padding;
};