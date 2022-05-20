#include "CSyntax.h"

void CSyntax::Program() {


	try {

		this->AcceptKeyword(CKeyword::programSy);
		this->Identifier();
		this->AcceptKeyword(CKeyword::semicolonSy);
		this->semantic->AddScope();
	}
	catch (std::shared_ptr<CError> e) {
		this->coutput->WriteErrorStd(e);
		this->SkipTo(std::set<CKeyword>{CKeyword::varSy, CKeyword::typeSy, CKeyword::functionSy, CKeyword::beginSy});
	}


	this->expectedKeywords = std::set<CKeyword>{ CKeyword::dotSy };
	try {
		this->Block();
		this->semantic->RemoveScope();
		this->AcceptKeyword(CKeyword::dotSy);
	}
	catch (std::shared_ptr<CError>& e) {
		this->coutput->WriteErrorStd(e);
	}
}

void CSyntax::Block() {
	this->TypeDeclarationPart();
	this->VarDeclarationPart();
	this->FunctionDeclarationPart();
	this->StatementPart();
}

void CSyntax::TypeDeclarationPart() {
	
	// Check if type declaration part is empty, if so exit
	if (!this->TryAcceptKeyword(CKeyword::typeSy)) return;

	try {
		this->TypeDeclaration();
		this->AcceptKeyword(CKeyword::semicolonSy);
		// Check all type declarations
		while (curToken->getType() == CTokenType::ttIdentifier) {
			this->TypeDeclaration();
			this->AcceptKeyword(CKeyword::semicolonSy);
		}
	}
	catch (std::shared_ptr<CError> e) {
		this->coutput->WriteErrorStd(e);
		this->SkipTo(std::set<CKeyword>{ CKeyword::varSy, CKeyword::functionSy, CKeyword::beginSy });
	}
}

void CSyntax::TypeDeclaration() {
	auto newType = curToken;
	this->Identifier();
	this->AcceptKeyword(CKeyword::equalSy);
	auto existingType = curToken;
	this->Type();
	this->semantic->AddType(newType, existingType);
}

void CSyntax::Type() {
	this->Identifier();
}

void CSyntax::VarDeclarationPart() {
	// Check if var declaration part is empty, if so exit
	if (!this->TryAcceptKeyword(CKeyword::varSy)) return;

	try {
		this->VarDeclaration();
		this->AcceptKeyword(CKeyword::semicolonSy);
		// Check all var declarations
		while (curToken->getType() == CTokenType::ttIdentifier) {
			this->VarDeclaration();
			this->AcceptKeyword(CKeyword::semicolonSy);
		}
	}
	catch (std::shared_ptr<CError> e) {
		this->coutput->WriteErrorStd(e);
		this->SkipTo(std::set<CKeyword>{ CKeyword::functionSy, CKeyword::beginSy });
	}
}

void CSyntax::VarDeclaration() {
	std::vector<std::shared_ptr<CToken>> vars;
	vars.push_back(curToken);
	this->Identifier();
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		vars.push_back(curToken);
		this->Identifier();
	}
	this->AcceptKeyword(CKeyword::colonSy);
	auto type = curToken;
	this->Type();
	for (int i = 0; i < vars.size(); i++) {
		this->semantic->AddIdentifier(vars[i], type);
	}
}

void CSyntax::FunctionDeclarationPart() {
	try {
		while (this->CurTokenIsGivenKeyword(CKeyword::functionSy)) {
			this->FunctionDeclaration();
			this->AcceptKeyword(CKeyword::semicolonSy);
		}
	}
	catch (std::shared_ptr<CError> e) {
		this->coutput->WriteErrorStd(e);
		this->SkipTo(std::set<CKeyword>{ CKeyword::beginSy });
	}
}

void CSyntax::FunctionDeclaration() {
	this->FunctionHeading();
	this->Block();
	this->semantic->RemoveScope();
}

