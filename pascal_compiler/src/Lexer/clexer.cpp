#include "clexer.h"


bool CLexer::isLetter(char c)
{
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}

bool CLexer::isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool CLexer::isIgnored(char c)
{
	return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

std::unique_ptr<CToken> CLexer::readDigit()
{
	// read number const
	// if a dot is encountered number is considered to be a float
	// reading ends when whitespace is encountered
	// in initial state curLiteral must contain the first digit of the number

	bool isFloat = false;

	std::string tokenStr = "";

	while (
		!this->isIgnored(curLiteral->c) && 
		oneCharKeywords.count(curLiteral->c) == 0 && 
		oneCharSpecialKeywords.count(curLiteral->c) == 0
		||
		curLiteral->c == '.'
		) 
	{
		tokenStr += curLiteral->c;
		if (curLiteral->c == '.') {
			isFloat = true;
		}
		curLiteral = std::move(this->io->NextChar());
	}



	if (isFloat) {
		//std::unique_ptr<CVariant> v = std::make_unique<CVariantReal>(std::stod(tokenStr));
		return std::make_unique<CTokenConst>(new CVariantReal(std::stod(tokenStr)), curLiteral->lineNumber, curLiteral->linePosition);
	}
	else {
		//std::unique_ptr<CVariant> v = std::make_unique<CVariantInt>(std::stoi(tokenStr));
		return std::make_unique<CTokenConst>(new CVariantInt(std::stoi(tokenStr)), curLiteral->lineNumber, curLiteral->linePosition);
	}
	
}

std::unique_ptr<CToken> CLexer::readIdentifier()
{
	// read identifier or word keyword or bool
	// reading ends when current string is a keyword or bool
	// or when a whitespace or some keywords (arithmetic) are encountered
	// in initial state curLiteral must contain the first character of the identifier/keyword/bool

	std::string tokenStr = "";
	std::unique_ptr<CToken> res;


	while (
		!this->isIgnored(curLiteral->c) &&
		oneCharKeywords.count(curLiteral->c) == 0 &&
		oneCharSpecialKeywords.count(curLiteral->c) == 0
		)
	{
		tokenStr += curLiteral->c;
		curLiteral = std::move(this->io->NextChar());
		
		// only keywords made of letters can be found here
		if (c_strToKeywords.count(tokenStr) != 0) {
			// string is a keyword
			return strToKeywordToken(tokenStr);
		}

		if (tokenStr == "true" || tokenStr == "false") {

			//std::unique_ptr<CVariant> v = std::make_unique<CVariantBoolean>(tokenStr == "true");
			return std::make_unique<CTokenConst>(new CVariantBoolean(tokenStr == "true"), curLiteral->lineNumber, curLiteral->linePosition);
		}

	}
	res = std::make_unique<CTokenIdentifier>(new CVariantString(tokenStr), curLiteral->lineNumber, curLiteral->linePosition);
	

	return res;
}

std::unique_ptr<CToken> CLexer::readString()
{
	// read string const
	// reading ends when enclosing ' is encountered
	// in initial state curLiteral must contain the opening "'"

	// TODO: backslash symbols if necessarry here

	std::string tokenStr = "";

	// read first letter inside string const
	curLiteral = std::move(this->io->NextChar());
	
	while (curLiteral->c != '\'') {
		tokenStr += curLiteral->c;
		curLiteral = std::move(this->io->NextChar());
	}

	// skip closing '
	curLiteral = std::move(this->io->NextChar());


	return std::make_unique<CTokenConst>(new CVariantString(tokenStr), curLiteral->lineNumber, curLiteral->linePosition);;
}

std::unique_ptr<CToken> CLexer::strToKeywordToken(std::string tokenStr)
{
	return std::make_unique<CTokenKeyword>(c_strToKeywords.at(tokenStr), curLiteral->lineNumber, curLiteral->linePosition);
}

CLexer::CLexer(CInputOutput* _io)
{
	this->io.reset(_io);
	this->curLiteral = std::move(this->io->NextChar());
}

CLexer::~CLexer()
{
}

std::unique_ptr<CToken> CLexer::NextToken()
{
	


	while (true) {

		// skip whitespaces, newlines, tabs, etc.
		while (this->isIgnored(curLiteral->c)) {
			curLiteral = std::move(this->io->NextChar());
		}

		// skip comments
		while (curLiteral->c == '{') {
			while (curLiteral->c != '}') {
				curLiteral = std::move(this->io->NextChar());
			}

			// skip }
			curLiteral = std::move(this->io->NextChar());

			if (curLiteral->c == EOF) {
				// TODO: unmatched comment opening
				return nullptr;
			}

		}

		if (!this->isIgnored(curLiteral->c) && curLiteral->c != '{') {
			break;
		}

	}

	if (curLiteral->c == '}') {
		// TODO: unmatched comment ending
		return nullptr;
	}

	// reached end of file
	if (curLiteral->c == EOF) {
		return nullptr;
	}

	

	
	// one character keywords (<, >... excluded)
	if (oneCharKeywords.count(curLiteral->c) > 0) {

		std::string tokenStr(1, curLiteral->c);
		this->curLiteral = std::move(this->io->NextChar());
		return std::make_unique<CTokenKeyword>(c_strToKeywords.at(tokenStr), curLiteral->lineNumber, curLiteral->linePosition);
	}

	// <, > ...
	if (oneCharSpecialKeywords.count(curLiteral->c)) {
		this->specialPrev = curLiteral->c;

		// read new literal
		//std::unique_ptr<CLiteral> curLiteral = std::move(this->io->NextChar());
		curLiteral = std::move(this->io->NextChar());

		std::string tokenStr(1, this->specialPrev);

		if (this->specialPrev == '<') {
			// <=, <>
			if (curLiteral->c == '=' || curLiteral->c == '>') {
				tokenStr += curLiteral->c;
				curLiteral = std::move(this->io->NextChar());
				return this->strToKeywordToken(tokenStr);
			}
		}

		if (this->specialPrev == '>') {
			// >=
			if (curLiteral->c == '=') {
				tokenStr += curLiteral->c;
				curLiteral = std::move(this->io->NextChar());
				return this->strToKeywordToken(tokenStr);
			}
		}

		if (this->specialPrev == ':') {
			// :=
			if (curLiteral->c == '=') {
				tokenStr += curLiteral->c;
				curLiteral = std::move(this->io->NextChar());
				return this->strToKeywordToken(tokenStr);
			}
		}

		// return < or >, keep currentLiteral
		return this->strToKeywordToken(tokenStr);
	}
	
	if (this->isDigit(curLiteral->c)) {
		
		return this->readDigit();
	}

	if (this->isLetter(curLiteral->c)) {
		
		return this->readIdentifier();
	}

	if (curLiteral->c == '\'') {
		
		return this->readString();
	}


	return nullptr;

		
}


