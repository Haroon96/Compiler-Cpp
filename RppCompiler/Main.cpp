#include <iostream>
#include "lex\LexicalAnalyzer.h"
#include "syntax\SyntaxAnalyzer.h"
#include "translator\Translator.h"
#include "vm\VirtualMachine.h"
#include "constants.h"
#include "SymbolTable.h"
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

std::string OpCodeReadable[] = {
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOV",
	"OUT",
	"IN",
	"IF_GE_GOTO",
	"IF_LE_GOTO",
	"IF_L_GOTO",
	"IF_G_GOTO",
	"IF_E_GOTO",
	"IF_NE_GOTO",
	"GOTO",
	"PARAM",
	"SET",
	"RET",
	"MOV_TO_ARR",
	"MOV_FROM_ARR",
	"LABEL",
	"CHAR_OUT",
	"CALL",
	"ALLOC",
	"UNUSED",
	"CALL_CLEANUP",
	"CALL_CLEANUP_NO_RESULT",
	"EXIT"
};

void print_friendly(std::string in_file, std::string out_file) {
	ifstream in(in_file);
	ofstream out(out_file);
	int op, d, s, t;
	in >> op;
	while (!in.eof()) {
		in >> op >> d >> s >> t;
		if (!in.eof()) {
			out << OpCodeReadable[op] << " ";
			out << d << " ";
			out << s << " ";
			out << t << endl;
		}
	}
	in.close();
	out.close();
}

int main(int argc, char *argv[]) {

	LexicalAnalyzer *lex = nullptr;
	SyntaxAnalyzer *syntax = nullptr;
	Translator *translator = nullptr;
	VirtualMachine *vm = nullptr;

	std::ostringstream err;
	ifstream *src;

	char* filename = argv[1];

	if (!verify(argc, filename, src)) {
		return 1;
	}

	std::string tree_out = filename + std::string(".tree.txt");
	std::string lex_out = filename + std::string(".lex.txt");
	std::string machine_code_out = filename + std::string(".mcode.txt");
	std::string friendly_code_out = filename + std::string(".fcode.txt");

	try {
		lex = new LexicalAnalyzer(src);
		translator = new Translator();
		syntax = new SyntaxAnalyzer(lex, translator);

		syntax->parse();

		ofstream tree(tree_out);
		ofstream tknlex(lex_out);
		ofstream mc(machine_code_out);

		tree << syntax->getStream()->str() << endl;
		tree << err.str() << std::endl;
		tknlex << lex->getStream()->str() << std::endl;
		mc << (*translator->getStream()) << std::endl;
		print_friendly(machine_code_out, friendly_code_out);

		tree.close();
		tknlex.close();
		mc.close();
		src->close();

		delete lex;
		delete syntax;
		delete src;

	} catch (exception &e) {
		err << "Error encountered on line " << lex->getLineNumber() << endl;
		err << "Details: " << e.what() << endl;
	}

	bool errorOccurred = (err.str().length() == 0);

	if (errorOccurred) {
		std::cout << "Compiled successfully. Running." << std::endl;
		vm = new VirtualMachine(new ifstream(machine_code_out));
		vm->execute();
	} else {
		std::cout << err.str() << std::endl;
	}


	delete vm;
	return 0;
}

