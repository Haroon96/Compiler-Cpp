#include <iostream>
#include "lex/LexicalAnalyzer.h"
#include "syntax/SyntaxAnalyzer.h"
using namespace std;

bool verify(int argc, char *argv[], ifstream *&src) {
	if (argc < 2) {
		cerr << "Source file name not specified" << endl;
		return false;
	}

	src = new ifstream(argv[1]);

	if (!src->is_open()) {
		cerr << "Invalid source file name specified" << endl;
		return false;
	}

	return true;
}

int main(int argc, char *argv[]) {


	LexicalAnalyzer *lex = nullptr;
	SyntaxAnalyzer *syntax = nullptr;

	std::ostringstream err;

	ifstream *src;

	if (!verify(argc, argv, src)) {
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

	ofstream fout("tree.txt");
	fout << syntax->getStream()->str() << endl;
	cout << syntax->getStream()->str() << endl;	

	fout << err.str();
	cout << err.str();

	fout.close();

	src->close();

	delete lex;
	delete syntax;
	delete src;
	return 0;
}

