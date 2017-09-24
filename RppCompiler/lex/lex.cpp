#include <vector>
#include "lex.h"
#include "automata.h"
#include "util.h"

namespace lex {

	std::vector<TokenLexeme*> analyze(std::ifstream *file) {

		std::vector<TokenLexeme*> tlPairs;

		// pointers to automaton methods
		// sorted by descending priority
		int(*automata[])(char, int&) = {
			&if_stmt,
			&else_stmt,
			&while_stmt,
			&return_stmt,
			&print_stmt,
			&def_stmt,
			&get_stmt,
			&int_t,
			&char_t,
			&numeric_constants,
			&identifiers,
			&literal_constant,
			&relational_operators,
			&special_symbols
		};
	
		// store total number of automata
		int totalAutomata = sizeof(automata) / sizeof(automata[0]);

		// store states of all automata in this array
		int *states = new int[totalAutomata];

		// to store token and lexeme
		int token;
		std::string lexeme;

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
	}

	std::vector<std::string> make_symbol_table(std::vector<TokenLexeme*> tlPairs) {

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
	}

}