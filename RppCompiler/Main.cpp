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

	std::ostringstream err;

	try {
		
		//verify(argc, argv);
		src = new ifstream("in.txt");
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

