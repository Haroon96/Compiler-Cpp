#include "SyntaxAnalyzer.h"
#include "../compiler.h"

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer * lex, Translator * translator, SymbolTable *symbolTable) {
	this->lex = lex;
	this->translator = translator;
	this->symbolTable = symbolTable;
	this->lookahead = nullptr;
	this->depth = 0;
	this->stream = new std::ostringstream();
}

int SyntaxAnalyzer::getLineNumber() {
	return lex->getLineNumber();
}

// may throw exception
void SyntaxAnalyzer::parse() {

	lookahead = lex->nextToken();
	
	/*
	translator->write(GOTO);
	translator->mark_patch();
	translator->write(UNUSED);
	translator->write(UNUSED);
	translator->next_instruction();
	*/
	if (lookahead == nullptr) {
		throw std::exception("An error occured while parsing the file. No tokens found.");
	}
	pad("Global declaration list");
	while (lex->hasNextToken()) {
		start();
	}

	/*
	int index = symbolTable->indexOf("main");
	if (index == -1) {
		throw std::exception("No main method found");
	}
	Symbol *symbol = symbolTable->getSymbol(std::to_string(index));
	translator->patch(symbol->getOffset());
	*/
}

std::ostringstream* SyntaxAnalyzer::getStream() {
	return stream;
}

void SyntaxAnalyzer::pad(std::string name) {
	for (int i = 0; i < depth; ++i) {
		*stream << "|-- ";
	}
	*stream << name << std::endl;
}

bool SyntaxAnalyzer::match(Token token) {
	if (lookahead->getToken() != token) {
		throw std::exception(std::string("Expected " + getTokenName(token) + " but encountered " + getTokenName(lookahead->getToken()) + " instead").c_str());
	}
	if (token == IDENTIFIER) {
		Symbol* symbol = symbolTable->getSymbol(lookahead->getLexeme());
		if (symbol->getOffset() == -1) {
			throw std::exception(("Undeclared variable " + symbol->getName()).c_str());
		}
	}
	pad(lookahead->getLexeme());
	if (lookahead != nullptr) {
		delete lookahead;
	}
	lookahead = lex->nextToken();
	return true;
}

void SyntaxAnalyzer::increase_depth() {
	depth++;
}

void SyntaxAnalyzer::decrease_depth() {
	depth--;
}

char * SyntaxAnalyzer::process_token_error(Token token) {
	switch (token) {
	case ELSE_STATEMENT:
		return "Dangling else statement";
		break;
	default:
		return "Unrecognized start of statement";
		break;
	}
}

void SyntaxAnalyzer::start() {
	increase_depth();
	if (lookahead->getToken() == DEF_STATEMENT) {
		pad("Function declaration");
		function_declaration();
	} else if (lookahead->getToken() == INT_TYPE || lookahead->getToken() == CHAR_TYPE) {
		pad("Variable declaration");
		variable_declaration();
	} else {
		throw std::exception("Expected variable or function declaration");
	}
	decrease_depth();
}

void SyntaxAnalyzer::function_declaration() {
	increase_depth();
	match(DEF_STATEMENT);

	translator->write_label(lookahead->getLexeme());
	translator->next_instruction();

	// fix this chaipi
	std::string lexeme = lookahead->getLexeme();
	symbolTable->getSymbol(lexeme)->setOffset(0);
	match(IDENTIFIER);
	symbolTable->removeSymbol(lexeme);

	match(L_PARENTHESES);
	
	pad("Parameters");
	increase_depth();
	parameters();
	decrease_depth();

	match(R_PARENTHESES);
	match(L_BRACE);
	statements();
	return_statement();
	match(R_BRACE);
	decrease_depth();
}

void SyntaxAnalyzer::parameters() {
	/*
	while (lookahead->getToken() != R_PARENTHESES) {
		pad("Parameter");
		variable_declaration();
		//translator->write(translator->pop());
		if (lookahead->getToken() == COMMA) {
		//	translator->write(",");
			match(COMMA);
		}
	}
	*/
}

void SyntaxAnalyzer::variable_declaration() {
	increase_depth();

	SymbolType symbolType = data_type();
	
	Symbol *symbol = symbolTable->getSymbol(lookahead->getLexeme());
	symbol->setType(symbolType);
	symbol->setLength(1);
	symbol->setOffset(symbolTable->nextOffset());

	match(IDENTIFIER);

	if (lookahead->getToken() == ASSIGNMENT_OPERATOR) {
		variable_initialization();
		translator->write_instruction(MOV, symbol->getOffset(), symbolTable->getSymbol(translator->pop())->getOffset());
		translator->next_instruction();
	} else if (lookahead->getToken() == L_SUBSCRIPT_OPERATOR) {
		array_declaration();
		symbol->setLength(std::stoi(translator->pop()));
	}
	
	decrease_depth();
}