void CSyntax::FunctionHeading() {
	this->AcceptKeyword(CKeyword::functionSy);
	auto function = curToken;
	this->Identifier();

	auto parameters = std::make_shared<CParameters>();

	if (this->TryAcceptKeyword(CKeyword::leftParSy)) {

		parameters->AddParameters(this->FormalParameterSection());
		while (this->TryAcceptKeyword(CKeyword::semicolonSy)) {
			parameters->AddParameters(this->FormalParameterSection());
		}
	}
	this->AcceptKeyword(CKeyword::rightParSy);
	this->AcceptKeyword(CKeyword::colonSy);
	auto functionType = curToken;
	this->Type();
	this->AcceptKeyword(CKeyword::semicolonSy);
	this->semantic->AddFunction(function, functionType, parameters->types);
	this->semantic->AddScope();
	this->semantic->AddIdentifier(function, functionType);
	for (int i = 0; i < parameters->names.size(); i++) {
		this->semantic->AddIdentifier(parameters->names[i], parameters->types[i]);
	}

}

std::shared_ptr<CParameters> CSyntax::FormalParameterSection() {

	if (curToken->getType() == CTokenType::ttIdentifier) {
		return this->ParameterGroup();
	}

	else if (this->TryAcceptKeyword(CKeyword::varSy)) {
		return this->ParameterGroup();
	}

}

std::shared_ptr<CParameters> CSyntax::ParameterGroup() {
	auto res = std::make_shared<CParameters>();
	int cnt = 1;
	auto paramName = curToken;
	this->Identifier();
	res->names.push_back(paramName);
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		auto paramName = curToken;
		this->Identifier();
		res->names.push_back(paramName);
		cnt++;
	}

	this->AcceptKeyword(CKeyword::colonSy);
	auto type = curToken;
	this->Type();
	res->types = std::vector<std::shared_ptr<CToken>>(cnt, type);
	
	return res;
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
	try {
		if (curToken->getType() == CTokenType::ttIdentifier) {
			// Simple statement always starts with an identifier
			this->SimpleStatement();
		}
		else if (this->CurTokenIsGivenKeyword(CKeyword::ifSy)
			|| this->CurTokenIsGivenKeyword(CKeyword::whileSy)
			|| this->CurTokenIsGivenKeyword(CKeyword::caseSy)) {
			// Structured statement always starts with if, while or case
			this->StructuredStatement();
		}
	}
	catch (std::shared_ptr<CError> e) {
		this->coutput->WriteErrorStd(e);
		this->SkipTo(std::set<CKeyword>{ CKeyword::semicolonSy });
	}
}

void CSyntax::SimpleStatement() {
	this->AssignmentStatement();
	
}

void CSyntax::AssignmentStatement() {
	auto varName = curToken;
	this->semantic->EnsureIdentifierExistsGlobally(varName);
	this->Variable();
	this->AcceptKeyword(CKeyword::assignSy);
	CBaseType assignmentBaseType = this->Expression();
	this->semantic->EnsureIdentifierTypeMatches(varName, assignmentBaseType);
}

void CSyntax::Variable() {
	this->Identifier();
}

CBaseType CSyntax::Expression() {
	CBaseType curBaseType = this->SimpleExpression();
	while (this->CurTokenIsGivenKeyword(CKeyword::lessSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::greaterSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::greaterEqualSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::lessEqualSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::equalSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::notEqualSy))
	{
		auto opToken = curToken;
		
		this->RelationalOperator();
		CBaseType incomingBaseType = this->SimpleExpression();

		this->semantic->EnsureCanTypeCast(curBaseType, incomingBaseType, opToken);
		curBaseType = CBaseType::cTypeBoolean;
	}
	return curBaseType;
}

void CSyntax::RelationalOperator() {
	this->GetNextToken();
}

CBaseType CSyntax::SimpleExpression() {
	CBaseType curBaseType = this->Term();
	while (this->CurTokenIsGivenKeyword(CKeyword::plusSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::minusSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::orSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::xorSy))
	{


		auto opToken = curToken;
		this->AddingOperator();
		CBaseType incomingBaseType = this->Term();

		this->semantic->EnsureCanTypeCast(curBaseType, incomingBaseType, opToken);

		if (incomingBaseType == CBaseType::cTypeReal) {
			curBaseType = CBaseType::cTypeReal;
		}
	}
	return curBaseType;
}

void CSyntax::AddingOperator() {
	this->GetNextToken();
}

