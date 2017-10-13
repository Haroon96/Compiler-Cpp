#include <iostream>
#include "lex/LexicalAnalyzer.h"
#include "syntax/SyntaxAnalyzer.h"
using namespace std;

ifstream* verify(int argc, char *argv[]) {
	if (argc < 2) {
		throw exception("Source file name not specified");
	}

	ifstream *src = new ifstream(argv[1]);

	if (!src->is_open()) {
		throw exception("Invalid source file name specified");
	}

	return src;
}

int main(int argc, char *argv[]) {

	ifstream *src;

	LexicalAnalyzer *lex = nullptr;
	SyntaxAnalyzer *syntax = nullptr;

	try {
		
		//verify(argc, argv);
		src = new ifstream("in.txt");
		lex = new LexicalAnalyzer(src);
		syntax = new SyntaxAnalyzer(lex);

		syntax->parse();

		cout << syntax->getStream()->str() << endl;

	} catch (exception &e) {
		cerr << "Error encountered on line " << lex->getLineNumber() << endl;
		cerr << "Details: " << e.what() << endl;
	}

	src->close();

	delete lex;
	delete syntax;
	delete src;
	return 0;
}

