#pragma once
#include"Node.h"

class Memory
{
public:
	void freeMemory(Node *node);
	void detectFreeMemory();
};