#include "SyntaxAnalyzer.h"
#include "../compiler.h"

int SyntaxAnalyzer::getLineNumber() {
	return lex->getLineNumber();
}

// may throw exception
void SyntaxAnalyzer::parse() {

	lookahead = lex->nextToken();

	if (lookahead == nullptr) {
		throw std::exception("An error occured while parsing the file. No tokens found.");
	}
	pad("Global declaration list");
	while (lex->hasNextToken()) {
		start();
	}
}

std::ostringstream* SyntaxAnalyzer::getStream() {
	return stream;
}

void SyntaxAnalyzer::pad(std::string name) {
	for (int i = 0; i < depth; ++i) {
		*stream << "|---";
	}
	*stream << name << std::endl;
}

bool SyntaxAnalyzer::match(Token token) {
	if (lookahead->token != token) {
		throw std::exception(std::string("Expected " + getTokenName(token) + " but encountered " + getTokenName(lookahead->token) + " instead").c_str());
	}
	pad(lookahead->lexeme);
	lookahead = lex->nextToken();
	return true;
}

void SyntaxAnalyzer::increase_depth() {
	depth++;
}

void SyntaxAnalyzer::decrease_depth() {
	depth--;
}

void SyntaxAnalyzer::start() {
	increase_depth();
	if (lookahead->token == DEF_STATEMENT) {
		pad("Function declaration");
		function_declaration();
	} else if (lookahead->token == INT_TYPE || lookahead->token == CHAR_TYPE) {
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
	match(IDENTIFIER);
	match(L_PARENTHESES);
	
	pad("Parameters");
	increase_depth();
	parameters();
	decrease_depth();
	
	match(R_PARENTHESES);
	match(L_BRACE);
	statements();
	match(R_BRACE);
	decrease_depth();
}

void SyntaxAnalyzer::parameters() {
	if (lookahead->token != R_PARENTHESES) {
		pad("Parameter");
		variable_declaration();
		additional_parameters();
	}
}

void SyntaxAnalyzer::variable_declaration() {
	increase_depth();

	data_type();
	match(IDENTIFIER);
	variable_initialization();

	decrease_depth();
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
	pad("Statement list");
	increase_depth();
	while (lookahead->token != R_BRACE) {
		statement();
	}
	decrease_depth();
}

void SyntaxAnalyzer::statement() {
	pad("Statement");
	increase_depth();
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
	decrease_depth();
}

void SyntaxAnalyzer::print_statement() {
	pad("Print statement");
	increase_depth();
	match(PRINT_STATEMENT);
	data_element();
	decrease_depth();
}
void SyntaxAnalyzer::get_statement() {
	pad("Get statement");
	increase_depth();
	match(GET_STATEMENT);
	match(IDENTIFIER);
	decrease_depth();
}
void SyntaxAnalyzer::if_statement() {
	pad("IF statement");
	increase_depth();
	match(IF_STATEMENT);
	boolean_expression();
	if (lookahead->token == L_BRACE) {
		match(L_BRACE);
		statements();
		match(R_BRACE);
	} else {
		statement();
	}
	decrease_depth();

	if (lookahead->token == ELSE_STATEMENT) {
		else_statement();
	}
}
void SyntaxAnalyzer::else_statement() {
	pad("ELSE statement");
	increase_depth();
	match(ELSE_STATEMENT);
	if (lookahead->token == L_BRACE) {
		match(L_BRACE);
		statements();
		match(R_BRACE);
	} else {
		statement();
	}
	decrease_depth();
}
void SyntaxAnalyzer::while_statement() {
	pad("WHILE statement");
	increase_depth();
	match(WHILE_STATEMENT);
	boolean_expression();
	if (lookahead->token == L_BRACE) {
		match(L_BRACE);
		statements();
		match(R_BRACE);
	} else {
		statement();
	}
	decrease_depth();
}
void SyntaxAnalyzer::return_statement() {
	pad("Return statement");
	increase_depth();
	match(RETURN_STATEMENT);
	expression();
	decrease_depth();
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
	pad("Function call");
	increase_depth();
	match(L_PARENTHESES);
	expression();
	match(R_PARENTHESES);
	decrease_depth();
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
	if (lookahead->token == ADDITION_OPERATOR) {
		match(ADDITION_OPERATOR);
		term();
	} else if (lookahead->token == SUBTRACTION_OPERATOR) {
		match(SUBTRACTION_OPERATOR);
		expression_p();
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
	if (lookahead->token == MULTIPLICATION_OPERATOR) {
		match(MULTIPLICATION_OPERATOR);
		factor();
		term_p();
	} else if (lookahead->token == DIVISION_OPERATOR) {
		match(DIVISION_OPERATOR);
		factor();
		term_p();
	}
	decrease_depth();
}
void SyntaxAnalyzer::factor() {
	pad("Factor");
	increase_depth();
	if (lookahead->token == IDENTIFIER) {
		identifier_prefix_statements();
	} else if (lookahead->token == NUMERIC_CONSTANT) {
		match(NUMERIC_CONSTANT);
	} else if (lookahead->token == L_PARENTHESES) {
		match(L_PARENTHESES);
		expression();
		match(R_PARENTHESES);
	} else if (lookahead->token == LITERAL_CONSTANT) {
		match(LITERAL_CONSTANT);
	} else {
		throw std::exception("Invalid expression");
	}
	decrease_depth();
}
void SyntaxAnalyzer::data_element() {
	if (lookahead->token == LITERAL_CONSTANT) {
		match(LITERAL_CONSTANT);
	} else {
		expression();
	}
}

void SyntaxAnalyzer::boolean_expression() {
	pad("Boolean expression");
	increase_depth();
	data_element();
	relational_operator();
	data_element();
	decrease_depth();
}

void SyntaxAnalyzer::relational_operator() {
	pad("Relational operator");
	increase_depth();
	match(RELATIONAL_OPERATOR);
	decrease_depth();
}
