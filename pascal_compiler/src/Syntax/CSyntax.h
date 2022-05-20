#pragma once
#include "../Lexer/clexer.h"
#include "../Utils/ckeyword.h"
#include "../Utils/cerror.h"
#include "../Utils/cvariant.h"
#include "../Utils/ctype.h"
#include "../Semantic/csemantic.h"
#include "../Utils/cparameters.h"
#include <memory>
#include <set>

class CSyntax
{
private:
	std::unique_ptr<COutput> coutput;
	std::unique_ptr<CLexer> lexer;
	std::unique_ptr<CSemantic> semantic;
	std::shared_ptr<CToken> curToken;
	std::set<CKeyword> expectedKeywords;
	void Program();
	void Block();
	void TypeDeclarationPart();
	void TypeDeclaration();
	void Type();
	void VarDeclarationPart();
	void VarDeclaration();
	void FunctionDeclarationPart();
	void FunctionDeclaration();
	void FunctionHeading();
	std::shared_ptr<CParameters> FormalParameterSection();
	std::shared_ptr<CParameters> ParameterGroup();
	void StatementPart();
	void CompoundStatement();
	void Statement();
	void SimpleStatement();
	void AssignmentStatement();
	void Variable();
	CBaseType Expression();
	void RelationalOperator();
	CBaseType SimpleExpression();
	void AddingOperator();
	CBaseType Term();
	void MultiplyingOperator();
	CBaseType Factor();
	void UnaryOperator();
	void UnsignedConst();
	void UnsignedNumber();
	std::vector<CBaseType> FunctionDesignator();
	CBaseType ActualParameter();
	void StructuredStatement();
	void IfStatement();
	void WhileStatement();
	void CaseStatement();
	void CaseListElement(CBaseType caseExprType);
	void Identifier();
	void GetNextToken();
	void AcceptKeyword(CKeyword kw);
	bool TryAcceptKeyword(CKeyword kw);
	bool CurTokenIsGivenKeyword(CKeyword kw);
	bool CurTokenIsNumberConst();
	bool CurTokenIsConst();
	void AcceptConst();
	void SkipTo(std::set<CKeyword> keywords);

public:
	CSyntax(CLexer* _lexer);
	void Run();
};