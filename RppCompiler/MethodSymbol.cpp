#include "MethodSymbol.h"

void MethodSymbol::addParam(ParameterType pt) {
	params.push_back(pt);
}

int MethodSymbol::getLength() {
	return params.size();
}