#pragma once

#include "Symbol.h"
#include "constants.h"

struct MethodSymbol : Symbol {
public:
	MethodSymbol(std::string lexeme) :Symbol(lexeme) {
		setType(METHOD);
	}
	void addParam(SymbolType pt);
	std::vector<SymbolType> getParams();
	int getLength();
private:
	std::vector<SymbolType> params;
};