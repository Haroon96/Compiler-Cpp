#pragma once

#include "Symbol.h"
#include "constants.h"

struct MethodSymbol : Symbol {
public:
	MethodSymbol(std::string lexeme) :Symbol(lexeme) {}
	void addParam(ParameterType pt);
	int getLength();
private:
	std::vector<ParameterType> params;
};