#pragma once
#include <string>

#define PURE = 0

enum class CVariantType {
	vtInt,
	vtReal,
	vtString,
	vtBoolean
};

class CVariant {
private:
	CVariantType variantType;
public:
	CVariant(CVariantType _variantType);
	CVariantType getVariantType();
	virtual std::string toString() PURE;
};

class CVariantInt : public CVariant {
private:
	int value;
public:
	CVariantInt(int _value);
	std::string toString() override;
	int getValue();
};

class CVariantReal : public CVariant {
private:
	double value;
public:
	CVariantReal(double _value);
	std::string toString() override;
	double getValue();
};

class CVariantString : public CVariant {
private:
	std::string value;
public:
	CVariantString(std::string _value);
	std::string toString() override;
	std::string getValue();
};

class CVariantBoolean : public CVariant {
private:
	bool value;
public:
	CVariantBoolean(bool _value);
	std::string toString() override;
	bool getValue();
};