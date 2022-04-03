#include "cerror.h"

CError::CError(int line, std::string message) {
	this->errorLine = line;
	this->errorMessage = message;
}