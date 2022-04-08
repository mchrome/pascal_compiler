#include "CSyntax.h"


CSyntax::CSyntax(CLexer* _lexer)
{
	this->lexer.reset(_lexer);
	this->curToken = std::move(this->lexer->NextToken());
}

void CSyntax::Run()
{

}
