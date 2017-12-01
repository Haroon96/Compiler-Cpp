#include "SymbolTable.h"
#include "Symbol.h"

SymbolTable::SymbolTable() {
	symbolTable = new std::vector<Symbol*>();
	offset = 0;
}

int SymbolTable::addSymbol(Symbol *symbol) {
	symbolTable->push_back(symbol);
	return symbolTable->size() - 1;
}

Symbol* SymbolTable::getSymbol(std::string id) {
	return getSymbol(std::stoi(id));
}

Symbol* SymbolTable::getSymbol(int id) {
	return (*symbolTable)[id];
}

int SymbolTable::indexOf(std::string name) {
	unsigned int size = symbolTable->size();
	for (unsigned int i = 0; i < size; ++i) {
		if ((*symbolTable)[i]->getName() == name) {
			return i;
		}
	}
	return -1;
}

int SymbolTable::nextOffset() {
	int size = symbolTable->size();
	if (size > 1) {
		offset += (*symbolTable)[size - 2]->getLength();
	}
	return offset;
}

void SymbolTable::removeSymbol(std::string id) {
	symbolTable->erase(symbolTable->begin() + std::stoi(id));
}

int SymbolTable::getLength() {
	unsigned int size = symbolTable->size();
	int length = 0;
	for (unsigned int i = 0; i < size; ++i) {
		length += (*symbolTable)[i]->getLength();
	}
	return length;
}