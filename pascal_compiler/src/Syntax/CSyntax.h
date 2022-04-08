#pragma once
#include "../Lexer/clexer.h"
#include <memory>

class CSyntax
{
private:
	std::unique_ptr<CLexer> lexer;
	std::unique_ptr<CToken> curToken;
public:
	CSyntax(CLexer* _lexer);
	void Run();
};