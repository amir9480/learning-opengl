#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "helpers.h"
#include "Node.h";

class Light : public Node
{
public:
	enum Type {
		Directional,
		Point
	};

public:
	Light(Type _type);

	virtual std::string getClass() const;

protected:
	Type mType;
};

#endif // _LIGHT_H_