#include "Symbol.h"
#include "constants.h"

Symbol::Symbol(std::string name) {
	this->name = name;
	this->offset = -1;
	this->type = INT_VAR;
	this->length = 0;
}

void Symbol::setOffset(int offset) {
	this->offset = offset;
}	

void Symbol::setType(SymbolType type) {
	this->type = type;
}

std::string Symbol::getName() {
	return name;
}

int Symbol::getOffset() {
	return offset;
}

SymbolType Symbol::getType() {
	return type;
}

int Symbol::getLength() {
	return length;
}

void Symbol::setLength(int length) {
	this->length = length;
}