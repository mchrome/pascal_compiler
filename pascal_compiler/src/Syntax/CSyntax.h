#pragma once
#include "../Lexer/clexer.h"
#include "../Utils/ckeyword.h"
#include "../Utils/cerror.h"
#include "../Utils/cvariant.h"
#include <memory>

class CSyntax
{
private:
	std::unique_ptr<CLexer> lexer;
	std::unique_ptr<CToken> curToken;
	void Program();
	void Block();
	void TypeDeclarationPart();
	void TypeDeclaration();
	void Type();
	void PointerType();
	void VarDeclarationPart();
	void VarDeclaration();
	void FuncionDeclarationPart();
	void FuncionDeclaration();
	void FunctionHeading();
	void FormalParameterSection();
	void ParameterGroup();
	void StatementPart();
	void CompoundStatement();
	void Statement();
	void SimpleStatement();
	void AssignmentStatement();
	void Variable();
	void Expression();
	void RelationalOperator();
	void SimpleExpression();
	void AddingOperator();
	void Term();
	void MultiplyingOperator();
	void Factor();
	void UnaryOperator();
	void UnsignedConst();
	void UnsignedNumber();
	void FunctionDesignator();
	void ProcedureStatement();
	void ActualParameter();
	void StructuredStatement();
	void IfStatement();
	void WhileStatement();
	void CaseStatement();
	void CaseListElement();
	void Identifier();
	void GetNextToken();
	void AcceptKeyword(CKeyword kw);
	bool TryAcceptKeyword(CKeyword kw);
	bool CurTokenIsGivenKeyword(CKeyword kw);
	bool CurTokenIsNumberConst();
	bool CurTokenIsConst();
	void AcceptConst();
public:
	CSyntax(CLexer* _lexer);
	void Run();
};