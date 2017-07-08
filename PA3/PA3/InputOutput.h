#pragma once

#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>

using namespace std;

extern int numAlphabets;
extern unsigned long m;

class InputOutput {
public:

	string inputGenome(char *inputPath);
	int alphabetParse(char *alphabetFile);
};

