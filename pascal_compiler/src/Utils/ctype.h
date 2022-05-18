#pragma once
#include <string>
enum class CBaseType : char {
	cTypeInt,
	cTypeReal,
	cTypeBoolean,
	cTypeString,
	cTypeUndefined
};

const std::map<CBaseType, std::string> c_basetypeToStr = {
	{CBaseType::cTypeInt, "integer"},
	{CBaseType::cTypeReal, "real"},
	{CBaseType::cTypeBoolean, "boolean"},
	{CBaseType::cTypeString, "string"},
	{CBaseType::cTypeUndefined, "undefined"}
};