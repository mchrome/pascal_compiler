#pragma once
#include "../Utils/cscope.h"
#include "../Utils/cerror.h"
#include "../IO//cinputoutput.h"
#include "../Utils/ctoken.h"

class CSemantic {
private:
	std::shared_ptr<CScope> current_scope;
	std::unique_ptr<COutput> coutput;
public:
	CSemantic();
	void AddScope();
	void RemoveScope();
	void AddIdentifier(std::shared_ptr<CToken> identifier, std::shared_ptr<CToken> type);
	void AddType(std::shared_ptr<CToken> identifier, std::shared_ptr<CToken> type);
	void AddBaseType(std::string identifier, CBaseType type);
	void AddBaseIdentifier(std::string identifier, std::string type);
	void AddFunction(std::shared_ptr<CToken> identifier, std::shared_ptr<CToken> type, std::vector<std::shared_ptr<CToken>> parameterTypes);
	CBaseType GetTypeBaseType(std::string type);
	CBaseType GetIdentifierBaseType(std::string identifier);
	bool IdentifierExistsLocally(std::shared_ptr<CToken> identifier);
	bool TypeExistsLocally(std::shared_ptr<CToken> identifier);
	bool IdentifierExistsGlobally(std::string identifier);
	void EnsureIdentifierExistsGlobally(std::shared_ptr<CToken> identifier);
	bool TypeExistsGlobally(std::string identifier);
	bool IdentifierTypeMatches(std::shared_ptr<CToken> identifier, CBaseType type);
	void EnsureIdentifierTypeMatches(std::shared_ptr<CToken> identifier, CBaseType type);
	bool FunctionParametersMatch(std::string identifier, std::vector<CBaseType> types);
	void EnsureFunctionParametersMatch(std::shared_ptr<CToken> identifier, std::vector<CBaseType> types);
	void EnsureExpressionIsBool(std::shared_ptr<CToken> statementKeyword, CBaseType exprType, CBaseType expected);
	bool ConstTypeMatches(std::shared_ptr<CToken> constToken, CBaseType exprType);
	void EnsureConstTypeMatches(std::shared_ptr<CToken> constToken, CBaseType exprType);
	bool CanTypeCast(CBaseType left, CBaseType right, std::shared_ptr<CToken> token);
	bool CanTypeCast(CBaseType type, std::shared_ptr<CToken> token);
	void EnsureCanTypeCast(CBaseType type, std::shared_ptr<CToken> token);
	void EnsureCanTypeCast(CBaseType left, CBaseType right, std::shared_ptr<CToken> op);
	CBaseType GetBaseTypeOfConstToken(std::shared_ptr<CTokenConst> token);
};