void SyntaxAnalyzer::variable_initialization() {
	pad("Variable initialization");
	increase_depth();
	assignment();
	decrease_depth();
}

SymbolType SyntaxAnalyzer::data_type() {
//	translator->push(lookahead->getLexeme());
	if (lookahead->getToken() == INT_TYPE) {
		match(INT_TYPE);
		return INT_VAR;
	} else if (lookahead->getToken() == CHAR_TYPE) {
		match(CHAR_TYPE);
		return CHAR_VAR;
	} else {
		throw std::exception("Invalid data type");
	}
}

void SyntaxAnalyzer::statements(int reset_point) {
	pad("Statement list");
	increase_depth();
	while (lookahead->getToken() != R_BRACE) {
		translator->reset_temp_index(reset_point);
		statement();
	}
	decrease_depth();
}

void SyntaxAnalyzer::statement() {
	pad("Statement");
	increase_depth();
	switch (lookahead->getToken()) {
	case PRINT_STATEMENT:
		print_statement();
		break;
	case GET_STATEMENT:
		get_statement();
		break;
	case IF_STATEMENT:
		if_statement();
		break;
	case WHILE_STATEMENT:
		while_statement();
		break;
	case INT_TYPE:
	case CHAR_TYPE:
		pad("Variable declaration");
		variable_declaration();
		break;
	case IDENTIFIER:
		identifier_prefix_statements();
		break;
	default:
		throw std::exception(process_token_error(lookahead->getToken()));
		break;
	}
	decrease_depth();
}

