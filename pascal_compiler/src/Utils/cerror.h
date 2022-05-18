#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ctoken.h"
#include "ckeyword.h"
#include "ctype.h"
#define PURE = 0

class CError {
private:
	int errorLine;
	int errorPos;
public:
	int GetErrorLine();
	int GetErrorPos();
	CError(int _errorLine, int _errorPos);
	virtual std::string toString() PURE;
	void StdOutput();
};

class CErrorSyntaxExpected : public CError {
private:
	std::shared_ptr<CToken> token;
	std::string expected;
public:
	CErrorSyntaxExpected(std::shared_ptr<CToken> _received, std::string _expected);
	std::string toString() override;
};

class CErrorSyntaxExpectedKeyword : public CErrorSyntaxExpected {
public:
	CErrorSyntaxExpectedKeyword(std::shared_ptr<CToken> _received, CKeyword _expected);
};

class CErrorSyntaxExpectedConst : public CErrorSyntaxExpected {
public:
	CErrorSyntaxExpectedConst(std::shared_ptr<CToken> _received);
};

class CErrorSemantic : public CError {
protected:
	std::shared_ptr<CToken> token;
public:
	CErrorSemantic(std::shared_ptr<CToken> token);
	std::string toString() override;
};

class CErrorSemanticAlreadyDefined : public CErrorSemantic {
public:
	CErrorSemanticAlreadyDefined(std::shared_ptr<CToken> token);
	std::string toString() override;
};

class CErrorSemanticNotDefined : public CErrorSemantic {
public:
	CErrorSemanticNotDefined(std::shared_ptr<CToken> token);
	std::string toString() override;
};

class CErrorSemanticTypeMismatch : public CErrorSemantic {
private:
	std::string received;
	std::string expected;

public:
	CErrorSemanticTypeMismatch(std::shared_ptr<CToken> token, CBaseType received, CBaseType expected);
	std::string toString() override;
};

class CErrorSemanticWrongExprType : public CErrorSemantic {
public:
	CErrorSemanticWrongExprType(std::shared_ptr<CToken> token);
	std::string toString() override;
};

class CErrorSemanticIncorrectParameters : public CErrorSemantic {
public:
	CErrorSemanticIncorrectParameters(std::shared_ptr<CToken> token);
	std::string toString() override;
};

class CErrorSemanticWrongConstType : public CErrorSemantic {
private:
	std::string received;
	std::string expected;

public:
	CErrorSemanticWrongConstType(std::shared_ptr<CToken> token, CBaseType received, CBaseType expected);
	std::string toString() override;
};