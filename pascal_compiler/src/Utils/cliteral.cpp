#include "cliteral.h"

CLiteral::CLiteral(char _c, int _lineNumber, int _linePosition)
{
	this->c = _c;
	this->lineNumber = _lineNumber;
	this->linePosition = _linePosition;
}
