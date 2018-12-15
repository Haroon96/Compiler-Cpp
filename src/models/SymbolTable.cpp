#include "SymbolTable.h"
#include "Symbol.h"
#include "../constants.h"

SymbolTable::SymbolTable() {
	mask = 0;
	offset = 1;
}

SymbolTable::~SymbolTable() {
	int size = symbolTable.size();
	for (int i = 0; i < size; ++i) {
		delete symbolTable[i];
	}
	symbolTable.clear();
}

int SymbolTable::addSymbol(Symbol *symbol) {
	symbolTable.push_back(symbol);
	return symbolTable.size() - 1;
}

Symbol* SymbolTable::getSymbol(std::string name) {
	int index = indexOf(name);
	return index == -1 ? nullptr : symbolTable[index];
}

int SymbolTable::indexOf(std::string name) {
	unsigned int size = symbolTable.size();
	for (unsigned int i = 0; i < size; ++i) {
		if (symbolTable[i]->getName() == name) {
			return i;
		}
	}
	return -1;
}

int SymbolTable::nextOffset() {
	int size = symbolTable.size();
	if (size > 1) {
		int i = 2;
		while (size - i >= 0 && symbolTable[size - i]->getType() == METHOD) i--;
		if (size - i >= 0 && symbolTable[size - i]->getOffset() >= 0) {
			offset += symbolTable[size - i]->getLength();
		}
	}
	return offset | mask;
}

void SymbolTable::removeSymbol(std::string id) {
	symbolTable.erase(symbolTable.begin() + std::stoi(id));
}

int SymbolTable::getLength() {
	unsigned int size = symbolTable.size();
	int length = 0;
	for (unsigned int i = 0; i < size; ++i) {
		if (symbolTable[i]->getType() != METHOD) {
			length += symbolTable[i]->getLength();
		}
	}
	return length;
}

void SymbolTable::setMask(int mask) {
	this->mask = mask;
}
