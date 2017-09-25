#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "../compiler.h"

class LexicalAnalyzer {
public:

	LexicalAnalyzer(std::ifstream *file);
	~LexicalAnalyzer();

	TokenLexeme* nextToken();
	bool hasNextToken();

	std::string getId(std::string index);
	std::vector<std::string>::iterator getIdTblStart();
	std::vector<std::string>::iterator getIdTblEnd();

private:

	std::vector<std::string> *idTbl;
	int findId(std::string id);
	std::ifstream *file;

};