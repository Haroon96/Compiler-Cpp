#include <vector>
#include "LexicalAnalyzer.h"
#include "automata.h"
#include "util.h"

LexicalAnalyzer::LexicalAnalyzer(std::ifstream * file) {
	this->file = file;
	this->idTbl = new std::vector<std::string>();
}

TokenLexeme* LexicalAnalyzer::nextToken() {

	// pointers to automaton methods
	// sorted by descending priority
	int(*automata[])(char, int&) = {
		&automata::if_stmt,
		&automata::else_stmt,
		&automata::while_stmt,
		&automata::return_stmt,
		&automata::print_stmt,
		&automata::def_stmt,
		&automata::get_stmt,
		&automata::int_t,
		&automata::char_t,
		&automata::numeric_constants,
		&automata::identifiers,
		&automata::literal_constant,
		&automata::relational_operators,
		&automata::special_symbols
	};

	// store total number of automata
	int totalAutomata = sizeof(automata) / sizeof(automata[0]);

	// store states of all automata in this array
	int *states = new int[totalAutomata];

	// to store token and lexeme
	int token = INVALID;
	std::string lexeme = "";

	TokenLexeme *tokenLexeme = nullptr;

	while (!file->eof() && tokenLexeme == nullptr) {

		// reset all machine states to 0
		memset(states, 0, totalAutomata * sizeof(int));

		bool feed = true;
		char symbol;

		while (feed) {

			// read next symbol
			symbol = file->get();

			if (symbol == '#') {
				// discard comment by reading till newline
				std::getline(*file, std::string());
				symbol = file->get();
			}

			// feed symbol to all machines
			for (int i = 0; i < totalAutomata && feed; ++i) {

				if ((token = automata[i](symbol, states[i])) != INVALID) {
					// a high-priority machine reached an accepting state
					// suspend and save token-lexeme pair
					feed = false;
				}
			}

			if (lexeme == "" && isWhitespace(symbol)) {
				feed = false;
				token = IGNORE;
			}

			if (feed) {
				lexeme += symbol;
			}

			if (feed && (isWhitespace(symbol) || file->eof()) && lexeme != "") {
				feed = false;
				token = UNKNOWN;
			}
			
			if (feed && symbol == '#') {
				feed = false;
				std::getline(*file, std::string());
				token = IGNORE;
			}

		}

		// tokens with IGNORE value are ignored
		if (token != IGNORE) {
			if (!file->eof() && !isWhitespace(symbol)) {
				// seek backwards
				file->seekg(-1, std::ios_base::cur);
			}
			if (token == IDENTIFIER) {
				int index = findId(lexeme);
				if (index >= 0) {
					lexeme = std::to_string(index);
				} else {
					idTbl->push_back(lexeme);
					lexeme = std::to_string(idTbl->size() - 1);
				}
			}
			tokenLexeme = new TokenLexeme(token, lexeme);
		}

	}
	return tokenLexeme;
}

bool LexicalAnalyzer::hasNextToken() {
	return file->peek() != EOF;
}
LexicalAnalyzer::~LexicalAnalyzer() {
	delete idTbl;
}
std::string LexicalAnalyzer::getId(std::string index) {
	return (*idTbl)[std::stoi(index)];
}
std::vector<std::string>::iterator LexicalAnalyzer::getIdTblStart() {
	return idTbl->begin();
}
std::vector<std::string>::iterator LexicalAnalyzer::getIdTblEnd() {
	return idTbl->end();
}
int LexicalAnalyzer::findId(std::string id) {
	int size = idTbl->size();
	for (int i = 0; i < size; ++i) {
		if ((*idTbl)[i] == id) {
			return i;
		}
	}
	return -1;
}