CBaseType CSyntax::Term() {
	CBaseType curBaseType = this->Factor();
	while (this->CurTokenIsGivenKeyword(CKeyword::multiplySy) ||
		this->CurTokenIsGivenKeyword(CKeyword::divisionSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::andSy))
	{
		
		auto opToken = curToken;

		this->MultiplyingOperator();
		CBaseType incomingBaseType = this->Factor();

		this->semantic->EnsureCanTypeCast(curBaseType, incomingBaseType, opToken);

		if (incomingBaseType == CBaseType::cTypeReal) {
			curBaseType = CBaseType::cTypeReal;
		}
		else if (incomingBaseType == CBaseType::cTypeInt) {
			curBaseType = CBaseType::cTypeInt;
		}
	}
	return curBaseType;
}

void CSyntax::MultiplyingOperator() {
	this->GetNextToken();
}

CBaseType CSyntax::Factor() {
	if (curToken->getType() == CTokenType::ttConst) {
		auto constTokenRef = std::dynamic_pointer_cast<CTokenConst>(curToken);
		this->UnsignedConst();
		return this->semantic->GetBaseTypeOfConstToken(constTokenRef);
	}
	else if (this->CurTokenIsGivenKeyword(CKeyword::plusSy) ||
		this->CurTokenIsGivenKeyword(CKeyword::minusSy))
	{
		auto opToken = curToken;
		this->UnaryOperator();

		CBaseType incomingBaseType = this->Factor();

		this->semantic->EnsureCanTypeCast(incomingBaseType, opToken);

		
		return incomingBaseType;
	}
	else if (this->CurTokenIsGivenKeyword(CKeyword::notSy)) {
		auto opToken = curToken;
		this->UnaryOperator();

		CBaseType incomingBaseType = this->Factor();

		this->semantic->EnsureCanTypeCast(incomingBaseType, opToken);

		return CBaseType::cTypeBoolean;
	}
	else if (this->TryAcceptKeyword(CKeyword::leftParSy)) {
		auto exprType = this->Expression();
		this->AcceptKeyword(CKeyword::rightParSy);
		return exprType;
	}
	else if (curToken->getType() == CTokenType::ttIdentifier) {
		auto identifierName = curToken;
		this->semantic->EnsureIdentifierExistsGlobally(identifierName);
		this->Identifier();
		if (this->CurTokenIsGivenKeyword(CKeyword::leftParSy)) {
			auto funcParameterTypes = this->FunctionDesignator();
			this->semantic->EnsureFunctionParametersMatch(identifierName, funcParameterTypes);
		}
		return semantic->GetIdentifierBaseType(identifierName->toString());
	}
	else {
		throw std::static_pointer_cast<CError>(std::make_shared<CErrorSyntaxUnexpectedKeyword>(curToken));
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

std::vector<CBaseType> CSyntax::FunctionDesignator() {
	this->AcceptKeyword(CKeyword::leftParSy);
	if (this->TryAcceptKeyword(CKeyword::rightParSy)) {
		return std::vector<CBaseType>();
	}
	std::vector<CBaseType> callTypes;
	callTypes.push_back(this->ActualParameter());
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		callTypes.push_back(this->ActualParameter());
	}
	this->AcceptKeyword(CKeyword::rightParSy);
	return callTypes;
}

CBaseType CSyntax::ActualParameter() {
	return this->Expression();
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
	auto statementKeyword = curToken;
	this->AcceptKeyword(CKeyword::ifSy);
	auto exprType = this->Expression();
	this->semantic->EnsureExpressionIsBool(statementKeyword, exprType, CBaseType::cTypeBoolean);
	this->AcceptKeyword(CKeyword::thenSy);
	this->Statement();
	
	if (this->TryAcceptKeyword(CKeyword::elseSy)) {
		this->Statement();
	}

}

void CSyntax::WhileStatement() {
	auto statementKeyword = curToken;
	this->AcceptKeyword(CKeyword::whileSy);
	auto exprType = this->Expression();
	this->semantic->EnsureExpressionIsBool(statementKeyword, exprType, CBaseType::cTypeBoolean);
	this->AcceptKeyword(CKeyword::doSy);
	this->Statement();
}

void CSyntax::CaseStatement()
{
	this->AcceptKeyword(CKeyword::caseSy);
	try {
		auto exprType = this->Expression();
		this->AcceptKeyword(CKeyword::ofSy);
		this->CaseListElement(exprType);
		while (this->TryAcceptKeyword(CKeyword::semicolonSy)) {
			this->CaseListElement(exprType);
		}
	}
	catch (std::shared_ptr<CError> e) {
		this->coutput->WriteErrorStd(e);
		this->SkipTo(std::set<CKeyword>{CKeyword::endSy});
	}
	this->AcceptKeyword(CKeyword::endSy);
}

void CSyntax::CaseListElement(CBaseType caseExprType)
{
	// Check if CaseListElement is empty, if so exit
	if (this->CurTokenIsGivenKeyword(CKeyword::endSy)) return;


	auto constToken = curToken;
	this->UnsignedConst();
	this->semantic->EnsureConstTypeMatches(constToken, caseExprType);
	while (this->TryAcceptKeyword(CKeyword::commaSy)) {
		auto constToken = curToken;
		this->UnsignedConst();
		this->semantic->EnsureConstTypeMatches(constToken, caseExprType);
	}
	this->AcceptKeyword(CKeyword::colonSy);
	this->Statement();
	
}

void CSyntax::Identifier() {
	if (this->curToken->getType() != CTokenType::ttIdentifier) {
		throw std::static_pointer_cast<CError>(std::make_shared<CErrorSyntaxExpectedIdentifier>(curToken, "An identifier"));
	}
	this->GetNextToken();
}

void CSyntax::GetNextToken()
{
	this->curToken=this->lexer->NextToken();
}

void CSyntax::AcceptKeyword(CKeyword kw)
{
	std::shared_ptr<CTokenKeyword> tempDerived = std::dynamic_pointer_cast<CTokenKeyword>(curToken);
	if (tempDerived == nullptr || tempDerived->getKeyword() != kw) {
		throw std::static_pointer_cast<CError>(std::make_shared<CErrorSyntaxExpectedKeyword>(curToken, kw));
	}
	this->GetNextToken();
}

bool CSyntax::TryAcceptKeyword(CKeyword kw)
{
	std::shared_ptr<CTokenKeyword> tempDerived = std::dynamic_pointer_cast<CTokenKeyword>(curToken);
	if (tempDerived == nullptr) return false;
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
	std::shared_ptr<CTokenKeyword> tempDerived = std::dynamic_pointer_cast<CTokenKeyword>(curToken);
	if (tempDerived == nullptr) return false;
	return tempDerived->getKeyword() == kw;
}

bool CSyntax::CurTokenIsNumberConst()
{
	std::shared_ptr<CTokenConst> tempDerived = std::dynamic_pointer_cast<CTokenConst>(curToken);
	if (tempDerived == nullptr) return false;
	return tempDerived->getVariantType() == CVariantType::vtInt || tempDerived->getVariantType() == CVariantType::vtReal;
}

bool CSyntax::CurTokenIsConst()
{
	return curToken->getType() == CTokenType::ttConst;
}

void CSyntax::AcceptConst()
{
	if (!this->CurTokenIsConst()) {
		throw std::static_pointer_cast<CError>(std::make_shared<CErrorSyntaxExpectedConst>(curToken));
	}
	this->GetNextToken();
}

void CSyntax::SkipTo(std::set<CKeyword> keywords)
{
	auto tKeyword = std::dynamic_pointer_cast<CTokenKeyword>(curToken);
	while (tKeyword == nullptr || (tKeyword->getKeyword() != CKeyword::eofSy && keywords.count(tKeyword->getKeyword()) == 0)) {
		this->GetNextToken();
		tKeyword = std::dynamic_pointer_cast<CTokenKeyword>(curToken);
	}
}



CSyntax::CSyntax(CLexer* _lexer)
{
	this->coutput = std::make_unique<COutput>("compiler_syntax_output.txt");
	this->lexer.reset(_lexer);
	this->semantic = std::make_unique<CSemantic>();
	this->semantic->AddScope();
	this->semantic->AddBaseType("integer", CBaseType::cTypeInt);
	this->semantic->AddBaseType("boolean", CBaseType::cTypeBoolean);
	this->semantic->AddBaseType("real", CBaseType::cTypeReal);
	this->semantic->AddBaseType("string", CBaseType::cTypeString);
	this->semantic->AddBaseIdentifier("true", "boolean");
	this->semantic->AddBaseIdentifier("false", "boolean");
	
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