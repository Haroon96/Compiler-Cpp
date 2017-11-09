#pragma once

#include <string>
#include <sstream>
#include <stack>

class Translator {
public:
	Translator();
	~Translator();

	int getLineNumber();

	void newline();
	void write_label(std::string s);
	void write(std::string s);
	void write(int n);
	void mark_patch();
	void patch(std::string s);
	void patch(int s);
	std::string get_temp_var();
	std::string push(std::string s);
	std::string pop();
	int getOffset();
	
	std::string* getStream();

private:
	std::string *tac;
	std::stack<int> *patch_marks;
	std::stack<std::string> *stack;
	int lineNumber;
	int tmp_index;
	int offset;
};