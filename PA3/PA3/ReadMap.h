#pragma once

#include "Node.h"
#include <iostream>

extern int nextIndex;
extern int* A;

class ReadMap
{
	void DFS_PrepareST(Node *T, int A[]);
	void createArray(int genomeLength);
	Node *ReadMap::FindLoc(Node *root, std::string read, int *maxDepth);
	std::string getSubString(int readLen, int leafIDfromA, int *startIndex, int *endIndex);

public:
	void PrepareST(Node* root);
	void MapReads(Node *root, std::string inputPath);
};