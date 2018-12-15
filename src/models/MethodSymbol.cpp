#include "MethodSymbol.h"

void MethodSymbol::addParam(SymbolType pt) {
	params.push_back(pt);
}

std::vector<SymbolType> MethodSymbol::getParams() {
	return params;
}

int MethodSymbol::getLength() {
	return params.size();
}