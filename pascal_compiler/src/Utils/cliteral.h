#pragma once
class CLiteral {

public:
	int lineNumber, linePosition;
	char c;
	CLiteral(char _c, int _lineNumber, int _linePosition);
};