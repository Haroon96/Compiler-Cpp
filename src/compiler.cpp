#include <iostream>
#include "lex\Tokenizer.h"
#include "parser\Parser.h"
#include "translator\Translator.h"
#include "constants.h"
#include "models\SymbolTable.h"

void print_help();
bool parse_args(int argc, char *args[], std::ifstream &src, std::string &filename, bool &verbose_flag);
void verbose_print(std::string src, std::string out, Tokenizer* lex, Parser* parser);
void print_friendly(std::ifstream in, std::ifstream out);

int main(int argc, char *argv[]) {

	Tokenizer *lex = nullptr;
	Parser *parser = nullptr;
	Translator *translator = nullptr;

	std::ifstream src;

	std::string filename;
	bool verbose;

	if (!parse_args(argc, argv, src, filename, verbose)) {
		return 1;
	}

	try {
		lex = new Tokenizer(&src);
		translator = new Translator();
		parser = new Parser(lex, translator);

		parser->parse();

		std::string out = filename + std::string(".out");

		// write out code
		std::ofstream mc(out);
		mc << (*translator->getStream()) << std::endl;	
		mc.close();
		
		src.close();

		if (verbose) {
			verbose_print(filename, out, lex, parser);
		}

		delete lex;
		delete translator;
		delete parser;

	} catch (std::runtime_error &e) {
		std::cerr << "Error encountered on line " << lex->getLineNumber() << std::endl;
		std::cerr << "Details: " << e.what() << std::endl;
	}

	return 0;
}

void print_friendly(std::string in_file, std::string out_file) {
	std::ifstream in(in_file);
	std::ofstream out(out_file);
	int op, d, s, t;
	in >> op;
	while (!in.eof()) {
		in >> op >> d >> s >> t;
		if (!in.eof()) {
			out << getReadableOpCode(op) << " ";
			out << d << " ";
			out << s << " ";
			out << t << std::endl;
		}
	}
	in.close();
	out.close();
}

void verbose_print(std::string src, std::string out, Tokenizer* lex, Parser* parser) {

	std::string lex_out = src + std::string(".lex.txt");
	std::string tree_out = src + std::string(".tree.txt");
	std::string friendly_code_out = src + std::string(".fcode.txt");

	std::ofstream tree(tree_out);
	std::ofstream tknlex(lex_out);

	tree << parser->getStream()->str() << std::endl;
	tknlex << lex->getStream()->str() << std::endl;
	print_friendly(out, friendly_code_out);

	tree.close();
	tknlex.close();
}

void print_help() {
	std::cout << "USAGE: compile [-v] filename" << std::endl;
	std::cout << "\t-v:\tVerbose. Outputs intermediate file analyses." << std::endl;
}

bool parse_args(int argc, char *args[], std::ifstream &src, std::string &filename, bool &verbose_flag) {

	if (argc < 2) {
		print_help();
		return false;
	} else if (argc == 2) {
		filename = args[1];
	} else {
		if (std::string(args[1]) == "-v") {
			verbose_flag = true;
		}
		filename = args[2];
	}

	src.open(filename);

	if (!src.is_open()) {
		std::cerr << "Invalid source file name specified" << std::endl;
		return false;
	}

	return true;

}
