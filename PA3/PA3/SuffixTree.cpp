#include "Node.h"
#include "InputOutput.h"
#include "SuffixTree.h"
#include <algorithm>
#include<Windows.h>
#include<iostream>

char *geneBuffer;
int geneLen;

Node* SuffixTree::createNode(int id, Node *parent, int beginLabel, int endLabel) {

	Node *newNode = new Node();
	newNode->id = id;
	newNode->beginLabel = beginLabel;
	newNode->endLabel = endLabel;
	newNode->suffixLink = NULL;
	newNode->numChild = 0;
	newNode->visited = false;
	newNode->child = (Node**)calloc(alphaNum + 1, sizeof(Node*));
	newNode->start_leaf_index = -1;
	newNode->end_leaf_index = -1;

	if (parent == NULL) {
		newNode->parent = newNode;
	}
	else {
		newNode->parent = parent;
	}

	if (parent != NULL) {
		newNode->stringDepth = parent->stringDepth + (endLabel - beginLabel) + 1;
	}
	else
		newNode->stringDepth = 0;

	return newNode;
}


/*
Node is added to the parent
*/
int SuffixTree::addChildToNode(Node *parent, Node *child) {

	Node *tempNode;

	parent->child[parent->numChild] = child;
	parent->numChild++;
	child->parent = parent;

	// sort the children
	for (int i = 0; i < parent->numChild - 1; ++i) {
		for (int j = 0; j < parent->numChild - 1 - i; ++j) {
			if (geneBuffer[parent->child[j]->beginLabel] > geneBuffer[parent->child[j + 1]->beginLabel]) {
				tempNode = parent->child[j + 1];
				parent->child[j + 1] = parent->child[j];
				parent->child[j] = tempNode;
			}
		}
	}
	return (0);
}

/*
Child is matched after the node
*/
Node *SuffixTree::matchChild(Node *node, int suffixNum, int *index) {
	Node *currentNode = NULL;
	for (*index = 0; *index < node->numChild; *index = *index + 1) {
		currentNode = node->child[*index];
		if (geneBuffer[currentNode->beginLabel] == geneBuffer[suffixNum]) {
			return currentNode;
		}
	}
	return NULL;
}

Node *SuffixTree::breakEdge(Node *currentNode, int startIndex, int childIndex) {

	for (int i = currentNode->beginLabel; i <= geneLen - 1; ++i) {
		if (geneBuffer[i] != geneBuffer[startIndex + (i - currentNode->beginLabel)]) {

			Node *newInternalNode = createNode(geneLen + internalNodeCount + 1,
				currentNode->parent, currentNode->beginLabel, i - 1);
			internalNodeCount++;

			if (newInternalNode->stringDepth > maxDepth) {
				maxDepth = newInternalNode->stringDepth;
				lowestNode = newInternalNode;
			}

			addChildToNode(newInternalNode, currentNode);
			newInternalNode->parent->child[childIndex] = newInternalNode;
			currentNode->beginLabel = newInternalNode->endLabel + 1;

			Node *newLeaf = createNode(leafCount, newInternalNode,
				startIndex + newInternalNode->endLabel - newInternalNode->beginLabel + 1, geneLen - 1);
			leafCount++;
			addChildToNode(newInternalNode, newLeaf);

			return newLeaf;

		}
	}
	return currentNode;
}

Node *SuffixTree::nodeHop(Node *v_prime, int startIndex) {
	int childIndex = 0;
	int minimum = 0;
	Node *matchedChild = NULL;
	matchedChild = matchChild(v_prime, startIndex, &childIndex);

	if (matchedChild == NULL) {
		return v_prime;
	}
	int i = 0;
	if (matchedChild != NULL) {
		minimum = min((geneLen - startIndex), (matchedChild->endLabel - matchedChild->beginLabel + 1));
		for (i = 0; i < minimum; i++) {
			if (geneBuffer[startIndex + i] != geneBuffer[matchedChild->beginLabel + i]) {
				return v_prime;
			}
		}
	}

	return nodeHop(matchedChild, startIndex + i);
}

