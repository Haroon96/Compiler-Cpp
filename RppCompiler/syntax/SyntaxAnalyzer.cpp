#include "SyntaxAnalyzer.h"
#include "../compiler.h"

int SyntaxAnalyzer::getLineNumber() {
	return lex->getLineNumber();
}

// may throw exception
void SyntaxAnalyzer::parse() {

	lookahead = lex->nextToken();

	if (lookahead == nullptr) {
		throw std::exception("An error while parsing file. No tokens found.");
	}
	while (lex->hasNextToken()) {
		start();
	}
}

std::ostringstream* SyntaxAnalyzer::getStream() {
	return stream;
}

void SyntaxAnalyzer::pad(std::string name) {
	*stream << name << std::endl;
	for (int i = 0; i < depth; ++i) {
		*stream << "|---";
	}
}

bool SyntaxAnalyzer::match(Token token) {
	if (lookahead->token != token) {
		throw std::exception(std::string("Expected " + getTokenName(token) + " but encountered " + getTokenName(lookahead->token) + " instead").c_str());
	}
	lookahead = lex->nextToken();
	return true;
}

void SyntaxAnalyzer::start() {
	if (lookahead->token == DEF_STATEMENT) {
		function_declaration();
	} else if (lookahead->token == INT_TYPE || lookahead->token == CHAR_TYPE) {
		variable_declaration();
	} else {
		throw std::exception("Expected variable or function declaration");
	}
}

void SyntaxAnalyzer::function_declaration() {
	pad("Function");
	match(DEF_STATEMENT);
	match(IDENTIFIER);
	match(L_PARENTHESES);
	parameters();
	match(R_PARENTHESES);
	match(L_BRACE);
	statements();
	match(R_BRACE);
}

void SyntaxAnalyzer::parameters() {
	if (lookahead->token != R_PARENTHESES) {
		variable_declaration();
		additional_parameters();
	}
}

void SyntaxAnalyzer::variable_declaration() {
	pad("Variable");
	data_type();
	match(IDENTIFIER);
	variable_initialization();
}

void SyntaxAnalyzer::variable_initialization() {
	if (lookahead->token == ASSIGNMENT_OPERATOR) {
		match(ASSIGNMENT_OPERATOR);
		expression();
	}
}

void SyntaxAnalyzer::data_type() {
	if (lookahead->token == INT_TYPE) {
		match(INT_TYPE);
	} else if (lookahead->token == CHAR_TYPE) {
		match(CHAR_TYPE);
	} else {
		throw std::exception("Invalid data type");
	}
}

void SyntaxAnalyzer::additional_parameters() {
	if (lookahead->token == COMMA) {
		match(COMMA);
		parameters();
	}
}

void SyntaxAnalyzer::statements() {
	while (lookahead->token != R_BRACE) {
		statement();
	}
}

void SyntaxAnalyzer::statement() {

	switch (lookahead->token) {
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
	case RETURN_STATEMENT:
		return_statement();
		break;
	case INT_TYPE:
	case CHAR_TYPE:
		variable_declaration();
		break;
	case IDENTIFIER:
		identifier_prefix_statements();
		break;
	default:
		throw std::exception("Unrecognized start of statement");
		break;
	}
}

void SyntaxAnalyzer::print_statement() {
	match(PRINT_STATEMENT);
	data_element();
}
void SyntaxAnalyzer::get_statement() {
	match(GET_STATEMENT);
	match(IDENTIFIER);
}
void SyntaxAnalyzer::if_statement() {
	match(IF_STATEMENT);
	boolean_expression();
	if (lookahead->token == L_BRACE) {
		match(L_BRACE);
		statements();
		match(R_BRACE);
	} else {
		statement();
	}

	if (lookahead->token == ELSE_STATEMENT) {
		else_statement();
	}
}
void SyntaxAnalyzer::else_statement() {
	match(ELSE_STATEMENT);
	if (lookahead->token == L_BRACE) {
		match(L_BRACE);
		statements();
		match(R_BRACE);
	} else {
		statement();
	}
}
void SyntaxAnalyzer::while_statement() {
	match(WHILE_STATEMENT);
	boolean_expression();
	if (lookahead->token == L_BRACE) {
		statements();
		match(R_BRACE);
	} else {
		statement();
	}
}
void SyntaxAnalyzer::return_statement() {
	match(RETURN_STATEMENT);
	expression();
}
void SyntaxAnalyzer::identifier_prefix_statements() {
	match(IDENTIFIER);
	if (lookahead->token == L_PARENTHESES) {
		method_call();
	} else if (lookahead->token == ASSIGNMENT_OPERATOR) {
		assignment();
	}
}
void SyntaxAnalyzer::method_call() {
	match(L_PARENTHESES);
	expression();
	match(R_PARENTHESES);
}
void SyntaxAnalyzer::assignment() {
	match(ASSIGNMENT_OPERATOR);
	expression();
}
void SyntaxAnalyzer::expression() {
	term();
	expression_p();
}
void SyntaxAnalyzer::expression_p() {
	if (lookahead->token == ADDITION_OPERATOR) {
		match(ADDITION_OPERATOR);
		term();
	} else if (lookahead->token == SUBTRACTION_OPERATOR) {
		match(SUBTRACTION_OPERATOR);
		expression_p();
	}
}
void SyntaxAnalyzer::term() {
	factor();
	term_p();
}
void SyntaxAnalyzer::term_p() {
	if (lookahead->token == MULTIPLICATION_OPERATOR) {
		match(MULTIPLICATION_OPERATOR);
		factor();
		term_p();
	} else if (lookahead->token == DIVISION_OPERATOR) {
		match(DIVISION_OPERATOR);
		factor();
		term_p();
	}
}
void SyntaxAnalyzer::factor() {
	if (lookahead->token == IDENTIFIER) {
		identifier_prefix_statements();
	} else if (lookahead->token == NUMERIC_CONSTANT) {
		match(NUMERIC_CONSTANT);
	} else if (lookahead->token == L_PARENTHESES) {
		match(L_PARENTHESES);
		expression();
		match(R_PARENTHESES);
	}
}
void SyntaxAnalyzer::data_element() {
	if (lookahead->token == LITERAL_CONSTANT) {
		match(LITERAL_CONSTANT);
	} else {
		expression();
	}
}

void SyntaxAnalyzer::boolean_expression() {
	data_element();
	match(RELATIONAL_OPERATOR);
	data_element();
}
