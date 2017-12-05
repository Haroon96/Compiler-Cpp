#pragma once

#include <string>
#include <sstream>
#include <stack>
#include "../constants.h"

class SymbolTable;

class Translator {
public:
	Translator();
	~Translator();

	int get_instruction_count();

	void write_label(std::string s);
	void write(std::string s);
	void write(int n);
	void write_instruction(int op, int dst = UNUSED, int op1 = UNUSED, int op2 = UNUSED);
	void mark_patch();
	void patch(std::string s);
	void patch(int s);
	void reset_temp_vars();
	void reset_temp_index(int index = 0);
	void next_instruction();
	void finalize(SymbolTable* globalSymbolTable);
	std::string get_temp_var(SymbolType type, SymbolTable *curr_scope);
	std::string push(std::string s);
	std::string pop();
	
	std::string* getStream();

private:
	std::string *tac;
	std::stack<int> *patch_marks;
	std::stack<std::string> *stack;
	int instruction_count;
	int tmp_index;
	int max_tmp_index;
};