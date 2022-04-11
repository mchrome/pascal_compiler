#include "CSyntax.h"


void CSyntax::Program() {
	this->AcceptKeyword(CKeyword::programSy);
	this->Identifier();
	this->AcceptKeyword(CKeyword::semicolonSy);
	this->Block();
	this->AcceptKeyword(CKeyword::dotSy);
}

void CSyntax::Block() {
	this->TypeDeclarationPart();
	this->VarDeclarationPart();
	this->FuncionDeclarationPart();
	this->StatementPart();
}

void CSyntax::TypeDeclarationPart() {
	
	// Check if type declaration part is empty, if so exit
	if (!this->TryAcceptKeyword(CKeyword::typeSy)) return;

	// Check all type declarations
	while (curToken->getType() == CTokenType::ttIdentifier) {
		this->TypeDeclaration();
		this->AcceptKeyword(CKeyword::semicolonSy);
	}
}

void CSyntax::TypeDeclaration() {
	this->Identifier();
	this->AcceptKeyword(CKeyword::equalSy);
	this->Type();
}

void CSyntax::Type() {
	
	if (this->CurTokenIsGivenKeyword(CKeyword::pointerSy)) {
		this->Identifier();
	}
	else {
		this->PointerType();
	}
}

void CSyntax::PointerType() {
	this->AcceptKeyword(CKeyword::pointerSy);
	this->Identifier();
}

void CSyntax::VarDeclarationPart() {
	// Check if var declaration part is empty, if so exit
	if (!this->TryAcceptKeyword(CKeyword::varSy)) return;

	// Check all var declarations
	while (curToken->getType() == CTokenType::ttIdentifier) {
		this->VarDeclaration();
		this->AcceptKeyword(CKeyword::semicolonSy);
	}
}

void CSyntax::VarDeclaration() {
	this->Identifier();
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		this->Identifier();
	}
	this->AcceptKeyword(CKeyword::colonSy);
	this->Type();
}

void CSyntax::FuncionDeclarationPart() {
	while (this->CurTokenIsGivenKeyword(CKeyword::functionSy)) {
		this->FuncionDeclaration();
		this->AcceptKeyword(CKeyword::semicolonSy);
	}
}

void CSyntax::FuncionDeclaration() {
	this->FunctionHeading();
	this->Block();
}

void CSyntax::FunctionHeading() {
	this->AcceptKeyword(CKeyword::functionSy);
	this->Identifier();

	if (this->TryAcceptKeyword(CKeyword::leftParSy)) {
		this->FormalParameterSection();
		while (this->TryAcceptKeyword(CKeyword::semicolonSy)) {
			this->FormalParameterSection();
		}
	}
	this->AcceptKeyword(CKeyword::rightParSy);
	this->AcceptKeyword(CKeyword::colonSy);
	this->Type();
	this->AcceptKeyword(CKeyword::semicolonSy);
}

void CSyntax::FormalParameterSection() {

	if (curToken->getType() == CTokenType::ttIdentifier) {
		this->ParameterGroup();
	}

	else if (this->TryAcceptKeyword(CKeyword::varSy)) {
		this->ParameterGroup();
	}

}

void CSyntax::ParameterGroup() {
	this->Identifier();
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		this->Identifier();
	}

	this->AcceptKeyword(CKeyword::colonSy);
	this->Type();

}

void CSyntax::StatementPart() {
	this->AcceptKeyword(CKeyword::beginSy);
	this->CompoundStatement();
	this->AcceptKeyword(CKeyword::endSy);
}

void CSyntax::CompoundStatement() {
	// Check if compound statement is empty
	if (this->CurTokenIsGivenKeyword(CKeyword::endSy)) return;
	
	this->Statement();
	while (this->TryAcceptKeyword(CKeyword::semicolonSy)) {
		this->Statement();
	}

}

void CSyntax::Statement() {
	if (curToken->getType() == CTokenType::ttIdentifier) {
		// Simple statement always starts with an identifier
		this->SimpleStatement();
	}
	else if (this->CurTokenIsGivenKeyword(CKeyword::ifSy) 
		|| this->CurTokenIsGivenKeyword(CKeyword::whileSy)
		|| this->CurTokenIsGivenKeyword(CKeyword::caseSy)) {
		// Structured statement always starts with if or while
		this->StructuredStatement();
	}
}

void CSyntax::SimpleStatement() {
	this->Identifier();
	if (this->CurTokenIsGivenKeyword(CKeyword::pointerSy) || this->CurTokenIsGivenKeyword(CKeyword::assignSy))
	{
		this->AssignmentStatement();
	}
	else if (this->CurTokenIsGivenKeyword(CKeyword::leftParSy)) {
		this->ProcedureStatement();
	}
	else {
		// TODO: Expected error
	}
}

void CSyntax::AssignmentStatement() {
	if (this->CurTokenIsGivenKeyword(CKeyword::pointerSy)) {
		this->Variable();
	}
	this->AcceptKeyword(CKeyword::assignSy);
	this->Expression();
}

void CSyntax::Variable() {
	while (this->TryAcceptKeyword(CKeyword::pointerSy)) {}
}

