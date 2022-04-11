#pragma once
#include<map>
#include<string>
#include<set>

enum class CKeyword : char {
	programSy,		// program 
	beginSy,		// begin 
	endSy,			// end 
	varSy,			// var 
	integerSy,		// integer
	realSy,			// real
	stringSy,		// string
	booleanSy,		// boolean
	typeSy,			// type 
	functionSy,		// function 
	ifSy,			// if 
	thenSy,			// then 
	elseSy,			// else 
	whileSy,		// while
	caseSy,			// case
	ofSy,			// of
	doSy,			// do 
	andSy,			// and 
	orSy,			// or 
	xorSy,			// xor 
	notSy,			// not 
	addressSy,		// @ 
	pointerSy,		// ^
	plusSy,			// +
	minusSy,		// - 
	multiplySy,		// *
	divisionSy,		// /
	leftParSy,		// (
	rightParSy,		// )
	assignSy,		// :=
	dotSy,			// .
	commaSy,		// ,
	colonSy,		// :
	semicolonSy,	// ;
	equalSy,		// =
	lessSy,			// <
	greaterSy,		// >
	lessEqualSy,	// <=
	greaterEqualSy,	// >=
	notEqualSy,		// <>
};

const std::map<std::string, CKeyword> c_strToKeywords = {
	{"program", CKeyword::programSy},
	{"begin", CKeyword::beginSy},
	{"end", CKeyword::endSy},
	{"var", CKeyword::varSy},
	{"integer", CKeyword::integerSy},
	{"real", CKeyword::realSy},
	{"string", CKeyword::stringSy},
	{"boolean", CKeyword::booleanSy},
	{"type", CKeyword::typeSy},
	{"function", CKeyword::functionSy},
	{"if", CKeyword::ifSy},
	{"then", CKeyword::thenSy},
	{"else", CKeyword::elseSy},
	{"while", CKeyword::whileSy},
	{"case", CKeyword::caseSy},
	{"of", CKeyword::ofSy},
	{"do", CKeyword::doSy},
	{"and", CKeyword::andSy},
	{"or", CKeyword::orSy},
	{"xor", CKeyword::xorSy},
	{"not", CKeyword::notSy},
	{"@", CKeyword::addressSy},
	{"^", CKeyword::pointerSy},
	{"+", CKeyword::plusSy},
	{"-", CKeyword::minusSy},
	{"*", CKeyword::multiplySy},
	{"/", CKeyword::divisionSy},
	{"(", CKeyword::leftParSy},
	{")", CKeyword::rightParSy},
	{":=", CKeyword::assignSy},
	{".", CKeyword::dotSy},
	{",", CKeyword::commaSy},
	{":", CKeyword::colonSy},
	{";", CKeyword::semicolonSy},
	{"=", CKeyword::equalSy},
	{"<", CKeyword::lessSy},
	{">", CKeyword::greaterSy},
	{"<=", CKeyword::lessEqualSy},
	{">=", CKeyword::greaterEqualSy},
	{"<>", CKeyword::notEqualSy},
};

const std::map<CKeyword, std::string> c_keywordsToStr = {
	{CKeyword::programSy, "program"},
	{CKeyword::beginSy, "begin"},
	{CKeyword::endSy, "end"},
	{CKeyword::varSy, "var"},
	{CKeyword::integerSy, "integer"},
	{CKeyword::realSy, "real"},
	{CKeyword::stringSy, "string"},
	{CKeyword::booleanSy, "boolean"},
	{CKeyword::typeSy, "type"},
	{CKeyword::functionSy, "function"},
	{CKeyword::ifSy, "if"},
	{CKeyword::thenSy, "then"},
	{CKeyword::elseSy, "else"},
	{CKeyword::whileSy, "while"},
	{CKeyword::caseSy, "case"},
	{CKeyword::ofSy, "of"},
	{CKeyword::doSy, "do"},
	{CKeyword::andSy, "and"},
	{CKeyword::orSy, "or"},
	{CKeyword::xorSy, "xor"},
	{CKeyword::notSy, "not"},
	{CKeyword::addressSy, "@"},
	{CKeyword::pointerSy, "^"},
	{CKeyword::plusSy, "+"},
	{CKeyword::minusSy, "-"},
	{CKeyword::multiplySy, "*"},
	{CKeyword::divisionSy, "/"},
	{CKeyword::leftParSy, "("},
	{CKeyword::rightParSy, ")"},
	{CKeyword::assignSy, ":="},
	{CKeyword::dotSy, "."},
	{CKeyword::commaSy, ","},
	{CKeyword::colonSy, ":"},
	{CKeyword::semicolonSy, ";"},
	{CKeyword::equalSy, "="},
	{CKeyword::lessSy, "<"},
	{CKeyword::greaterSy, ">"},
	{CKeyword::lessEqualSy, "<="},
	{CKeyword::greaterEqualSy, ">=" },
	{CKeyword::notEqualSy, "<>"},
};

const std::set<char> oneCharKeywords = {
	'@',
	'^',
	'+',
	'-',
	'*',
	'/',
	'(',
	')',
	'{',
	'}',
	'.',
	',',
	';',
	'='
};

const std::set<char> oneCharSpecialKeywords = {
	'<',
	'>',
	':'
};