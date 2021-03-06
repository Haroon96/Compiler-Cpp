#pragma once

#include <vector>
#include <string>

struct Symbol;

class SymbolTable {
public:
	SymbolTable();
	~SymbolTable();
	int addSymbol(Symbol *symbol);
	Symbol* getSymbol(std::string name);
	int indexOf(std::string name);
	int nextOffset();
	void removeSymbol(std::string id);
	int getLength();
	void setMask(int mask);

private:
	std::vector<Symbol*> symbolTable;
	int offset;
	int mask;

};