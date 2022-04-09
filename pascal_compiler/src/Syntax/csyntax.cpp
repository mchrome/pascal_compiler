#include "CSyntax.h"
#include "../Utils/cerror.h"

void CSyntax::Program() {
	this->GetNextToken();
	this->Identifier();
	if (curToken->toString() != ";") {
		CErrorSyntaxExpected(curToken.release(), ";").StdOutput();
	}
	this->GetNextToken();
	this->Block();
	if (curToken->toString() != ".") {
		CErrorSyntaxExpected(curToken.release(), ".").StdOutput();
	}
}

void CSyntax::Block() {
	this->TypeDeclarationPart();
	this->VarDeclarationPart();
	this->FuncionDeclarationPart();
	this->StatementPart();
}

void CSyntax::TypeDeclarationPart() {
	
	// Check if type declaration part is empty, if so exit
	if (this->curToken->toString() != "type") return;
	
	this->GetNextToken();

	// Check all type declarations
	while (curToken->getType() == CTokenType::ttIdentifier) {
		this->TypeDeclaration();
		if (this->curToken->toString() != ";") {
			CErrorSyntaxExpected(curToken.release(), ";").StdOutput();
		}
		this->GetNextToken();
	}
}

void CSyntax::TypeDeclaration() {
	this->Identifier();
	this->GetNextToken();
	if (curToken->toString() != "=") {
		CErrorSyntaxExpected(curToken.release(), "=").StdOutput();
	}
	this->GetNextToken();
	this->Type();
}

void CSyntax::Type() {
	
	if (curToken->toString() != "^") {
		this->Identifier();
	}
	else {
		this->PointerType();
	}
	
	this->GetNextToken();
	
}

void CSyntax::PointerType() {
	this->GetNextToken();
	this->Identifier();
}

void CSyntax::VarDeclarationPart() {
	// Check if var declaration part is empty, if so exit
	if (this->curToken->toString() != "var") return;

	this->GetNextToken();

	// Check all var declarations
	while (curToken->getType() == CTokenType::ttIdentifier) {
		this->VarDeclaration();
		if (this->curToken->toString() != ";") {
			CErrorSyntaxExpected(curToken.release(), ";").StdOutput();
		}
		this->GetNextToken();
	}
}

void CSyntax::VarDeclaration() {
	this->Identifier();
	this->GetNextToken();
	while (curToken->toString() == ",") {
		this->GetNextToken();
		this->Identifier();
		this->GetNextToken();
	}
	if (curToken->toString() != ":") {
		CErrorSyntaxExpected(curToken.release(), ":").StdOutput();
	}
	this->GetNextToken();
	this->Type();
}

void CSyntax::FuncionDeclarationPart() {
	while (this->curToken->toString() == "function") {
		this->FuncionDeclaration();
		if (this->curToken->toString() != ";") {
			CErrorSyntaxExpected(curToken.release(), ";").StdOutput();
		}
		this->GetNextToken();
	}
}

void CSyntax::FuncionDeclaration() {
	this->FunctionHeading();
	this->Block();
}

void CSyntax::FunctionHeading() {
	this->GetNextToken();
	this->Identifier();

	if (curToken->toString() == "(") {
		this->FormalParameterSection();
		while (curToken->toString() == ";") {
			this->GetNextToken();
			this->FormalParameterSection();
		}
	}

	if (curToken->toString() != ")") {
		CErrorSyntaxExpected(curToken.release(), ")").StdOutput();
	}

	if (curToken->toString() != ":") {
		CErrorSyntaxExpected(curToken.release(), ":").StdOutput();
	}
	this->GetNextToken();
	this->Type();
	if (this->curToken->toString() != ";") {
		CErrorSyntaxExpected(curToken.release(), ";").StdOutput();
	}
}

void CSyntax::FormalParameterSection() {

	if (curToken->getType() == CTokenType::ttIdentifier) {
		this->ParameterGroup();
	}

	if (curToken->toString() == "var") {
		this->GetNextToken();
		this->ParameterGroup();
	}

}

void CSyntax::ParameterGroup() {
	this->Identifier();
	while (curToken->toString() == ",") {
		this->GetNextToken();
		this->Identifier();
	}

	if (curToken->toString() != ":") {
		CErrorSyntaxExpected(curToken.release(), ":").StdOutput();
	}

	this->GetNextToken();
	this->Type();

}

void CSyntax::StatementPart() {
	if (curToken->toString() != "begin") {
		CErrorSyntaxExpected(curToken.release(), "begin").StdOutput();
	}
	this->GetNextToken();
	this->CompoundStatement();
	if (curToken->toString() != "end") {
		CErrorSyntaxExpected(curToken.release(), "end").StdOutput();
	}
	this->GetNextToken();
}

void CSyntax::CompoundStatement() {
	// Check if compound statement is empty
	if (curToken->toString() == "end") return;
	
	this->Statement();
	while (curToken->toString() == ";") {
		this->GetNextToken();
		this->Statement();
	}

}

void CSyntax::Statement() {
	if (curToken->getType() == CTokenType::ttIdentifier) {
		// Simple statement always starts with an identifier
		this->SimpleStatement();
	}
	else if (curToken->toString() == "if" || curToken->toString() == "while") {
		// Structured statement always starts with if or while
		this->StructuredStatement();
	}
}

