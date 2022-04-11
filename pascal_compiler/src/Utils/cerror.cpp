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
	return "CSyntax [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] " +
		"\"" + this->expected + "\" was expected, but received " + this->token->toStringWithType() + " instead.";
}

void CErrorSyntaxExpected::StdOutput()
{
	std::cout << this->toString() << std::endl;
}

void CErrorSyntaxExpected::FileOutput(std::string filePath)
{
	std::ofstream fout(filePath);
	fout << this->toString() << std::endl;
}

CErrorSyntaxExpectedKeyword::CErrorSyntaxExpectedKeyword(CToken* _received, CKeyword _expected) :
	CErrorSyntaxExpected(_received, c_keywordsToStr.at(_expected))
{
}
