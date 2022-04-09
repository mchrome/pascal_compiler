#include "CSyntax.h"

void CSyntax::Program() {
	this->GetNextToken();
	this->Identifier();
	if (curToken->toString() != ";") {
		// TODO: Expected error
	}
	this->GetNextToken();
	this->Block();
	if (curToken->toString() != ".") {
		// TODO: Expected error
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
			// TODO: Expected error
		}
		this->GetNextToken();
	}
}

void CSyntax::TypeDeclaration() {
	this->Identifier();
	this->GetNextToken();
	if (curToken->toString() != "=") {
		// TODO: Expected error
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
			// TODO: Expected error
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
		// TODO: Expected error
	}
	this->GetNextToken();
	this->Type();
}

void CSyntax::FuncionDeclarationPart() {
	while (this->curToken->toString() == "function") {
		this->FuncionDeclaration();
		if (this->curToken->toString() != ";") {
			// TODO: Expected error
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
		// TODO: Expected error
	}

	if (curToken->toString() != ":") {
		// TODO: Expected error
	}
	this->GetNextToken();
	this->Type();
	if (this->curToken->toString() != ";") {
		// TODO: Expected error
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
		// TODO: Expected error
	}

	this->GetNextToken();
	this->Type();

}

void CSyntax::StatementPart() {

}

void CSyntax::CompoundStatement() {

}

void CSyntax::Statement() {

}

void CSyntax::SimpleStatement() {

}

void CSyntax::AssignmentStatement() {

}

void CSyntax::Variable() {

}

void CSyntax::Expression() {

}

void CSyntax::RelationalOperator() {

}

void CSyntax::SimpleExpression() {

}

void CSyntax::AddingOperator() {

}

void CSyntax::Term() {

}

void CSyntax::MultiplyingOperator() {

}

void CSyntax::Factor() {

}

void CSyntax::UnaryOperator() {

}

void CSyntax::UnsignedConst() {

}

void CSyntax::UnsignedNumber() {

}

void CSyntax::FunctionDesignator() {

}

void CSyntax::ProcedureStatement() {

}

void CSyntax::ActualParameter() {

}

void CSyntax::Empty() {

}

void CSyntax::StructuredStatement() {

}

void CSyntax::IfStatement() {

}

void CSyntax::WhileStatement() {

}

void CSyntax::Identifier() {
	if (this->curToken->getType() != CTokenType::ttIdentifier) {
		// TODO: Expected error
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
			// TODO: Expected "." keyword error
		}
	}
}