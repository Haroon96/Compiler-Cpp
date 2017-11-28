#include "Translator.h"
#include "../compiler.h"
#include <string>

Translator::Translator(SymbolTable *symbolTable) {
	this->symbolTable = symbolTable; 
	tac = new std::string();
	patch_marks = new std::stack<int>();
	stack = new std::stack<std::string>();
	instruction_count = 0;
	tmp_index = 0;
	max_tmp_index = -1;
}

Translator::~Translator() {
	delete tac;
	delete patch_marks;
}

int Translator::get_instruction_count() {
	return instruction_count;
}

void Translator::write_label(std::string s) {
	write(LABEL);
	write(s);
	write(UNUSED);
	write(UNUSED);
}

void Translator::write(std::string s) {
	tac->append(s);
	tac->append(" ");
}

void Translator::write(int n) {
	write(std::to_string(n));
}

void Translator::write_instruction(OpCode op, int dst, int op1, int op2) {
	write(op);
	write(dst);
	write(op1);
	write(op2);
}

void Translator::mark_patch() {
	int index = tac->length();
	patch_marks->push(index);
	tac->append(" ");
}

void Translator::patch(std::string s) {
	int index = patch_marks->top();

	tac->insert(index, s);

	patch_marks->pop();
}

void Translator::patch(int s) {
	patch(std::to_string(s));
}

void Translator::reset_temp_index(int index) {
	tmp_index = index;
}

void Translator::next_instruction() {
	instruction_count++;
	tac->append("\n");
}

std::string Translator::get_temp_var(SymbolType type) {
	int index;
	if (tmp_index > max_tmp_index) {
		max_tmp_index = tmp_index;

		// add new temp variable to symbol table
		Symbol *symbol = new Symbol("tmp" + std::to_string(tmp_index));
		index = symbolTable->addSymbol(symbol);
		symbol->setOffset(symbolTable->nextOffset());
		symbol->setLength(1);
		symbol->setType(type);
	} else {
		index = symbolTable->indexOf("tmp" + std::to_string(tmp_index));
	}
	++tmp_index;
	return std::to_string(index);
}

std::string Translator::push(std::string s) {
	stack->push(s);
	return s;
}

std::string Translator::pop() {
	std::string tmp = stack->top();
	stack->pop();
	return tmp;
}

std::string * Translator::getStream() {
	return tac;
}

void Translator::finalize() {
	tac->insert(0, "\n");
	tac->insert(0, std::to_string(symbolTable->getLength()));
}