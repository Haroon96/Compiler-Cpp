#include <iostream>
#include "lex/LexicalAnalyzer.h"
#include "syntax/SyntaxAnalyzer.h"
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

	// test code
	// specifies file input
	// ======================
	argv = new char*[2]{ {nullptr}, {"input.txt"} };
	argc = 2;
	// ======================

	LexicalAnalyzer *lex = nullptr;
	SyntaxAnalyzer *syntax = nullptr;

	std::ostringstream err;

	char* filename = argv[1];

	ifstream *src;

	if (!verify(argc, filename, src)) {
		return 1;
	}
	
	try {
		lex = new LexicalAnalyzer(src);
		syntax = new SyntaxAnalyzer(lex);

		syntax->parse();

	} catch (exception &e) {
		err << "Error encountered on line " << lex->getLineNumber() << endl;
		err << "Details: " << e.what() << endl;
	}

	if (err.str().length() == 0) {
		std::cout << "Compiled successfully." << std::endl;
	} else {
		std::cout << err.str() << std::endl;
	}
	
	ofstream tree(filename + std::string(".tree.txt"));
	ofstream tknlex(filename + std::string(".lex.txt"));
	ofstream idTbl(filename + std::string(".lex_ids.txt"));

	tree << syntax->getStream()->str() << endl;
	tree << err.str() << std::endl;
	tknlex << lex->getStream()->str() << std::endl;

	for (std::vector<std::string>::iterator i = lex->getIdTblStart(); i != lex->getIdTblEnd(); ++i) {
		idTbl << (*i) << std::endl;
	}

	tree.close();
	tknlex.close();
	idTbl.close();

	src->close();

	delete lex;
	delete syntax;
	delete src;

	return 0;
}

