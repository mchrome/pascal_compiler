#include "cerror.h"

int CError::GetErrorLine()
{
	return this->errorLine;
}

int CError::GetErrorPos()
{
	return this->errorPos;
}


CError::CError(int _errorLine, int _errorPos)
{
	this->errorLine = _errorLine;
	this->errorPos = _errorPos;
}

void CError::StdOutput()
{
	std::cout << this->toString() << std::endl;
}

CErrorSyntaxExpectedIdentifier::CErrorSyntaxExpectedIdentifier(std::shared_ptr<CToken> _received, std::string _expected)
	: CError(_received->getLineNumber(), _received->getLinePosition())
{
	this->token = _received;
	this->expected = _expected;
}

std::string CErrorSyntaxExpectedIdentifier::toString()
{
	return "CSyntax [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] " +
		"\"" + this->expected + "\" was expected, but received " + this->token->toStringWithType() + " instead.";
}


CErrorSyntaxExpectedKeyword::CErrorSyntaxExpectedKeyword(std::shared_ptr<CToken> _received, CKeyword _expected) :
	CErrorSyntaxExpectedIdentifier(_received, c_keywordsToStr.at(_expected))
{
}

CErrorSyntaxExpectedConst::CErrorSyntaxExpectedConst(std::shared_ptr<CToken> _received)
	: CErrorSyntaxExpectedIdentifier(_received, "A constant")
{
}

CErrorSemantic::CErrorSemantic(std::shared_ptr<CToken> token) : 
	CError(token->getLineNumber(), token->getLinePosition())
{
	this->token = token;
}

std::string CErrorSemantic::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] "
		+ this->token->toStringWithType() + " caused a semantic error.";
	
}

CErrorSemanticAlreadyDefined::CErrorSemanticAlreadyDefined(std::shared_ptr<CToken> token) : CErrorSemantic(token)
{
}

std::string CErrorSemanticAlreadyDefined::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] "
		+ this->token->toStringWithType() + " is already defined in a local scope.";
}

CErrorSemanticNotDefined::CErrorSemanticNotDefined(std::shared_ptr<CToken> token) : CErrorSemantic(token)
{
}

std::string CErrorSemanticNotDefined::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] "
		+ this->token->toStringWithType() + " is not defined";
}


CErrorSemanticTypeMismatch::CErrorSemanticTypeMismatch(std::shared_ptr<CToken> token, CBaseType received, CBaseType expected) : CErrorSemantic(token)
{
	this->received = c_basetypeToStr.at(received);
	this->expected = c_basetypeToStr.at(expected);
}

std::string CErrorSemanticTypeMismatch::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] "
		+ this->token->toStringWithType() + " was assigned a " + this->received + " but its type is " + this->expected;
}

CErrorSemanticWrongExprType::CErrorSemanticWrongExprType(std::shared_ptr<CToken> token) : CErrorSemantic(token)
{
}

std::string CErrorSemanticWrongExprType::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Expression type for "
		+ this->token->toString() + " statement must be boolean.";
}

CErrorSemanticIncorrectParameters::CErrorSemanticIncorrectParameters(std::shared_ptr<CToken> token) : CErrorSemantic(token)
{
}

std::string CErrorSemanticIncorrectParameters::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Parameters for function call of "
		+ this->token->toStringWithType() + " do not match the definition.";
}

CErrorSemanticWrongConstType::CErrorSemanticWrongConstType(std::shared_ptr<CToken> token, CBaseType received, CBaseType expected) : CErrorSemantic(token)
{
	this->received = c_basetypeToStr.at(received);
	this->expected = c_basetypeToStr.at(expected);
}

std::string CErrorSemanticWrongConstType::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Constant "
		+ this->token->toStringWithType() + " has type " + this->received + " but type " + this->expected + " was expected.";
}

CErorLexerUnmatchedCommentOpening::CErorLexerUnmatchedCommentOpening(std::shared_ptr<CLiteral> literal) : CErrorLexer(literal)
{
}

std::string CErorLexerUnmatchedCommentOpening::toString()
{
	return "CLexer [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Unmatched comment opening.";
}

CErrorLexer::CErrorLexer(std::shared_ptr<CLiteral> literal) : CError(literal->lineNumber, literal->linePosition)
{
	this->literal = literal;
}

CErorLexerUnmatchedCommentEnding::CErorLexerUnmatchedCommentEnding(std::shared_ptr<CLiteral> literal):CErrorLexer(literal)
{
}

std::string CErorLexerUnmatchedCommentEnding::toString()
{
	return "CLexer [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Unmatched comment ending.";
}

CErrorSemanticCantTypeCast::CErrorSemanticCantTypeCast(std::shared_ptr<CToken> token) : CErrorSemantic(token)
{
}

std::string CErrorSemanticCantTypeCast::toString()
{
	return "CSemantic [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Can't perform a type cast.";
}

CErrorSyntaxUnexpectedKeyword::CErrorSyntaxUnexpectedKeyword(std::shared_ptr<CToken> token) 
	: CError(token->getLineNumber(), token->getLinePosition())
{
	this->token = token;
}

std::string CErrorSyntaxUnexpectedKeyword::toString()
{
	return "CSyntax [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Recieved an unexpected keyword " + 
		this->token->toStringWithType();
}

CErrorLexerIncorrectNumber::CErrorLexerIncorrectNumber(std::shared_ptr<CLiteral> literal) : CErrorLexer(literal) {

}

std::string CErrorLexerIncorrectNumber::toString()
{
	return "CLexer [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Incorrect number.";
}

CErrorLexerUnknownLiteral::CErrorLexerUnknownLiteral(std::shared_ptr<CLiteral> literal) : CErrorLexer(literal)
{
}

std::string CErrorLexerUnknownLiteral::toString()
{
	return "CLexer [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Unknown literal.";
}

CErrorLexerUnmatchedStringQuotes::CErrorLexerUnmatchedStringQuotes(std::shared_ptr<CLiteral> literal) : CErrorLexer(literal)
{
}

std::string CErrorLexerUnmatchedStringQuotes::toString()
{
	return "CLexer [" + std::to_string(this->GetErrorLine()) + ":" + std::to_string(this->GetErrorPos()) + "] Unmatched string quotes.";
}
