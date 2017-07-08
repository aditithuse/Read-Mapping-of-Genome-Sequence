
#pragma once

#include "node.h"
#include <string>

extern int geneLen;
extern char *geneBuffer;

class SuffixTree {

public:
	Node *root;
	int leafCount = 1;
	int maxDepth = 0;
	int internalNodeCount = 0;
	int sCompareCount = 0;
	int addChildToNode(Node *parent, Node *child);
	Node *lowestNode;   //For longest Repeat
	int alphaNum;

	//the following functions are of type node as we will be returning the newly added node

	Node *createNode(int id, Node *parent, int beginLabel, int endLabel);
	Node *insertSuffix(int suffixNum, Node *root, Node *leaf);
	Node *nodeHop(Node *v_prime, int startIndex);
	Node *establishSuffixLink(Node *v_prime, Node *u, int betaBegin, int betaEnd, int suffix);
	Node *findPath(Node *v, int suffixNum);
	Node *matchChild(Node *node, int suffixNum, int *index);
	Node *breakEdge(Node *node, int startIndex, int childIndex);

	Node *suffixTree(std::string gene, int numAlphabets);
};