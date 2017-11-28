#include <iostream>
#include "lex\LexicalAnalyzer.h"
#include "syntax\SyntaxAnalyzer.h"
#include "translator\Translator.h"
#include "vm\VirtualMachine.h"
#include "compiler.h"
using namespace std;

bool verify(int argc, char *filename, ifstream *&src) {
	if (argc < 2) {
		cerr << "Source file name not specified" << endl;
		return false;
	}

	src = new ifstream(filename);

	if (!src->is_open()) {
		cerr << "Invalid source file name specified" << endl;
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {

	SymbolTable *symbolTable = nullptr;
	LexicalAnalyzer *lex = nullptr;
	SyntaxAnalyzer *syntax = nullptr;
	Translator *translator = nullptr;
	VirtualMachine *vm = nullptr;

	std::ostringstream err;
	ifstream *src;

	// test code. remove from production.
	char* filename = "test.txt";
	argc = 2;
	// -------------------------
	
	//char* filename = argv[1];

	if (!verify(argc, filename, src)) {
		return 1;
	}

	try {
		symbolTable = new SymbolTable();
		lex = new LexicalAnalyzer(src, symbolTable);
		translator = new Translator(symbolTable);
		syntax = new SyntaxAnalyzer(lex, translator, symbolTable);

		syntax->parse();

		translator->finalize();


		ofstream tree(filename + std::string(".tree.txt"));
		ofstream tknlex(filename + std::string(".lex.txt"));
		ofstream tac(filename + std::string(".mcode.txt"));

		tree << syntax->getStream()->str() << endl;
		tree << err.str() << std::endl;
		tknlex << lex->getStream()->str() << std::endl;
		tac << (*translator->getStream()) << std::endl;

		tree.close();
		tknlex.close();
		src->close();


		vm = new VirtualMachine(new ifstream(filename + std::string(".mcode.txt")));
		vm->execute();

		delete lex;
		delete syntax;
		delete src;
		delete vm;

	} catch (exception &e) {
		err << "Error encountered on line " << lex->getLineNumber() << endl;
		err << "Details: " << e.what() << endl;
	}

	bool errorOccurred = (err.str().length() == 0);

	if (errorOccurred) {
		std::cout << "Compiled successfully." << std::endl;
	} else {
		std::cout << err.str() << std::endl;
	}


	return 0;
}

