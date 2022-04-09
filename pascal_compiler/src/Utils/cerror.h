#pragma once
#include <string>

class CError {
private:
	int errorLine;
	int errorPos;
	std::string errorMessage;
public:
	int GetErrorLine();
	int GetErrorPos();
	std::string GetErrorMessage();
	CError(std::string message, int _errorLine, int _errorPos);
};