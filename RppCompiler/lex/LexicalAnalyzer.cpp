#include "LexicalAnalyzer.h"
#include "automata.h"
#include "util.h"
#include "../compiler.h"
#include <sstream>

const char *LexicalAnalyzer::padding = "                              ";

LexicalAnalyzer::LexicalAnalyzer(std::ifstream * file) {
	this->file = file;
	this->symbolTable = new std::vector<Symbol*>();
	this->lineNumber = 1;
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
				lineNumber++;
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
				std::getline(*file, std::string());
				token = IGNORE;
			}

		}

		// do not save ignored tokens (whitespaces)
		if (token != IGNORE) {
			if (!file->eof() && !isWhitespace(symbol)) {
				// seek backwards
				file->seekg(-1, std::ios_base::cur);
			}

			// if lexeme is an identifier
			if (token == IDENTIFIER) {
				// locate in symbol table
				int index = findId(lexeme);

				// if found, reference it
				if (index >= 0) {
					lexeme = std::to_string(index);
				} else {
					// else add it to symbol table
					symbolTable->push_back(new Symbol(lexeme));
					lexeme = std::to_string(symbolTable->size() - 1);
				}
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
LexicalAnalyzer::~LexicalAnalyzer() {
	symbolTable->clear();
	delete symbolTable;
}
std::string LexicalAnalyzer::getId(std::string index) {
	return (*symbolTable)[std::stoi(index)]->getName();
}
std::vector<Symbol*>::iterator LexicalAnalyzer::getSymbolTableStart() {
	return symbolTable->begin();
}
std::vector<Symbol*>::iterator LexicalAnalyzer::getSymbolTableEnd() {
	return symbolTable->end();
}
Symbol * LexicalAnalyzer::getSymbol(std::string id) {
	return (*symbolTable)[std::stoi(id)];
}
std::ostringstream* LexicalAnalyzer::getStream() {
	return tlStream;
}
int LexicalAnalyzer::getLineNumber() {
	return lineNumber;
}
int LexicalAnalyzer::findId(std::string id) {
	int size = symbolTable->size();
	for (int i = 0; i < size; ++i) {
		if ((*symbolTable)[i]->getName() == id) {
			return i;
		}
	}
	return -1;
}