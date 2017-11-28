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
	int *ds;

};