void CSyntax::Expression() {
	this->SimpleExpression();
	while (this->CurTokenIsGivenKeyword(CKeyword::lessSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::greaterSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::greaterEqualSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::lessEqualSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::equalSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::notEqualSy))
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
	while (this->CurTokenIsGivenKeyword(CKeyword::plusSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::minusSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::orSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::xorSy))
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
	while (this->CurTokenIsGivenKeyword(CKeyword::multiplySy) ||
		this->CurTokenIsGivenKeyword(CKeyword::divisionSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::andSy))
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
	else if (this->CurTokenIsGivenKeyword(CKeyword::plusSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::minusSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::notSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::addressSy))
	{
		this->UnaryOperator();
		this->Factor();
	}
	else if (this->TryAcceptKeyword(CKeyword::leftParSy)) {
		this->Expression();
		this->AcceptKeyword(CKeyword::rightParSy);
	}
	else if (curToken->getType() == CTokenType::ttIdentifier) {
		this->Identifier();
		if (this->CurTokenIsGivenKeyword(CKeyword::pointerSy)) {
			this->Variable();
		}
		else if (this->CurTokenIsGivenKeyword(CKeyword::leftParSy)) {
			this->FunctionDesignator();
		}
	}
}

void CSyntax::UnaryOperator() {
	this->GetNextToken();
}

void CSyntax::UnsignedConst() {
	if (this->CurTokenIsNumberConst()) {
		this->UnsignedNumber();
	}
	else {
		// its a string const
		this->AcceptConst();
	}
}

void CSyntax::UnsignedNumber() {
	this->AcceptConst();
}

void CSyntax::FunctionDesignator() {
	this->AcceptKeyword(CKeyword::leftParSy);
	if (this->TryAcceptKeyword(CKeyword::rightParSy)) {
		return;
	}
	this->ActualParameter();
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		this->ActualParameter();
	}
	this->AcceptKeyword(CKeyword::rightParSy);
}

void CSyntax::ProcedureStatement() {
	this->AcceptKeyword(CKeyword::leftParSy);
	if (this->TryAcceptKeyword(CKeyword::rightParSy)) {
		return;
	}
	this->ActualParameter();
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		this->ActualParameter();
	}
	this->AcceptKeyword(CKeyword::rightParSy);
}

void CSyntax::ActualParameter() {
	this->Expression();
}


void CSyntax::StructuredStatement() {
	if (this->CurTokenIsGivenKeyword(CKeyword::ifSy)) {
		this->IfStatement();
	}
	else if (this->CurTokenIsGivenKeyword(CKeyword::whileSy)) {
		this->WhileStatement();
	}
	else if (this->CurTokenIsGivenKeyword(CKeyword::caseSy)) {
		this->CaseStatement();
	}
}

void CSyntax::IfStatement() {
	this->AcceptKeyword(CKeyword::ifSy);
	this->Expression();
	this->AcceptKeyword(CKeyword::thenSy);
	if (this->CurTokenIsGivenKeyword(CKeyword::beginSy)) {
		this->StatementPart();
	}
	else {
		this->Statement();
	}
	
	if (this->TryAcceptKeyword(CKeyword::elseSy)) {

		if (this->CurTokenIsGivenKeyword(CKeyword::beginSy)) {
			this->StatementPart();
		}
		else {
			this->Statement();
		}
	}

}

void CSyntax::WhileStatement() {
	this->AcceptKeyword(CKeyword::whileSy);
	this->Expression();
	this->AcceptKeyword(CKeyword::doSy);
	if (this->CurTokenIsGivenKeyword(CKeyword::beginSy)) {
		this->StatementPart();
	}
	else {
		this->Statement();
	}
}

void CSyntax::CaseStatement()
{
	this->AcceptKeyword(CKeyword::caseSy);
	this->Expression();
	this->AcceptKeyword(CKeyword::ofSy);
	this->CaseListElement();
	while (this->TryAcceptKeyword(CKeyword::semicolonSy)) {
		this->CaseListElement();
	}
	this->AcceptKeyword(CKeyword::endSy);
}

void CSyntax::CaseListElement()
{
	// Check if CaseListElement is empty, if so exit
	if (this->CurTokenIsGivenKeyword(CKeyword::endSy)) return;

	this->UnsignedConst();
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		this->UnsignedConst();
	}
	this->AcceptKeyword(CKeyword::colonSy);
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

void CSyntax::AcceptKeyword(CKeyword kw)
{
	CTokenKeyword* tempDerived = static_cast<CTokenKeyword*>(curToken.get());
	if (tempDerived->getKeyword() != kw) {
		CErrorSyntaxExpectedKeyword(curToken.release(), kw).StdOutput();
	}
	this->GetNextToken();
}

bool CSyntax::TryAcceptKeyword(CKeyword kw)
{
	CTokenKeyword* tempDerived = static_cast<CTokenKeyword*>(curToken.get());
	if (tempDerived->getKeyword() != kw) {
		return false;
	}
	else {
		this->GetNextToken();
		return true;
	}
}

bool CSyntax::CurTokenIsGivenKeyword(CKeyword kw)
{
	CTokenKeyword* tempDerived = static_cast<CTokenKeyword*>(curToken.get());
	return tempDerived->getKeyword() == kw;
}

bool CSyntax::CurTokenIsNumberConst()
{
	CTokenConst* tempDerived = static_cast<CTokenConst*>(curToken.get());
	return tempDerived->getVariantType() == VariantType::vtInt || tempDerived->getVariantType() == VariantType::vtReal;
}

bool CSyntax::CurTokenIsConst()
{
	return dynamic_cast<CTokenConst*>(curToken.get()) != nullptr;
}

void CSyntax::AcceptConst()
{
	if (!this->CurTokenIsConst()) {
		CErrorSyntaxExpectedConst(curToken.release()).StdOutput();
	}
	this->GetNextToken();
}



CSyntax::CSyntax(CLexer* _lexer)
{
	this->lexer.reset(_lexer);
	this->GetNextToken();
}

void CSyntax::Run()
{
	if (this->CurTokenIsGivenKeyword(CKeyword::programSy)) {
		this->Program();
	}
	else {
		this->Block();
		this->AcceptKeyword(CKeyword::dotSy);
	}
}