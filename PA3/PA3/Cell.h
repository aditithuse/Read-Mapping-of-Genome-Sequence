
#pragma once

#include<iostream>
#include <string>	//getline
#include <ctype.h>

#ifndef CELL_H
#define CELL_H

using namespace std;
struct Cell
{
	float S;
	float D;
	float I;

public:

	int substitution(char a, char b, int m_a, int m_i)
	{
		if (a == b)
			return m_a;
		else return m_i;
	}

};
#endif