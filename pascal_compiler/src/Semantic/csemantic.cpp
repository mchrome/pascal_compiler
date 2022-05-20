#include "csemantic.h"

CSemantic::CSemantic()
{
	this->current_scope = nullptr;
}

void CSemantic::AddScope()
{
	this->current_scope = std::make_shared<CScope>(this->current_scope);
}

void CSemantic::RemoveScope()
{
	this->current_scope = this->current_scope->GetParentScope();
}

void CSemantic::AddIdentifier(std::shared_ptr<CToken> identifier, std::shared_ptr<CToken> type)
{
	if (this->IdentifierExistsLocally(identifier)) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticAlreadyDefined>(identifier));
		return;
	}
	if (!this->TypeExistsGlobally(type->toString())) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticNotDefined>(type));
		return;
	}
	current_scope->identifiers[identifier->toString()] = type->toString();
}

void CSemantic::AddType(std::shared_ptr<CToken> identifier, std::shared_ptr<CToken> type)
{

	if (this->IdentifierExistsLocally(identifier)) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticAlreadyDefined>(identifier));
		return;
	}
	if (!this->TypeExistsGlobally(type->toString())) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticNotDefined>(type));
		return;
	}
	current_scope->identifiers[identifier->toString()] = type->toString();
	current_scope->types[identifier->toString()] = this->GetTypeBaseType(type->toString());
}

void CSemantic::AddBaseType(std::string identifier, CBaseType type)
{
	current_scope->types[identifier] = type;
}

void CSemantic::AddBaseIdentifier(std::string identifier, std::string type)
{
	current_scope->identifiers[identifier] = type;
}

void CSemantic::AddFunction(std::shared_ptr<CToken> identifier, std::shared_ptr<CToken> type, std::vector<std::shared_ptr<CToken>> parameterTypes)
{
	std::vector<CBaseType> parameterBaseTypes;
	this->AddIdentifier(identifier, type);
	for (int i = 0; i < parameterTypes.size(); i++) {
		if (!this->TypeExistsGlobally(parameterTypes[i]->toString())) {
			// TODO: error TypeNotFound
			return;
		}
		parameterBaseTypes.push_back(this->GetTypeBaseType(parameterTypes[i]->toString()));
	}
	current_scope->functionParameters[identifier->toString()]=parameterBaseTypes;
}



CBaseType CSemantic::GetTypeBaseType(std::string type)
{
	auto scope = current_scope;
	while (scope!=nullptr) {
		if (scope->types.count(type)) {
			return scope->types[type];
		}
		scope = scope->GetParentScope();
	}
	return CBaseType::cTypeUndefined;
}

CBaseType CSemantic::GetIdentifierBaseType(std::string identifier)
{
	auto scope = current_scope;
	while (scope != nullptr) {
		if (scope->identifiers.count(identifier)) {
			return GetTypeBaseType(scope->identifiers[identifier]);
		}
		scope = scope->GetParentScope();
	}
	return CBaseType::cTypeUndefined;
}

bool CSemantic::IdentifierExistsLocally(std::shared_ptr<CToken> identifier)
{
	return current_scope->identifiers.count(identifier->toString());
}

bool CSemantic::TypeExistsLocally(std::shared_ptr<CToken> identifier)
{
	return current_scope->types.count(identifier->toString());
}

bool CSemantic::IdentifierExistsGlobally(std::string identifier)
{
	auto scope = current_scope;
	while (scope != nullptr) {
		if (scope->identifiers.count(identifier)) {
			return true;
		}
		scope = scope->GetParentScope();
	}
	return false;
}

void CSemantic::EnsureIdentifierExistsGlobally(std::shared_ptr<CToken> identifier)
{
	if (!IdentifierExistsGlobally(identifier->toString())) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticNotDefined>(identifier));
	}
}

bool CSemantic::TypeExistsGlobally(std::string identifier)
{
	auto scope = current_scope;
	while (scope != nullptr) {
		if (scope->types.count(identifier)) {
			return true;
		}
		scope = scope->GetParentScope();
	}
	return false;
}

bool CSemantic::IdentifierTypeMatches(std::shared_ptr<CToken> identifier, CBaseType type)
{
	auto identTokenBaseType = GetIdentifierBaseType(identifier->toString());

	return identTokenBaseType == type || (identTokenBaseType == CBaseType::cTypeReal && type == CBaseType::cTypeInt);
}

void CSemantic::EnsureIdentifierTypeMatches(std::shared_ptr<CToken> identifier, CBaseType type)
{
	if (!IdentifierTypeMatches(identifier, type)) {
		this->coutput->WriteErrorStd(
			std::make_shared<CErrorSemanticTypeMismatch>(
				identifier, 
				type,
				this->GetIdentifierBaseType(identifier->toString())
				)
		);
	}
}

