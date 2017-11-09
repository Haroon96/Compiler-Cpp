#pragma once

#include <string>
#include <vector>
#include <fstream>

struct TokenLexeme;
struct Symbol;

class LexicalAnalyzer {
public:

	LexicalAnalyzer(std::ifstream *file);
	~LexicalAnalyzer();

	TokenLexeme* nextToken();
	bool hasNextToken();

	std::string getId(std::string index);
	int findId(std::string id);
	std::vector<Symbol*>::iterator getSymbolTableStart();
	std::vector<Symbol*>::iterator getSymbolTableEnd();
	Symbol* getSymbol(std::string id);
	std::ostringstream* getStream();
	int getLineNumber();
private:

	std::vector<Symbol*> *symbolTable;
	std::ifstream *file;
	std::ostringstream *tlStream;
	int lineNumber;

	static const char *padding;
};