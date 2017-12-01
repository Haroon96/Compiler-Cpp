#pragma once

#include <vector>

struct Symbol;

class SymbolTable {
public:
	SymbolTable();
	int addSymbol(Symbol *symbol);
	Symbol* getSymbol(std::string id);
	Symbol* getSymbol(int id);
	int indexOf(std::string name);
	int nextOffset();
	void removeSymbol(std::string id);
	int getLength();

private:
	std::vector<Symbol*> *symbolTable;
	int offset;

};