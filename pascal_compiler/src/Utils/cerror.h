#pragma once
#include <string>

class CError {
public:
	int errorLine;
	std::string errorMessage;
	CError(int line, std::string message);
};