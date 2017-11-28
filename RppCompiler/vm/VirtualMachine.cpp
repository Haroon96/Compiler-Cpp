#include "VirtualMachine.h"
#include "../compiler.h"
#include <iostream>

VirtualMachine::VirtualMachine(std::ifstream * file) {
	int ds_size;
	*file >> ds_size;
	ds = new int[ds_size];

	while (!file->eof()) {
		int *instruction = new int[4];
		*file >> instruction[0];
		*file >> instruction[1];
		*file >> instruction[2];
		*file >> instruction[3];
		instruction_memory.push_back(instruction);
	}

}

VirtualMachine::~VirtualMachine() {
	unsigned int size = instruction_memory.size();
	for (unsigned int i = 0; i < size; ++i) {
		delete instruction_memory[i];
	}
	instruction_memory.clear();
}

void VirtualMachine::execute() {
	int dst, op1, op2;
	for (int ip = 0; ; ++ip) {
		dst = instruction_memory[ip][1];
		op1 = instruction_memory[ip][2];
		op2 = instruction_memory[ip][3];
		
		switch (instruction_memory[ip][0]) {
		case IN:
			std::cin >> ds[dst];
			break;
		case OUT:
			std::cout << ds[dst] << std::endl;
			break;
		case ADD:
			ds[dst] = ds[op1] + ds[op2];
			break;
		case SET:
			ds[dst] = op1;
			break;
		case LABEL:
			break;
		case SUB:
			ds[dst] = ds[op1] - ds[op2];
			break;
		case MUL:
			ds[dst] = ds[op1] * ds[op2];
			break;
		case DIV:
			ds[dst] = ds[op1] / ds[op2];
			break;
		case MOV:
			ds[dst] = ds[op1];
			break;
		case IF_GE_GOTO:
			if (ds[dst] >= ds[op1]) {
				ip = op2 - 1;
			}
			break;
		case IF_LE_GOTO:
			if (ds[dst] <= ds[op1]) {
				ip = op2 - 1;
			}
			break;
		case IF_L_GOTO:
			if (ds[dst] < ds[op1]) {
				ip = op2 - 1;
			}
			break;
		case IF_G_GOTO:
			if (ds[dst] > ds[op1]) {
				ip = op2 - 1;
			}
			break;
		case IF_E_GOTO:
			if (ds[dst] == ds[op1]) {
				ip = op2 - 1;
			}
			break;
		case IF_NE_GOTO:
			if (ds[dst] != ds[op1]) {
				ip = op2 - 1;
			}
			break;
		case GOTO:
			ip = dst - 1;
			break;
		case PARAM:
			break;
		case MOV_TO_ARR:
			ds[dst + ds[op1]] = ds[op2];
			break;
		case MOV_FROM_ARR:
			ds[dst] = ds[op1 + ds[op2]];
			break;
		case RET:
			return;
		}

	}
}
