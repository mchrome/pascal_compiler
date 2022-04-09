#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "ctoken.h"
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