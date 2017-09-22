#include <iostream>
#include "lex/lex.h"
using namespace std;

int main(int argc, char *argv[]) {
	/*
	if (argc < 2) {
		cerr << "Source file name not specified" << endl;
		return 1;
	}

	ifstream src(argv[1]);*/

	ifstream src("test.txt");

	if (!src.is_open()) {
		cerr << "Invalid source file name specified" << endl;
		return 1;
	}

	std::vector<lex::TokenLexeme*> tlp = lex::analyze(&src);

	for (int i = 0; i < tlp.size(); ++i) {
		cout << tlp[i]->token << " " << tlp[i]->lexeme << endl;
	}

	src.close();
	return 0;
}