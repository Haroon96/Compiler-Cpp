#include "Parser.h"
#include "../constants.h"
#include "../models/SymbolTable.h"
#include "../models/Symbol.h"
#include "../models/MethodSymbol.h"
#include "../models/TokenLexeme.h"

Parser::Parser(Tokenizer * lex, Translator * translator) {
	this->lex = lex;
	this->translator = translator;
	this->lookahead = nullptr;
	this->depth = 0;
	this->stream = new std::ostringstream();
	this->global_scope = new SymbolTable();
	this->global_scope->setMask(GLOBAL);
	this->curr_scope = global_scope;
}

Parser::~Parser() {
	delete global_scope;
}

int Parser::getLineNumber() {
	return lex->getLineNumber();
}

// may throw exception
void Parser::parse() {

	lookahead = lex->nextToken();

	if (lookahead == nullptr) {
		throw std::runtime_error("An error occured while parsing the file. No tokens found.");
	}
	pad("Global declaration list");
	while (lex->hasNextToken()) {
		start();
	}

	Symbol *symbol = global_scope->getSymbol("main");
	if (symbol == nullptr) {
		throw std::runtime_error("No main method found");
	}
	translator->finalize(global_scope);
}

std::ostringstream* Parser::getStream() {
	return stream;
}

void Parser::start_new_scope() {
	this->curr_scope = new SymbolTable();
}

void Parser::end_of_scope() {
	delete curr_scope;
	curr_scope = global_scope;
}

void Parser::pad(std::string name) {
	for (int i = 0; i < depth; ++i) {
		*stream << "|-- ";
	}
	*stream << name << std::endl;
}

bool Parser::match(Token token) {
	if (lookahead->getToken() != token) {
		throw std::runtime_error(std::string("Expected " + getTokenName(token) + " but encountered " + getTokenName(lookahead->getToken()) + " instead").c_str());
	}
	if (token == IDENTIFIER) {
		Symbol* symbol = locateSymbol(lookahead->getLexeme());
		if (symbol == nullptr) {
			throw std::runtime_error(std::string("Undeclared identifier " + lookahead->getLexeme()).c_str());
		}
	}
	pad(lookahead->getLexeme());
	if (lookahead != nullptr) {
		delete lookahead;
	}
	lookahead = lex->nextToken();
	return true;
}

Symbol *Parser::locateSymbol(std::string lexeme) {
	// if global scope or symbol not found locally, look in global scope
	Symbol *symbol = nullptr;
	if (curr_scope == nullptr || (symbol = curr_scope->getSymbol(lexeme)) == nullptr) {
		symbol = global_scope->getSymbol(lexeme);
	}
	return symbol;
}

MethodSymbol* Parser::locateMethod(std::string lexeme) {
	MethodSymbol *method;
	method = (MethodSymbol*)global_scope->getSymbol(lexeme);
	return (method == nullptr || method->getType() != METHOD) ? nullptr : method;
}

void Parser::increase_depth() {
	depth++;
}

void Parser::decrease_depth() {
	depth--;
}

std::string Parser::process_token_error(Token token) {
	// you can throw custom errors based on the token received here
	return std::string("Unrecognized start of statement");
}

void Parser::start() {
	increase_depth();
	if (lookahead->getToken() == DEF_STATEMENT) {
		pad("Function declaration");
		start_new_scope();
		translator->reset_temp_vars();
		function_declaration();
	} else if (lookahead->getToken() == INT_TYPE || lookahead->getToken() == CHAR_TYPE) {
		pad("Variable declaration");
		global_variable_declaration();
	} else {
		throw std::runtime_error("Expected variable or function declaration");
	}
	decrease_depth();
}

void Parser::function_declaration() {
	increase_depth();
	match(DEF_STATEMENT);

	std::string lexeme = lookahead->getLexeme();

	MethodSymbol *symbol = new MethodSymbol(lexeme);
	symbol->setOffset(translator->get_instruction_count());
	int index = global_scope->addSymbol(symbol);

	translator->write(LABEL);
	translator->write(index);
	translator->mark_patch();
	translator->write(UNUSED);
	translator->next_instruction();

	translator->write(ALLOC);
	translator->mark_patch();
	translator->write(UNUSED);
	translator->write(UNUSED);
	translator->next_instruction();

	match(IDENTIFIER);

	match(L_PARENTHESES);
	
	pad("Parameters");
	increase_depth();
	parameters(symbol);
	decrease_depth();

	match(R_PARENTHESES);
	match(L_BRACE);
	statements();
	return_statement();

	// local vars - params
	translator->patch(curr_scope->getLength() - symbol->getLength());
	translator->patch(translator->get_instruction_count());

	match(R_BRACE);
	decrease_depth();
}

