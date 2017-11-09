#include "Translator.h"
#include <string>

Translator::Translator() {
	tac = new std::string();
	patch_marks = new std::stack<int>();
	stack = new std::stack<std::string>();
	lineNumber = 0;
	tmp_index = 0;
	offset = 0;
	newline();
}

Translator::~Translator() {
	delete tac;
	delete patch_marks;
}

int Translator::getLineNumber() {
	return lineNumber;
}

void Translator::newline() {
	++lineNumber;
	tac->append("\n");
	tac->append("L");
	tac->append(std::to_string(lineNumber));
	tac->append(":\t");
}

void Translator::write_label(std::string s) {
	tac->append(s);
}

void Translator::write(std::string s) {
	tac->append(s);
	tac->append(" ");
}

void Translator::write(int n) {
	write(std::to_string(n));
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

std::string Translator::get_temp_var() {
	++offset;
	return "tmp" + std::to_string((++tmp_index));
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

int Translator::getOffset() {
	return (offset++) * 4;
}

std::string * Translator::getStream() {
	return tac;
}
