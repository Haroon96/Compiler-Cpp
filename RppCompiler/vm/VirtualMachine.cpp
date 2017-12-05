#include "VirtualMachine.h"
#include "../constants.h"
#include <iostream>

VirtualMachine::VirtualMachine(std::ifstream * file) {
	int ds_size;
	*file >> ds_size;
	ds = new int[ds_size];
	base_ptr = 0;
	stack_ptr = -1;

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
	int d, s, t, tmp;
	for (int ip = 0; ip != -1; ++ip) {
		d = instruction_memory[ip][1];
		s = instruction_memory[ip][2];
		t = instruction_memory[ip][3];
		
		switch (instruction_memory[ip][0]) {
		case IN:
			std::cin >> tmp;
			write_mem(d, tmp);
			break;
		case OUT:
			std::cout << read_mem(d) << std::endl;
			break;
		case ADD:
			write_mem(d, read_mem(s) + read_mem(t));
			break;
		case SET:
			write_mem(d, s);
			break;
		case LABEL:
			ip = s - 1;
			break;
		case SUB:
			write_mem(d, read_mem(s) - read_mem(t));
			break;
		case MUL:
			write_mem(d, read_mem(s) * read_mem(t));
			break;
		case DIV:
			write_mem(d, read_mem(s) / read_mem(t));
			break;
		case MOV:
			write_mem(d, read_mem(s));
			break;
		case IF_GE_GOTO:
			if (read_mem(d) >= read_mem(s)) {
				ip = t - 1;
			}
			break;
		case IF_LE_GOTO:
			if (read_mem(d) <= read_mem(s)) {
				ip = t - 1;
			}
			break;
		case IF_L_GOTO:
			if (read_mem(d) < read_mem(s)) {
				ip = t - 1;
			}
			break;
		case IF_G_GOTO:
			if (read_mem(d) > read_mem(s)) {
				ip = t - 1;
			}
			break;
		case IF_E_GOTO:
			if (read_mem(d) == read_mem(s)) {
				ip = t - 1;
			}
			break;
		case IF_NE_GOTO:
			if (read_mem(d) != read_mem(s)) {
				ip = t - 1;
			}
			break;
		case GOTO:
			ip = d - 1;
			break;
		case PARAM:
			push(read_mem(d));
			break;
		case MOV_TO_ARR:
			write_mem(d + read_mem(s), read_mem(t));
			break;
		case MOV_FROM_ARR:
			write_mem(d, read_mem(s + read_mem(t)));
			break;
		case ALLOC:
			while (d-- > 0) push();
			break;
		case RET:
			clear_stack_call();
			write_mem(-2, pop());
			ip = pop();
			base_ptr = pop();
			break;
		case CHAR_OUT:
			std::cout << (char)d << std::endl;
			break;
		case CALL:
			push(0); // for result
			push(base_ptr);
			push(ip);
			base_ptr = stack_ptr;
			ip = d;
			break;
		case CALL_CLEANUP:
			write_mem(d, pop());
			while (s-- > 0) pop();
			break;
		case CALL_CLEANUP_NO_RESULT:
			pop();
			while (d-- > 0) pop();
			break;
		case EXIT:
			return;
		}
	}
}

void VirtualMachine::push(int val) {
	stack.push_back(val);
	stack_ptr++;
}

int VirtualMachine::pop() {
	int top = stack[stack_ptr];
	stack.erase(stack.begin() + stack_ptr);
	stack_ptr--;
	return top;
}

void VirtualMachine::clear_stack_call() {
	while (stack_ptr > base_ptr + 1) {
		pop();
	}
}

int VirtualMachine::read_mem(int addr) {
	if (addr >= 0 && ((addr & GLOBAL) == GLOBAL)) {
		return ds[(addr & ~GLOBAL)];
	} else {
		return stack[base_ptr + addr];
	}
}

void VirtualMachine::write_mem(int addr, int val) {
	if (addr >= 0 && ((addr & GLOBAL) == GLOBAL)) {
		ds[(addr & ~GLOBAL)] = val;
	} else {
		stack[base_ptr + addr] = val;
	}
}