int Parser::parameters(MethodSymbol *method) {

	if (lookahead->getToken() == R_PARENTHESES) {
		return -2;
	}

	bool is_array;
	pad("Parameter");
	is_array = false;

	SymbolType symbolType = data_type();

	Symbol *symbol = new Symbol(lookahead->getLexeme());

	curr_scope->addSymbol(symbol);
	symbol->setType(symbolType);
	symbol->setLength(1);

	match(IDENTIFIER);

	if (lookahead->getToken() == L_SUBSCRIPT_OPERATOR) {
		match(L_SUBSCRIPT_OPERATOR);
		symbol->setLength(std::stoi(lookahead->getLexeme()));
		match(NUMERIC_CONSTANT);
		match(R_SUBSCRIPT_OPERATOR);
		is_array = true;
	}

	if (is_array) {
		if (symbolType == INT_VAR) {
			method->addParam(INT_ARR);
			symbol->setType(INT_ARR);
		} else {
			method->addParam(CHAR_ARR);
			symbol->setType(CHAR_ARR);
		}
	} else {
		method->addParam(symbolType);
	}

	if (lookahead->getToken() == COMMA) {
		match(COMMA);
	}
	int offset = parameters(method) - 1;
	symbol->setOffset(offset);
	return offset;
}

void Parser::variable_declaration() {
	increase_depth(); 
	SymbolType symbolType = data_type();
	variable_declaration_list(symbolType);
	decrease_depth();
}

void Parser::variable_declaration_list(SymbolType type) {

	std::string lexeme = lookahead->getLexeme();

	Symbol *symbol = new Symbol(lexeme);

	curr_scope->addSymbol(symbol);

	symbol->setType(type);
	symbol->setLength(1);
	symbol->setOffset(curr_scope->nextOffset());

	match(IDENTIFIER);

	if (lookahead->getToken() == ASSIGNMENT_OPERATOR) {
		variable_initialization();
		translator->write_instruction(MOV, symbol->getOffset(), curr_scope->getSymbol(translator->pop())->getOffset());
		translator->next_instruction();
	}

	if (lookahead->getToken() == COMMA) {
		match(COMMA);
		variable_declaration_list(type);
	}

}

void Parser::global_variable_declaration() {
	increase_depth();
	SymbolType symbolType = data_type();
	global_variable_declaration_list(symbolType);
	decrease_depth();
}

void Parser::global_variable_declaration_list(SymbolType type) {

	std::string lexeme = lookahead->getLexeme();

	Symbol *symbol = new Symbol(lexeme);
	
	global_scope->addSymbol(symbol);
	
	symbol->setType(type);
	symbol->setLength(1);
	symbol->setOffset(global_scope->nextOffset());

	match(IDENTIFIER);

	if (lookahead->getToken() == ASSIGNMENT_OPERATOR) {
		variable_initialization();
		translator->write_instruction(MOV, symbol->getOffset(), global_scope->getSymbol(translator->pop())->getOffset());
		translator->next_instruction();
	} else if (lookahead->getToken() == L_SUBSCRIPT_OPERATOR) {
		array_declaration();
		symbol->setLength(std::stoi(translator->pop()));
		if (type == INT_VAR) {
			symbol->setType(INT_ARR);
		} else {
			symbol->setType(CHAR_ARR);
		}
	}
	
	if (lookahead->getToken() == COMMA) {
		match(COMMA);
		global_variable_declaration_list(type);
	}
}

void Parser::variable_initialization() {
	pad("Variable initialization");
	increase_depth();
	assignment();
	decrease_depth();
}

SymbolType Parser::data_type() {
	if (lookahead->getToken() == INT_TYPE) {
		match(INT_TYPE);
		return INT_VAR;
	} else if (lookahead->getToken() == CHAR_TYPE) {
		match(CHAR_TYPE);
		return CHAR_VAR;
	} else {
		throw std::runtime_error("Invalid data type");
	}
}

void Parser::statements(int reset_point) {
	pad("Statement list");
	increase_depth();
	while (lookahead->getToken() != R_BRACE && lookahead->getToken() != RETURN_STATEMENT) {
		translator->reset_temp_index(reset_point);
		statement();
	}
	decrease_depth();
}

void Parser::statement() {
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
		throw std::runtime_error(process_token_error(lookahead->getToken()));
		break;
	}
	decrease_depth();
}

