#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ctoken.h"
#include "ckeyword.h"
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
	virtual void StdOutput() PURE;
	virtual void FileOutput(std::string filePath) PURE;
};

class CErrorSyntaxExpected : public CError {
private:
	std::unique_ptr<CToken> token;
	std::string expected;
public:
	CErrorSyntaxExpected(CToken* _received, std::string _expected);
	std::string toString() override;
	void StdOutput() override;
	void FileOutput(std::string filePath) override;
};

class CErrorSyntaxExpectedKeyword : public CErrorSyntaxExpected {
public:
	CErrorSyntaxExpectedKeyword(CToken* _received, CKeyword _expected);
};

class CErrorSyntaxExpectedConst : public CErrorSyntaxExpected {
public:
	CErrorSyntaxExpectedConst(CToken* _received);
};