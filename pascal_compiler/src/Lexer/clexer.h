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
	std::shared_ptr<CToken> readDigit();
	std::shared_ptr<CToken> readIdentifier();
	std::shared_ptr<CToken> readString();
	std::shared_ptr<CLiteral> curLiteral;
	std::shared_ptr<CToken> strToKeywordToken(std::string tokenStr);
	std::shared_ptr<CLiteral> GetNextLiteral();
public:
	CLexer(CInput* _io);
	~CLexer();
	std::shared_ptr<CToken> NextToken();
};
