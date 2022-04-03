#include "cvariant.h"

CVariant::CVariant(VariantType _variantType)
{
	this->variantType = _variantType;
}

VariantType CVariant::getVariantType()
{
	return this->variantType;
}

CVariantInt::CVariantInt(int _value) : CVariant(VariantType::vtInt)
{
	this->value = _value;
}

std::string CVariantInt::toString()
{
	return std::to_string(this->value);
}

int CVariantInt::getValue()
{
	return this->value;
}

CVariantReal::CVariantReal(double _value) : CVariant(VariantType::vtReal)
{
	this->value = _value;
}

std::string CVariantReal::toString()
{
	return std::to_string(this->value);
}

double CVariantReal::getValue()
{
	return this->value;
}

CVariantString::CVariantString(std::string _value) : CVariant(VariantType::vtString)
{
	this->value = _value;
}

std::string CVariantString::toString()
{
	return this->value;
}

std::string CVariantString::getValue()
{
	return this->value;
}

CVariantBoolean::CVariantBoolean(bool _value) : CVariant(VariantType::vtBoolean)
{
	this->value = _value;
}

std::string CVariantBoolean::toString()
{
	return std::to_string(this->value);
}

bool CVariantBoolean::getValue()
{
	return this->value;
}