void SyntaxAnalyzer::print_statement() {
	pad("Print statement");
	increase_depth();
	match(PRINT_STATEMENT);
	if (lookahead->getToken() == LITERAL_CONSTANT) {
		translator->write_instruction(CHAR_OUT, lookahead->getLexeme()[1]);
		match(LITERAL_CONSTANT);
	} else {
		expression();
		translator->write_instruction(OUT, symbolTable->getSymbol(translator->pop())->getOffset());
	}
	translator->next_instruction();
	decrease_depth();
}
void SyntaxAnalyzer::get_statement() {
	pad("Get statement");
	increase_depth();
	match(GET_STATEMENT);

	// add check for methods here
	std::string id = lookahead->getLexeme();
	match(IDENTIFIER);
	if (lookahead->getToken() != L_SUBSCRIPT_OPERATOR) {
		translator->write_instruction(IN, symbolTable->getSymbol(id)->getOffset());
	} else {
		match(L_SUBSCRIPT_OPERATOR);
		expression();
		match(R_SUBSCRIPT_OPERATOR);

		translator->write_instruction(
			ARR_IN,
			symbolTable->getSymbol(id)->getOffset(),
			symbolTable->getSymbol(translator->pop())->getOffset()
		);

	}
	translator->next_instruction();
	decrease_depth();
}
void SyntaxAnalyzer::if_statement() {
	pad("IF statement");
	increase_depth();
	match(IF_STATEMENT);

	boolean_expression();

	std::string op2 = translator->pop();
	std::string op = translator->pop();
	std::string op1 = translator->pop();

	OpCode opcode;

	if (op == "==") {
		opcode = IF_E_GOTO;
	} else if (op == "!=") {
		opcode = IF_NE_GOTO;
	} else if (op == "<=") {
		opcode = IF_LE_GOTO;
	} else if (op == ">=") {
		opcode = IF_GE_GOTO;
	} else if (op == "<") {
		opcode = IF_L_GOTO;
	} else if (op == ">") {
		opcode = IF_G_GOTO;
	}

	translator->write_instruction(
		opcode,
		symbolTable->getSymbol(op1)->getOffset(),
		symbolTable->getSymbol(op2)->getOffset(),
		translator->get_instruction_count() + 2
	);

	translator->next_instruction();
	translator->write(GOTO);
	translator->mark_patch();
	translator->write(UNUSED);
	translator->write(UNUSED);
	translator->next_instruction();

	if (lookahead->getToken() == L_BRACE) {
		match(L_BRACE);
		statements();
		match(R_BRACE);
	} else {
		statement();
	}
	
	decrease_depth();

	int end = translator->get_instruction_count();
	if (lookahead->getToken() == ELSE_STATEMENT) {
		else_statement();
		++end;
	}
	translator->patch(end);
}
void SyntaxAnalyzer::else_statement() {
	pad("ELSE statement");
	increase_depth();
	translator->write(GOTO);
	translator->mark_patch();
	translator->write(UNUSED);
	translator->write(UNUSED);
	translator->next_instruction();
	match(ELSE_STATEMENT);
	if (lookahead->getToken() == L_BRACE) {
		match(L_BRACE);
		statements();
		match(R_BRACE);
	} else {
		statement();
	}
	translator->patch(translator->get_instruction_count());
	decrease_depth();
}
void SyntaxAnalyzer::while_statement() {
	pad("WHILE statement");
	increase_depth();
	match(WHILE_STATEMENT);

	int start = translator->get_instruction_count();
	
	boolean_expression();

	std::string op2 = translator->pop();
	std::string op = translator->pop();
	std::string op1 = translator->pop();

	OpCode opcode;

	if (op == "==") {
		opcode = IF_E_GOTO;
	} else if (op == "!=") {
		opcode = IF_NE_GOTO;
	} else if (op == "<=") {
		opcode = IF_LE_GOTO;
	} else if (op == ">=") {
		opcode = IF_GE_GOTO;
	} else if (op == "<") {
		opcode = IF_L_GOTO;
	} else if (op == ">") {
		opcode = IF_G_GOTO;
	}

	translator->write_instruction(
		opcode,
		symbolTable->getSymbol(op1)->getOffset(),
		symbolTable->getSymbol(op2)->getOffset(),
		translator->get_instruction_count() + 2
	);

	translator->next_instruction();
	translator->write(GOTO);
	translator->mark_patch();
	translator->write(UNUSED);
	translator->write(UNUSED);
	translator->next_instruction();

	if (lookahead->getToken() == L_BRACE) {
		match(L_BRACE);
		statements(1);
		match(R_BRACE);
	} else {
		statement();
	}
	translator->write_instruction(GOTO, start);
	translator->next_instruction();
	translator->patch(translator->get_instruction_count());
	decrease_depth();
}
void SyntaxAnalyzer::return_statement() {
	pad("Return statement");
	increase_depth();
	if (lookahead->getToken() == RETURN_STATEMENT) {
		match(RETURN_STATEMENT);
		expression();
		translator->write_instruction(RET, symbolTable->getSymbol(translator->pop())->getOffset());
	} else {
		translator->write_instruction(RET, 0);
	}
	translator->next_instruction();
	decrease_depth();
}
void SyntaxAnalyzer::identifier_prefix_statements() {
	std::string id = lookahead->getLexeme();
	match(IDENTIFIER);
	if (lookahead->getToken() == L_PARENTHESES) {
		/*int params = method_call();
		std::string tmp = translator->get_temp_var();
		translator->write(tmp);
		translator->write("=");
		translator->write(id);
		translator->write("(");
		for (int i = 0; i < params; ++i) {
			translator->mark_patch();
			if (i < params - 1)
				translator->write(",");
		}
		for (int i = 0; i < params; ++i) {
			translator->patch(translator->pop());
		}
		translator->write(")");
		translator->newline();
		translator->push(tmp);*/
	} else if (lookahead->getToken() == L_SUBSCRIPT_OPERATOR) {
		match(L_SUBSCRIPT_OPERATOR);
		expression();
		match(R_SUBSCRIPT_OPERATOR);

		assignment();

		std::string result = translator->pop();

		translator->write_instruction(
			MOV_TO_ARR,
			symbolTable->getSymbol(id)->getOffset(),
			symbolTable->getSymbol(translator->pop())->getOffset(),
			symbolTable->getSymbol(result)->getOffset()
		);
		translator->next_instruction();
	} else if (lookahead->getToken() == ASSIGNMENT_OPERATOR) {
		assignment();
		translator->write_instruction(
			MOV,
			symbolTable->getSymbol(id)->getOffset(),
			symbolTable->getSymbol(translator->pop())->getOffset()
		);
		translator->next_instruction();
	}
}
int SyntaxAnalyzer::method_call() {
	/*
	pad("Function call");
	increase_depth();
	match(L_PARENTHESES);
	pad("Function parameter list");
	increase_depth();
	int param_count = 0;
	while (lookahead->getToken() != R_PARENTHESES) {
		expression();
		if (lookahead->getToken() == COMMA) {
			match(COMMA);
		}
		++param_count;
	}
	decrease_depth();
	match(R_PARENTHESES);
	decrease_depth();
	return param_count;
	*/
	return 0;
}
void SyntaxAnalyzer::assignment() {
	pad("Assignment");
	increase_depth();
	match(ASSIGNMENT_OPERATOR);
	expression();
	decrease_depth();
}
void SyntaxAnalyzer::expression() {
	pad("Expression");
	increase_depth();
	term();
	expression_p();
	decrease_depth();
}

