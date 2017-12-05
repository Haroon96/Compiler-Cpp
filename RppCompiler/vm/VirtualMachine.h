#pragma once
#include <vector>
#include <fstream>

class Instruction;

class VirtualMachine {
	
public:
	VirtualMachine(std::ifstream* file);
	~VirtualMachine();

	void execute();

private:
	std::vector<int*> instruction_memory;
	std::vector<int> stack;
	void push(int val = 0);
	int pop();
	void clear_stack_call();
	int read_mem(int addr);
	void write_mem(int addr, int val);

	int *ds;

	int base_ptr;
	int stack_ptr;

};