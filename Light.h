#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "helpers.h"
#include "Node.h";

class Light : public Node
{
public:

	virtual std::string getClass() const override;
};

#endif // _LIGHT_H_