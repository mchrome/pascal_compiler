#include "cerror.h"

int CError::GetErrorLine()
{
	return this->errorLine;
}

int CError::GetErrorPos()
{
	return this->errorPos;
}

std::string CError::GetErrorMessage()
{
	return this->errorMessage;
}

CError::CError(std::string message, int _errorLine, int _errorPos)
{
	this->errorMessage = message;
	this->errorLine = _errorLine;
	this->errorPos = _errorPos;
}
