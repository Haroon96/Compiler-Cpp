#pragma once

#include <string>
#include <vector>
#include <fstream>

struct TokenLexeme;

class LexicalAnalyzer {
public:

	LexicalAnalyzer(std::ifstream *file);
	~LexicalAnalyzer();

	TokenLexeme* nextToken();
	bool hasNextToken();

	std::string getId(std::string index);
	std::vector<std::string>::iterator getIdTblStart();
	std::vector<std::string>::iterator getIdTblEnd();
	std::ostringstream* getStream();
	int getLineNumber();
private:

	std::vector<std::string> *idTbl;
	int findId(std::string id);
	std::string getIdLexeme(int index);
	std::ifstream *file;
	std::ostringstream *tlStream;
	int lineNumber;

	static const char *padding;
};