bool CSemantic::FunctionParametersMatch(std::string identifier, std::vector<CBaseType> types)
{
	std::vector<CBaseType> parameters;
	auto scope = current_scope;
	while (scope != nullptr) {
		if (scope->functionParameters.count(identifier)) {
			parameters = scope->functionParameters[identifier];
		}
		scope = scope->GetParentScope();
	}


	if (parameters.size() != types.size()) return false;
	for (int i = 0; i < parameters.size(); i++) {
		if (parameters[i] != types[i]) {
			return false;
		}
	}
	return true;
}

void CSemantic::EnsureFunctionParametersMatch(std::shared_ptr<CToken> identifier, std::vector<CBaseType> types)
{
	if (!FunctionParametersMatch(identifier->toString(), types)) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticIncorrectParameters>(identifier));
	}
}

void CSemantic::EnsureExpressionIsBool(std::shared_ptr<CToken> statementKeyword, CBaseType exprType, CBaseType expected)
{
	if (exprType != expected) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticWrongExprType>(statementKeyword));
	}
}

bool CSemantic::ConstTypeMatches(std::shared_ptr<CToken> token, CBaseType exprType)
{
	auto constToken = std::dynamic_pointer_cast<CTokenConst>(token);
	if (constToken == nullptr) return false;

	auto constTokenBaseType = GetBaseTypeOfConstToken(constToken);

	return constTokenBaseType == exprType || (constTokenBaseType == CBaseType::cTypeReal && exprType == CBaseType::cTypeInt);
}

void CSemantic::EnsureConstTypeMatches(std::shared_ptr<CToken> constToken, CBaseType exprType)
{
	if (!ConstTypeMatches(constToken, exprType)) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticWrongConstType>(constToken,
			GetBaseTypeOfConstToken(std::dynamic_pointer_cast<CTokenConst>(constToken)),
			exprType
			)
		);
	}
}

bool CSemantic::CanTypeCast(CBaseType left, CBaseType right, std::shared_ptr<CToken> token)
{
	auto tokenKw = std::dynamic_pointer_cast<CTokenKeyword>(token);
	if (tokenKw == nullptr) return false;
	CKeyword op = tokenKw->getKeyword();


	if ((op == CKeyword::andSy || op == CKeyword::orSy || op == CKeyword::xorSy) && (left != CBaseType::cTypeBoolean || right!= CBaseType::cTypeBoolean)) {
		return false;
	}
	else if (left == CBaseType::cTypeString || right == CBaseType::cTypeString) {
		return false;
	}
	else if ((op == CKeyword::plusSy || op == CKeyword::minusSy) && !(left == CBaseType::cTypeInt || left == CBaseType::cTypeReal)
		&& !(right == CBaseType::cTypeInt || right == CBaseType::cTypeReal))
		return false;
	return true;
}

bool CSemantic::CanTypeCast(CBaseType type, std::shared_ptr<CToken> token)
{
	auto tokenKw = std::dynamic_pointer_cast<CTokenKeyword>(token);
	if (tokenKw == nullptr) return false;
	CKeyword op = tokenKw->getKeyword();

	if (op == CKeyword::notSy && type != CBaseType::cTypeBoolean)
		return false;
	else if ((op == CKeyword::plusSy || op == CKeyword::minusSy) && (type != CBaseType::cTypeInt && type != CBaseType::cTypeReal))
		return false;
	return true;
}

void CSemantic::EnsureCanTypeCast(CBaseType type, std::shared_ptr<CToken> token)
{
	if (!CanTypeCast(type, token)) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticCantTypeCast>(token));
	}
}

void CSemantic::EnsureCanTypeCast(CBaseType left, CBaseType right, std::shared_ptr<CToken> token)
{
	if (!CanTypeCast(left, right, token)) {
		this->coutput->WriteErrorStd(std::make_shared<CErrorSemanticCantTypeCast>(token));
	}
}

CBaseType CSemantic::GetBaseTypeOfConstToken(std::shared_ptr<CTokenConst> token)
{
	if (token->getVariantType() == CVariantType::vtBoolean) {
		return CBaseType::cTypeBoolean;
	}
	else if (token->getVariantType() == CVariantType::vtInt) {
		return CBaseType::cTypeInt;
	}
	else if (token->getVariantType() == CVariantType::vtReal) {
		return CBaseType::cTypeReal;
	}
	else {
		return CBaseType::cTypeString;
	}
}