Node *SuffixTree::findPath(Node *v, int suffixNum) {

	int childIndex = 0;

	Node *hoppedTo = nodeHop(v, suffixNum);
	int hops = hoppedTo->stringDepth - v->stringDepth;

	Node *matchedChild = NULL;
	matchedChild = matchChild(hoppedTo, suffixNum + hops, &childIndex);

	if (matchedChild != NULL) {
		matchedChild = breakEdge(matchedChild, suffixNum + hops, childIndex);
	}

	else {
		matchedChild = createNode(leafCount, hoppedTo, suffixNum + hops, geneLen - 1);
		addChildToNode(hoppedTo, matchedChild);
		leafCount++;
	}

	return matchedChild;
}

Node *SuffixTree::establishSuffixLink(Node *v_prime, Node *u, int betaBegin, int betaEnd, int suffix) {
	int childIndex = 0;
	int counter = 0;
	int betaLen = betaEnd - betaBegin + 1;
	Node *currentNode = v_prime;
	Node *internalNode = NULL, *matchedChild = NULL, *v = NULL;

	while (counter <= betaLen) {
		matchedChild = matchChild(currentNode, betaBegin + counter, &childIndex);

		if (matchedChild) {
			int edgeLen = (matchedChild->endLabel - matchedChild->beginLabel + 1);
			if (edgeLen + counter > betaLen) {
				internalNode = breakEdge(matchedChild, suffix + currentNode->stringDepth, childIndex);
				//establish SL
				v = internalNode->parent;
				u->suffixLink = v;
				return internalNode;
			}
			else if (edgeLen + counter == betaLen) {
				v = matchedChild;
				u->suffixLink = v;
				internalNode = findPath(v, suffix + u->stringDepth - 1);
				return internalNode;
			}
			else {
				counter = counter + edgeLen;
				currentNode = matchedChild;
				continue;
			}
		}

	}

	return internalNode;

}

Node *SuffixTree::insertSuffix(int suffixNum, Node *root, Node *leaf) {

	if (leaf == NULL) {
		return NULL;
	}

	Node *u = leaf->parent;

	//Case 1A: u has suffixLink and u != root
	if (u->suffixLink != NULL && u != root) {
		int alpha = u->stringDepth - 1;
		Node *v = u->suffixLink;

		if (v->id != 0) {
			return findPath(v, suffixNum + alpha);
		}
		else if (v->id == 0) {
			return findPath(v, suffixNum);
		}
	}

	//Case 1B: u has suffixLink and u == root
	else if (u->suffixLink != NULL && u == root) {
		return findPath(u, suffixNum);
	}

	//Case 2A: u does not have a suffixLink and u_prime (u's parent) is not equal to root
	else if (u->suffixLink == NULL && u->parent != root) {
		int betaBegin = u->beginLabel;
		int betaEnd = u->endLabel;
		Node *u_prime = u->parent;
		Node *v_prime = u_prime->suffixLink;

		return establishSuffixLink(v_prime, u, betaBegin, betaEnd, suffixNum);

	}

	//Case 2B: u does not have a suffixLink and u_prime (u's parent) is equal to root
	else if (u->suffixLink == NULL && u->parent == root) {
		int betaBegin = u->beginLabel;
		int betaEnd = u->endLabel;
		Node *u_prime = u->parent;

		if (betaBegin != betaEnd) {
			return establishSuffixLink(u_prime, u, betaBegin + 1, betaEnd, suffixNum);
		}
		else {
			u->suffixLink = root;
			return findPath(u_prime, suffixNum);
		}
	}

}

Node *SuffixTree::suffixTree(std::string gene, int numAlphabets) {

	//assigning the input gene string to the local-global variable;
	geneLen = gene.length();
	geneBuffer = new char[geneLen];
	alphaNum = numAlphabets;

	for (int i = 0; i < geneLen; i++) {
		geneBuffer[i] = gene[i];
	}

	root = createNode(0, NULL, 0, 0);
	root->suffixLink = root;
	Node *leaf = root;

	float sizeNode = sizeof(root);
	float sizeST = sizeof(SuffixTree);

	for (int i = 0; i < geneLen; i++) {
		leaf = insertSuffix(i, root, leaf);
		if (leaf == NULL) {
			return NULL;
		}
	}

	cout << "\nTotal size required for Tree in Bytes: " << (2 * sizeNode * geneLen) + sizeST;
	cout << "\nTotal memory used in suffix tree: " << sizeNode* (leafCount + internalNodeCount) + sizeST;

	return root;
}