void CSyntax::SimpleStatement() {
	this->Identifier();
	if (curToken->toString() == "^" || curToken->toString() == ":=")
	{
		this->AssignmentStatement();
	}
	else if (curToken->toString() == "(") {
		this->ProcedureStatement();
	}
	else {
		// TODO: Expected error
	}
}

void CSyntax::AssignmentStatement() {
	if (curToken->toString() == "^") {
		this->Variable();
	}
	if (curToken->toString() != ":=") {
		CErrorSyntaxExpected(curToken.release(), ":=").StdOutput();
	}
	this->GetNextToken();
	this->Expression();
}

void CSyntax::Variable() {
	while (curToken->toString() == "^") {
		this->GetNextToken();
	}
}

void CSyntax::Expression() {
	this->SimpleExpression();
	if (curToken->toString() == "<" ||
		curToken->toString() == ">" ||
		curToken->toString() == ">=" ||
		curToken->toString() == "<=" ||
		curToken->toString() == "=" ||
		curToken->toString() == "<>")
	{
		this->RelationalOperator();
		this->SimpleExpression();
	}
}

void CSyntax::RelationalOperator() {
	this->GetNextToken();
}

void CSyntax::SimpleExpression() {
	this->Term();
	while (curToken->toString() == "+" ||
		curToken->toString() == "-" ||
		curToken->toString() == "or") 
	{
		this->AddingOperator();
		this->Term();
	}
}

void CSyntax::AddingOperator() {
	this->GetNextToken();
}

void CSyntax::Term() {
	this->Factor();
	while (curToken->toString() == "*" ||
		curToken->toString() == "/" ||
		curToken->toString() == "and")
	{
		this->MultiplyingOperator();
		this->Factor();
	}
}

void CSyntax::MultiplyingOperator() {
	this->GetNextToken();
}

void CSyntax::Factor() {
	if (curToken->getType() == CTokenType::ttConst) {
		this->UnsignedConst();
	}
	else if (curToken->toString() == "+" ||
		curToken->toString() == "-" ||
		curToken->toString() == "not") 
	{
		this->UnaryOperator();
		this->Factor();
	}
	else if (curToken->toString() == "(") {
		this->GetNextToken();
		this->Expression();
		if (curToken->toString() != ")") {
			CErrorSyntaxExpected(curToken.release(), ")").StdOutput();
		}
		this->GetNextToken();
	}
	else if (curToken->getType() == CTokenType::ttIdentifier) {
		this->Identifier();
		if (curToken->toString() == "^") {
			this->Variable();
		}
		else if (curToken->toString() == "(") {
			this->FunctionDesignator();
		}
	}
}

void CSyntax::UnaryOperator() {
	this->GetNextToken();
}

void CSyntax::UnsignedConst() {
	if (curToken->toString()[0] >= '1' && curToken->toString()[0] <= '9') {
		this->UnsignedNumber();
	}
	else {
		// its a string const
		this->GetNextToken();
	}
}

void CSyntax::UnsignedNumber() {
	this->GetNextToken();
}

void CSyntax::FunctionDesignator() {
	this->GetNextToken();
	if (curToken->toString() == ")") {
		this->GetNextToken();
		return;
	}
	this->ActualParameter();
	while (curToken->toString() == ",") {
		this->GetNextToken();
		this->ActualParameter();
	}
	if (curToken->toString() != ")") {
		CErrorSyntaxExpected(curToken.release(), ")").StdOutput();
	}
	this->GetNextToken();
}

void CSyntax::ProcedureStatement() {
	this->GetNextToken();
	if (curToken->toString() == ")") {
		this->GetNextToken();
		return;
	}
	this->ActualParameter();
	while (curToken->toString() == ",") {
		this->GetNextToken();
		this->ActualParameter();
	}
	if (curToken->toString() != ")") {
		CErrorSyntaxExpected(curToken.release(), ")").StdOutput();
	}
	this->GetNextToken();
}

void CSyntax::ActualParameter() {
	this->Expression();
}


void CSyntax::StructuredStatement() {
	if (curToken->toString() == "if") {
		this->IfStatement();
	}
	else if (curToken->toString() == "while") {
		this->WhileStatement();
	}
}

void CSyntax::IfStatement() {
	this->GetNextToken();
	this->Expression();
	if (curToken->toString() != "then") {
		CErrorSyntaxExpected(curToken.release(), "then").StdOutput();
	}
	this->GetNextToken();
	this->Statement();
	if (curToken->toString() == "else") {
		this->GetNextToken();
		this->Statement();
	}
}

void CSyntax::WhileStatement() {
	this->GetNextToken();
	this->Expression();
	if (curToken->toString() != "do") {
		CErrorSyntaxExpected(curToken.release(), "do").StdOutput();
	}
	this->GetNextToken();
	this->Statement();
}

void CSyntax::Identifier() {
	if (this->curToken->getType() != CTokenType::ttIdentifier) {
		CErrorSyntaxExpected(curToken.release(), "An identifier").StdOutput();
	}
	this->GetNextToken();
}

void CSyntax::GetNextToken()
{
	this->curToken = std::move(this->lexer->NextToken());
}

CSyntax::CSyntax(CLexer* _lexer)
{
	this->lexer.reset(_lexer);
	this->GetNextToken();
}

void CSyntax::Run()
{
	if (curToken->toString() == "program") {
		this->Program();
	}
	else {
		this->Block();
		if (curToken->toString() != ".") {
			CErrorSyntaxExpected(curToken.release(), ".").StdOutput();
		}
	}
}