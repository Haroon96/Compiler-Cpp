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

	/*
	// buffer for storing file contents
	std::string buffer;
	int buffer_length;

	char symbol;

	while (!file->eof()) {

		// buffer one line of input
		std::getline(*file, buffer);
		buffer_length = buffer.length();

		// i is used to iterate over the buffer
		int i = 0;
		while (i < buffer_length) {

			// next lexeme starts being parsed from here
			lexeme = "";

			// reset all machine states to 0
			memset(states, 0, totalAutomata * sizeof(int));

			// to indicate if next symbol is to be fed to all machines
			bool feed = true;

			// to store index of accepting machine
			int acceptingMachine;

			// continue feeding symbols until buffer runs out
			// or a machine accepts the string
			while (feed && i <= buffer_length) {

				// read next symbol from buffer
				symbol = buffer[i++];

				// feed symbol to all automata
				for (int j = 0; j < totalAutomata && feed; ++j) {
					// check if an automaton accepts the string
					if ((token = automata[j](symbol, states[j])) != INVALID) {
						// stop feeding symbols and set flag
						acceptingMachine = j;
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
				if (feed && isWhitespace(symbol) && lexeme != "") {
					feed = false;
					token = UNKNOWN;
				}

				// a comment was encountered
				// rest of the buffer is commented out so iterate over it
				if (feed && symbol == '#') {
					feed = false;
					i = buffer_length;
					token = IGNORE;
				}
			}

			// do not save ignored tokens (whitespaces)
			if (token != IGNORE) {
				tlPairs.push_back(new TokenLexeme(token, lexeme));

				// seek iterator backwards
				if (!isWhitespace(symbol) && i <= buffer_length) {
					--i;
				}
			}
		}
	}

	delete[]states;

	return tlPairs;
	*/
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
/*
std::vector<std::string> Lex::make_symbol_table(std::vector<TokenLexeme*> tlPairs) {

	int tlSize = tlPairs.size();
	std::vector<std::string> ids;

	// scan token-lexeme list
	for (int i = 0; i < tlSize; ++i) {

		// if token is an identifier
		if (tlPairs[i]->token == IDENTIFIER) {

			int idSize = ids.size();
			bool found = false;

			// search for same lexeme in symbol table
			for (int j = 0; j < idSize && !found; ++j) {

				// found a similar lexeme, change lexeme to index of existing
				if (ids[j] == tlPairs[i]->lexeme) {
					tlPairs[i]->lexeme = std::to_string(j);
					found = true;
				}
			}

			// if lexeme not found in table, add to it
			if (!found) {
				ids.push_back(tlPairs[i]->lexeme);
				tlPairs[i]->lexeme = std::to_string(idSize);
			}
		}
	}
	return ids;
}*/