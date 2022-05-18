#pragma once
#include "cvariant.h"
#define PURE = 0
#include <string>
#include <memory>
#include "ckeyword.h"

enum class CTokenType {
	ttIdentifier,
	ttKeyword,
	ttConst
};

class CToken {
private:
	CTokenType tType;
	int lineNumber;
	int linePosition;
public:
	CToken(CTokenType _type, int _lineNumber, int _linePosition);
	CTokenType getType();
	std::string getTypeAsString();
	int getLineNumber();
	int getLinePosition();
	virtual std::string toString() PURE;
	virtual std::string toStringWithType() PURE;
};

class CTokenIdentifier : public CToken {
private:
	std::unique_ptr<CVariant> name;
public:
	CTokenIdentifier(CVariant* _name, int _lineNumber, int _linePosition);
	std::string toString() override;
	std::string toStringWithType() override;
};

class CTokenKeyword : public CToken {
private:
	CKeyword kword;
public:
	CTokenKeyword(CKeyword _kword, int _lineNumber, int _linePosition);
	CKeyword getKeyword();
	std::string toString() override;
	std::string toStringWithType() override;
};

class CTokenConst : public CToken {
private:
	std::unique_ptr<CVariant> value;
public:
	CTokenConst(CVariant* _value, int _lineNumber, int _linePosition);
	CVariantType getVariantType();
	std::string toString() override;
	std::string toStringWithType() override;
};