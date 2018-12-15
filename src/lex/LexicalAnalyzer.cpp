#include "LexicalAnalyzer.h"
#include "automata.h"
#include "util.h"
#include "../constants.h"
#include "../models/TokenLexeme.h"
#include <sstream>
#include <cstring>
#include <string>

const char *LexicalAnalyzer::padding = "                              ";

LexicalAnalyzer::LexicalAnalyzer(std::ifstream * file) {
	this->file = file;
	this->line_no = 1;
	this->tlStream = new std::ostringstream();
}

TokenLexeme* LexicalAnalyzer::nextToken() {

	// pointers to automaton methods
	// sorted by descending priority
	Token(*automata[])(char, int&) = {
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
	Token token = INVALID;
	std::string lexeme;

	TokenLexeme *tokenLexeme = nullptr;

	// while not eof and a valid token isn't found
	while (!file->eof() && tokenLexeme == nullptr) {

		lexeme = "";

		// reset all machine states to 0
		memset(states, 0, totalAutomata * sizeof(int));

		// to indicate if next symbol is to be fed to all machines
		bool feed = true;
		char symbol;

		while (feed) {
			// read next symbol
			symbol = file->get();

			// update line number
			if (symbol == '\n') {
				line_no++;
			}

			// feed symbol to all automata
			for (int i = 0; i < totalAutomata && feed; ++i) {

				// check if an automaton accepts the string
				if ((token = automata[i](symbol, states[i])) != INVALID) {
					// stop feeding symbols and set flag
					feed = false;
				}
			}

			// encountered a whitespace. ignore it and reset.
			if (lexeme == "" && isWhitespace(symbol)) {
				feed = false;
				token = IGNORE;
			}

			// lexeme was not accepted by any machine yet
			// append symbol to lexeme and continue feeding
			if (feed) {
				lexeme += symbol;
			}

			// an unrecognized lexeme was encountered
			// mark it as unknown and reset
			if (feed && (isWhitespace(symbol) || file->eof()) && lexeme != "") {
				feed = false;
				token = UNKNOWN;
			}

			// a comment was encountered
			// rest of the line is commented out so skip it
			if (feed && symbol == '#') {
				feed = false;
				std::string dump;
				std::getline(*file, dump);
				token = IGNORE;
			}

		}

		// do not save ignored tokens (whitespaces)
		if (token != IGNORE) {
			if (!file->eof() && !isWhitespace(symbol)) {
				// seek backwards
				file->seekg(-1, std::ios_base::cur);
			}

			// set token-lexeme variable to end loop
			tokenLexeme = new TokenLexeme(token, lexeme);
		}

	}
	if (tokenLexeme != nullptr) {
		std::string tokenName = getTokenName(tokenLexeme->getToken());
		*tlStream << tokenName;
		*tlStream << (padding + tokenName.length());
		*tlStream << tokenLexeme->getLexeme() << std::endl;
	}
	return tokenLexeme;
}

bool LexicalAnalyzer::hasNextToken() {
	return file->peek() != EOF;
}
std::ostringstream * LexicalAnalyzer::getStream() {
	return tlStream;
}

int LexicalAnalyzer::getLineNumber() {
	return line_no;
}