void Parser::print_statement() {
	pad("Print statement");
	increase_depth();
	match(PRINT_STATEMENT);
	if (lookahead->getToken() == LITERAL_CONSTANT) {
		translator->write_instruction(CHAR_OUT, lookahead->getLexeme()[1]);
		match(LITERAL_CONSTANT);
	} else {
		expression();
		translator->write_instruction(OUT, locateSymbol(translator->pop())->getOffset());
	}
	translator->next_instruction();
	decrease_depth();
}
void Parser::get_statement() {
	pad("Get statement");
	increase_depth();
	match(GET_STATEMENT);

	// add check for methods here
	std::string id = lookahead->getLexeme();
	match(IDENTIFIER);
	if (lookahead->getToken() != L_SUBSCRIPT_OPERATOR) {
		translator->write_instruction(IN, curr_scope->getSymbol(id)->getOffset());
	} else {
		std::string tmp = translator->get_temp_var(INT_VAR, curr_scope);
		translator->write_instruction(IN, curr_scope->getSymbol(tmp)->getOffset());
		translator->next_instruction();

		match(L_SUBSCRIPT_OPERATOR);
		expression();
		match(R_SUBSCRIPT_OPERATOR);

		translator->write_instruction(
			MOV_TO_ARR,
			curr_scope->getSymbol(id)->getOffset(),
			curr_scope->getSymbol(translator->pop())->getOffset(),
			curr_scope->getSymbol(tmp)->getOffset()
		);

	}
	translator->next_instruction();
	decrease_depth();
}
void Parser::if_statement() {
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
		curr_scope->getSymbol(op1)->getOffset(),
		curr_scope->getSymbol(op2)->getOffset(),
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
void Parser::else_statement() {
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
void Parser::while_statement() {
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
		curr_scope->getSymbol(op1)->getOffset(),
		curr_scope->getSymbol(op2)->getOffset(),
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
void Parser::return_statement() {
	pad("Return statement");
	increase_depth();
	if (lookahead->getToken() == RETURN_STATEMENT) {
		match(RETURN_STATEMENT);
		expression();
		translator->write_instruction(RET, curr_scope->getSymbol(translator->pop())->getOffset());
	} else {
		translator->write_instruction(RET, 0);
	}
	translator->next_instruction();
	decrease_depth();
}
void Parser::identifier_prefix_statements() {
	std::string name = lookahead->getLexeme();
	match(IDENTIFIER);
	if (lookahead->getToken() == L_PARENTHESES) {
		MethodSymbol *symbol = locateMethod(name);
		if (symbol == nullptr) {
			throw std::runtime_error("Call to undeclared procedure");
		}
		method_call(symbol);
		translator->write_instruction(CALL, symbol->getOffset());
		translator->next_instruction();
		translator->write_instruction(CALL_CLEANUP_NO_RESULT, symbol->getLength());
		translator->next_instruction();
	} else if (lookahead->getToken() == L_SUBSCRIPT_OPERATOR) {
		Symbol *symbol = locateSymbol(name);
		match(L_SUBSCRIPT_OPERATOR);
		expression();
		match(R_SUBSCRIPT_OPERATOR);

		assignment();

		std::string result = translator->pop();

		translator->write_instruction(
			MOV_TO_ARR,
			symbol->getOffset(),
			curr_scope->getSymbol(translator->pop())->getOffset(),
			curr_scope->getSymbol(result)->getOffset()
		);
		translator->next_instruction();
	} else if (lookahead->getToken() == ASSIGNMENT_OPERATOR) {
		Symbol *symbol = locateSymbol(name);
		assignment();
		translator->write_instruction(
			MOV,
			symbol->getOffset(),
			curr_scope->getSymbol(translator->pop())->getOffset()
		);
		translator->next_instruction();
	}
}
void Parser::method_call(MethodSymbol *method) {
	pad("Function call");
	increase_depth();
	pad("Function parameter list");
	increase_depth();
	try {
		match(L_PARENTHESES);
		std::vector<SymbolType> params = method->getParams();
		for (std::vector<SymbolType>::iterator i = params.begin(); i != params.end(); ++i) {
			switch (*i) {
			case INT_VAR:
				expression();
				translator->write_instruction(PARAM, locateSymbol(translator->pop())->getOffset());
				translator->next_instruction();
				break;
			case INT_ARR:
				break;
			case CHAR_VAR:
				break;
			case CHAR_ARR:
				break;
			}
			if (i + 1 != params.end()) {
				match(COMMA);
			}
		}
		match(R_PARENTHESES);
	} catch (std::runtime_error e) {
		throw std::runtime_error(std::string("Invalid parameter list for method " + method->getName()).c_str());
	}
	decrease_depth();
	decrease_depth();
}
void Parser::assignment() {
	pad("Assignment");
	increase_depth();
	match(ASSIGNMENT_OPERATOR);
	expression();
	decrease_depth();
}
void Parser::expression() {
	pad("Expression");
	increase_depth();
	term();
	expression_p();
	decrease_depth();
}

void Parser::expression_p() {
	pad("Expression'");
	increase_depth();
	if (lookahead->getToken() == ADDITION_OPERATOR || lookahead->getToken() == SUBTRACTION_OPERATOR) {
		std::string op = lookahead->getLexeme();

		match(lookahead->getToken());

		term();
		expression_p();

		std::string tmp = translator->get_temp_var(INT_VAR, curr_scope);

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
			locateSymbol(tmp)->getOffset(),
			locateSymbol(operand1)->getOffset(),
			locateSymbol(operand2)->getOffset()
		);
		translator->next_instruction();
		translator->push(tmp);
	}
	decrease_depth();
}

void Parser::term() {
	pad("Term");
	increase_depth();
	factor();
	term_p();
	decrease_depth();
}

void Parser::term_p() {
	pad("Term'");
	increase_depth();
	if (lookahead->getToken() == MULTIPLICATION_OPERATOR || lookahead->getToken() == DIVISION_OPERATOR) {
		std::string op = lookahead->getLexeme();
		match(lookahead->getToken());
		factor();
		term_p();

		std::string tmp = translator->get_temp_var(INT_VAR, curr_scope);

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
			locateSymbol(tmp)->getOffset(),
			locateSymbol(operand1)->getOffset(),
			locateSymbol(operand2)->getOffset()
		);
		translator->next_instruction();

		translator->push(tmp);
	}
	decrease_depth();
}

void Parser::factor() {
	pad("Factor");
	increase_depth();
	if (lookahead->getToken() == IDENTIFIER) {
		identifier_prefix_factors();
	} else if (lookahead->getToken() == NUMERIC_CONSTANT) {
		std::string tmp_var = translator->get_temp_var(INT_VAR, curr_scope);
		translator->write_instruction(
			SET,
			curr_scope->getSymbol(tmp_var)->getOffset(),
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
		throw std::runtime_error("Invalid expression");
	}
	decrease_depth();
}

void Parser::identifier_prefix_factors() {
	std::string name = lookahead->getLexeme();
	Symbol *symbol = locateSymbol(name);
	match(IDENTIFIER);
	if (lookahead->getToken() == L_PARENTHESES) {
		MethodSymbol *symbol = locateMethod(name);
		if (symbol == nullptr) {
			throw std::runtime_error("Call to undeclared procedure");
		}
		method_call(symbol);
		std::string tmp = translator->get_temp_var(INT_VAR, curr_scope);
		translator->write_instruction(
			CALL,
			symbol->getOffset()
		);
		translator->next_instruction();
		translator->write_instruction(
			CALL_CLEANUP,
			curr_scope->getSymbol(tmp)->getOffset(),
			symbol->getLength()
		);
		translator->next_instruction();
		translator->push(tmp);
	} else if (lookahead->getToken() == L_SUBSCRIPT_OPERATOR) {
		match(L_SUBSCRIPT_OPERATOR);
		expression();
		match(R_SUBSCRIPT_OPERATOR);

		std::string tmp = translator->get_temp_var(INT_VAR, curr_scope);

		translator->write_instruction(
			MOV_FROM_ARR,
			locateSymbol(tmp)->getOffset(),
			locateSymbol(name)->getOffset(),
			locateSymbol(translator->pop())->getOffset()
		);
		translator->next_instruction();
		translator->push(tmp);

	} else {
		translator->push(symbol->getName());
	}
}

void Parser::boolean_expression() {
	pad("Boolean expression");
	increase_depth();
	expression();
	relational_operator();
	expression();
	decrease_depth();
}

void Parser::relational_operator() {
	pad("Relational operator");
	increase_depth();
	translator->push(lookahead->getLexeme());
	match(RELATIONAL_OPERATOR);
	decrease_depth();
}

void Parser::array_declaration() {
	pad("Array declaration");
	increase_depth();
	match(L_SUBSCRIPT_OPERATOR);
	translator->push(lookahead->getLexeme());
	match(NUMERIC_CONSTANT);
	match(R_SUBSCRIPT_OPERATOR);
	decrease_depth();
}