void SyntaxAnalyzer::expression_p() {
	pad("Expression'");
	increase_depth();
	if (lookahead->getToken() == ADDITION_OPERATOR || lookahead->getToken() == SUBTRACTION_OPERATOR) {
		std::string op = lookahead->getLexeme();

		match(lookahead->getToken());

		term();
		expression_p();

		std::string tmp = translator->get_temp_var();

		std::string operand2 = translator->pop();
		std::string operand1 = translator->pop();

		OpCode opcode;
		if (op == "+") {
			opcode = ADD;
		} else if (op == "-") {
			opcode = SUB;
		}

		translator->write_instruction(
			opcode,
			symbolTable->getSymbol(tmp)->getOffset(),
			symbolTable->getSymbol(operand1)->getOffset(),
			symbolTable->getSymbol(operand2)->getOffset()
		);
		translator->next_instruction();
		translator->push(tmp);
	}
	decrease_depth();
}

void SyntaxAnalyzer::term() {
	pad("Term");
	increase_depth();
	factor();
	term_p();
	decrease_depth();
}

void SyntaxAnalyzer::term_p() {
	pad("Term'");
	increase_depth();
	if (lookahead->getToken() == MULTIPLICATION_OPERATOR || lookahead->getToken() == DIVISION_OPERATOR) {
		std::string op = lookahead->getLexeme();
		match(lookahead->getToken());
		factor();
		term_p();

		std::string tmp = translator->get_temp_var();

		std::string operand2 = translator->pop();
		std::string operand1 = translator->pop();

		OpCode opcode;
		if (op == "*") {
			opcode = MUL;
		} else if (op == "/") {
			opcode = DIV;
		}

		translator->write_instruction(
			opcode,
			symbolTable->getSymbol(tmp)->getOffset(),
			symbolTable->getSymbol(operand1)->getOffset(),
			symbolTable->getSymbol(operand2)->getOffset()
		);
		translator->next_instruction();

		translator->push(tmp);
	}
	decrease_depth();
}

void SyntaxAnalyzer::factor() {
	pad("Factor");
	increase_depth();
	if (lookahead->getToken() == IDENTIFIER) {
		identifier_prefix_factors();
	} else if (lookahead->getToken() == NUMERIC_CONSTANT) {
		std::string tmp_var = translator->get_temp_var();
		translator->write_instruction(
			SET,
			symbolTable->getSymbol(tmp_var)->getOffset(),
			std::stoi(lookahead->getLexeme())
		);
		translator->next_instruction();
		translator->push(tmp_var);
		match(NUMERIC_CONSTANT);
	} else if (lookahead->getToken() == L_PARENTHESES) {
		match(L_PARENTHESES);
		expression();
		match(R_PARENTHESES);
	} else if (lookahead->getToken() == LITERAL_CONSTANT) {
		translator->push(lookahead->getLexeme());
		match(LITERAL_CONSTANT);
	} else {
		throw std::exception("Invalid expression");
	}
	decrease_depth();
}

void SyntaxAnalyzer::identifier_prefix_factors() {
	std::string id = lookahead->getLexeme();
	match(IDENTIFIER);
	if (lookahead->getToken() == L_PARENTHESES) {
		/*
		int params = method_call();
		std::string tmp = translator->get_temp_var();
		translator->write(tmp);
		translator->write("=");
		translator->write(id);
		translator->write("(");
		for (int i = 0; i < params; ++i) {
			translator->mark_patch();
			if (i < params - 1)
				translator->write(",");
		}
		for (int i = 0; i < params; ++i) {
			translator->patch(translator->pop());
		}
		translator->write(")");
		translator->newline();
		translator->push(tmp);
		*/
	} else if (lookahead->getToken() == L_SUBSCRIPT_OPERATOR) {
		match(L_SUBSCRIPT_OPERATOR);
		expression();
		match(R_SUBSCRIPT_OPERATOR);

		std::string tmp = translator->get_temp_var();

		translator->write_instruction(
			MOV_FROM_ARR,
			symbolTable->getSymbol(tmp)->getOffset(),
			symbolTable->getSymbol(id)->getOffset(),
			symbolTable->getSymbol(translator->pop())->getOffset()
		);
		translator->next_instruction();
		translator->push(tmp);

	} else {
		translator->push(id);
	}
}

void SyntaxAnalyzer::boolean_expression() {
	pad("Boolean expression");
	increase_depth();
	expression();
	relational_operator();
	expression();
	decrease_depth();
}

void SyntaxAnalyzer::relational_operator() {
	pad("Relational operator");
	increase_depth();
	translator->push(lookahead->getLexeme());
	match(RELATIONAL_OPERATOR);
	decrease_depth();
}

void SyntaxAnalyzer::array_declaration() {
	pad("Array declaration");
	increase_depth();
	match(L_SUBSCRIPT_OPERATOR);
	translator->push(lookahead->getLexeme());
	match(NUMERIC_CONSTANT);
	match(R_SUBSCRIPT_OPERATOR);
	decrease_depth();
}

