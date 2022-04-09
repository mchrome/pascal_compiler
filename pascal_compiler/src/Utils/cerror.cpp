#include "cerror.h"

int CError::GetErrorLine()
{
	return this->errorLine;
}

int CError::GetErrorPos()
{
	return this->errorPos;
}


CError::CError(int _errorLine, int _errorPos)
{
	this->errorLine = _errorLine;
	this->errorPos = _errorPos;
}

CErrorSyntaxExpected::CErrorSyntaxExpected(CToken* _received, std::string _expected)
	: CError(_received->getLineNumber(), _received->getLinePosition())
{
	this->token.reset(_received);
	this->expected = _expected;
}

std::string CErrorSyntaxExpected::toString()
{
	return "\"" + this->expected + "\" was expected, but received" + this->token->toString() + "instead.";
}
