#ifndef __connector_h__
#define __connector_h__

#include "command.h"
using namespace std;

class connector
{
public:
	connector();
	virtual bool execen(bool, command *) = 0;
};

connector::connector() {};

class logicalor : public connector
{
public:
	bool execen(bool, command *);
};

class logicaland : public connector
{
public:
	bool execen(bool, command *);
};

class semicolon : public connector
{
public:
	bool execen(bool, command *);
};

bool logicalor::execen(bool left, command * right)
{
	return left || right->execute();
}

bool logicaland::execen(bool left, command * right)
{
	return left && right->execute();
}

bool semicolon::execen(bool left, command * right)
{
	return right->execute();
}

#endif