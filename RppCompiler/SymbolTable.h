#pragma once

#include <vector>

struct Symbol;

class SymbolTable {
public:
	SymbolTable();
	int addSymbol(Symbol *symbol);
	Symbol* getSymbol(std::string name);
	int indexOf(std::string name);
	int nextOffset();
	void removeSymbol(std::string id);
	int getLength();
	void setMask(int mask);

private:
	std::vector<Symbol*> *symbolTable;
	int offset;
	int mask;

};