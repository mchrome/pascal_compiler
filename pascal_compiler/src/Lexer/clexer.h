#pragma once

#include "../Utils/cliteral.h"
#include "../Utils/ctoken.h"
#include "../Utils/ckeyword.h"
#include "../IO/cinputoutput.h"
#include <memory>


class CLexer
{
private:
	std::unique_ptr<CInput> cinput;
	std::unique_ptr<COutput> coutput;
	bool isLetter(char c);
	bool isDigit(char c);
	bool isIgnored(char c);
	char specialPrev;
	std::unique_ptr<CToken> readDigit();
	std::unique_ptr<CToken> readIdentifier();
	std::unique_ptr<CToken> readString();
	std::unique_ptr<CLiteral> curLiteral;
	std::unique_ptr<CToken> strToKeywordToken(std::string tokenStr);
public:
	CLexer(CInput* _io);
	~CLexer();
	std::unique_ptr<CToken> NextToken();
};
