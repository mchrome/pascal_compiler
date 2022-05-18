#include "ctoken.h"

CToken::CToken(CTokenType _type, int _lineNumber, int _linePosition)
{
	this->tType = _type;
	this->lineNumber = _lineNumber;
	this->linePosition = _linePosition;
}

CTokenType CToken::getType()
{
	return this->tType;
}

std::string CToken::getTypeAsString()
{
	if (this->tType == CTokenType::ttConst) {
		return "[ttConst]";
	}
	if (this->tType == CTokenType::ttIdentifier) {
		return "[ttIdentifier]";
	}
	return "[ttKeyword]";
}

int CToken::getLineNumber()
{
	return this->lineNumber;
}

int CToken::getLinePosition()
{
	return this->linePosition;
}

CTokenIdentifier::CTokenIdentifier(CVariant* _name, int _lineNumber, int _linePosition) 
	: CToken(CTokenType::ttIdentifier, _lineNumber, _linePosition)
{
	this->name.reset(_name);
}

std::string CTokenIdentifier::toString()
{
	return this->name->toString();
}

std::string CTokenIdentifier::toStringWithType()
{
	return this->getTypeAsString() + " " + this->name->toString();
}

CTokenConst::CTokenConst(CVariant* _value, int _lineNumber, int _linePosition)
	: CToken(CTokenType::ttConst, _lineNumber, _linePosition)
{
	this->value.reset(_value);
}

CVariantType CTokenConst::getVariantType()
{
	return this->value->getVariantType();
}

std::string CTokenConst::toString()
{
	return this->value->toString();
}

std::string CTokenConst::toStringWithType()
{
	return this->getTypeAsString() + " " + this->value->toString();
}

CTokenKeyword::CTokenKeyword(CKeyword _kword, int _lineNumber, int _linePosition)
	: CToken(CTokenType::ttKeyword, _lineNumber, _linePosition)
{
	this->kword = _kword;
}

CKeyword CTokenKeyword::getKeyword()
{
	return this->kword;
}

std::string CTokenKeyword::toString()
{
	return c_keywordsToStr.at(kword);
}

std::string CTokenKeyword::toStringWithType()
{
	return this->getTypeAsString() + " " + c_keywordsToStr.at(kword);
}
