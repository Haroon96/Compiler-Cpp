#include <iostream>
#include "lex\LexicalAnalyzer.h"
#include "syntax\SyntaxAnalyzer.h"
#include "translator\Translator.h"
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

	LexicalAnalyzer *lex = nullptr;
	SyntaxAnalyzer *syntax = nullptr;
	Translator *translator = nullptr;

	std::ostringstream err;

	char* filename = argv[1];

	ifstream *src;

	if (!verify(argc, filename, src)) {
		return 1;
	}
	
	try {
		lex = new LexicalAnalyzer(src);
		translator = new Translator();
		syntax = new SyntaxAnalyzer(lex, translator);

		syntax->parse();

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

	ofstream tree(filename + std::string(".tree.txt"));
	ofstream tknlex(filename + std::string(".lex.txt"));
	ofstream idTbl(filename + std::string(".lex_ids.txt"));
	ofstream tac(filename + std::string(".tac.txt"));

	tree << syntax->getStream()->str() << endl;
	tree << err.str() << std::endl;
	tknlex << lex->getStream()->str() << std::endl;
	tac << (*translator->getStream()) << std::endl;

	for (std::vector<Symbol*>::iterator i = lex->getSymbolTableStart(); i != lex->getSymbolTableEnd(); ++i) {
		idTbl << (*i)->getName() << " " << (*i)->getOffset() << " " << getSymbolType((*i)->getType()) << std::endl;
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

