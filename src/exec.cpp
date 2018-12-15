#include <iostream>
#include <fstream>
#include "constants.h"
#include "models\SymbolTable.h"
#include "vm\VirtualMachine.h"

bool parse_args(int argc, char *args[], std::ifstream &src);
void print_help();

int main(int argc, char *argv[]) {

	std::ifstream code;

	if (parse_args(argc, argv, code)) {
		VirtualMachine(&code).execute();
		return 0;
	}

	return 1;
}

bool parse_args(int argc, char *args[], std::ifstream &src) {

	char *filename;
	
	if (argc < 2) {
		print_help();
		return false;
	} else {
		filename = args[1];
	}
	
	src.open(filename);
	
	if (!src.is_open()) {
		std::cerr << "Invalid file name specified" << std::endl;
		return false;
	}
	
	return true;
}

void print_help() {
	std::cout << "USAGE: exec filename" << std::endl;
}
