#include <vector>
#include "lex.h"
#include "automata.h"
#include "util.h"

namespace lex {


	// TODO: Code cleanup
	std::vector<TokenLexeme*> analyze(std::ifstream *file) {
		std::vector<TokenLexeme*> tlpairs;

		int(*machines[])(char, int&) = {
			&numeric_constants,
			&identifiers,
			&literal_constant,
			&relational_operators,
			&operators
		};

		int total_machines = sizeof(machines) / sizeof(machines[0]);

		int *states = new int[total_machines];
		
		int token;

		while (!file->eof()) {

			// reset all machine states to 0
			memset(states, 0, total_machines * sizeof(int));
			
			bool feed = true;
			int acceptingMachine;
			std::string lexeme = "";
			char symbol;

			while (feed) {

				// read next symbol
				symbol = file->get();

				// feed symbol to all machines
				for (int i = 0; i < total_machines && feed; ++i) {
					
					if ((token = machines[i](symbol, states[i])) != INVALID) {
						// a high-priority machine reached an accepting state
						// suspend and save token-lexeme pair
						feed = false;
						acceptingMachine = i;
					}
				}

				if (feed) {
					lexeme += symbol;
				}

				if (feed && (isWhitespace(symbol) || file->eof()) && lexeme != "") {
					feed = false;
					token = UNKNOWN;
				}

			}

			// tokens with IGNORE value are ignored
			if (token != IGNORE) {
				tlpairs.push_back(new TokenLexeme(token, lexeme));
				// seek backwards
				if (!file->eof() && !isWhitespace(symbol)) {
					file->seekg(-1, std::ios_base::cur);
				}
			}

		}

		delete[]states;
		
		return tlpairs;
	}
}