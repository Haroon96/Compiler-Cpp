#pragma once

#include "../constants.h"
#include <string>

struct Symbol {
public:
	Symbol(std::string name);
	void setOffset(int offset);
	void setType(SymbolType type);
	std::string getName();
	int getOffset();
	SymbolType getType();
	virtual int getLength();
	void setLength(int length);

private:
	std::string name;
	int offset;
	int length;
	SymbolType type;

};
