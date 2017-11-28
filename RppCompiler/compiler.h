#pragma once

#include <vector>
#include <string>

enum Token {
	INVALID = 300,
	NUMERIC_CONSTANT,
	IDENTIFIER,
	RELATIONAL_OPERATOR,
	UNKNOWN,
	LITERAL_CONSTANT,
	ADDITION_OPERATOR,
	SUBTRACTION_OPERATOR,
	MULTIPLICATION_OPERATOR,
	DIVISION_OPERATOR,
	ASSIGNMENT_OPERATOR,
	IF_STATEMENT,
	ELSE_STATEMENT,
	WHILE_STATEMENT,
	RETURN_STATEMENT,
	PRINT_STATEMENT,
	GET_STATEMENT,
	DEF_STATEMENT,
	INT_TYPE,
	CHAR_TYPE,
	IGNORE,
	L_BRACE,
	R_BRACE,
	L_SUBSCRIPT_OPERATOR,
	R_SUBSCRIPT_OPERATOR,
	COMMA,
	L_PARENTHESES,
	R_PARENTHESES
};

enum SymbolType {
	METHOD,
	INT_VAR,
	CHAR_VAR
};

enum OpCode {
	ADD,
	SUB,
	MUL,
	DIV,
	MOV,
	OUT,
	IN,
	IF_GE_GOTO,
	IF_LE_GOTO,
	IF_L_GOTO,
	IF_G_GOTO,
	IF_E_GOTO,
	IF_NE_GOTO,
	GOTO,
	PARAM,
	SET,
	RET,
	MOV_TO_ARR,
	MOV_FROM_ARR,
	ARR_IN,
	LABEL,
	CHAR_OUT,
	UNUSED = -1
};

enum AddressMode {
	DIRECT,
	INDIRECT
};

std::string getTokenName(Token t);
std::string getSymbolType(SymbolType t);

struct TokenLexeme {
	TokenLexeme(Token token, std::string lexeme) : token(token), lexeme(lexeme) {}
	Token getToken() {
		if (this == nullptr) {
			throw std::exception("Unexpected end of file");
		}
		return token;
	}
	std::string getLexeme() {
		if (this == nullptr) {
			throw std::exception("Unexpected end of file");
		}
		return lexeme;
	}
private:
	Token token;
	std::string lexeme;
};

struct Symbol {
	Symbol(std::string name) :name(name), offset(-1), type(INT_VAR), length(0) {
	}
	void setOffset(int offset) {
		this->offset = offset;
	}
	void setType(SymbolType type) {
		this->type = type;
	}
	std::string getName() {
		return name;
	}
	int getOffset() {
		return offset;
	}
	SymbolType getType() {
		return type;
	}
	int getLength() {
		return length;
	}
	void setLength(int length) {
		this->length = length;
	}
private:
	std::string name;
	int offset;
	int length;
	SymbolType type;
};

class SymbolTable {

private:
	std::vector<Symbol*> *symbolTable;
	int offset;
public:

	SymbolTable() {
		symbolTable = new std::vector<Symbol*>();
		offset = 0;
	}

	int addSymbol(Symbol *symbol) {
		symbolTable->push_back(symbol);
		return symbolTable->size() - 1;
	}

	Symbol* getSymbol(std::string id) {
		return getSymbol(std::stoi(id));
	}

	Symbol* getSymbol(int id) {
		return (*symbolTable)[id];
	}

	int indexOf(std::string name) {
		unsigned int size = symbolTable->size();
		for (unsigned int i = 0; i < size; ++i) {
			if ((*symbolTable)[i]->getName() == name) {
				return i;
			}
		}
		return -1;
	}

	int nextOffset() {
		int size = symbolTable->size();
		if (size > 1) {
			offset += (*symbolTable)[size - 2]->getLength();
		}
		return offset;
	}

	void removeSymbol(std::string id) {
		symbolTable->erase(symbolTable->begin() + std::stoi(id));
	}

	int getLength() {
		unsigned int size = symbolTable->size();
		int length = 0;
		for (unsigned int i = 0; i < size; ++i) {
			length += (*symbolTable)[i]->getLength();
		}
		return length;
	}

};