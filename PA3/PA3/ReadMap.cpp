#include"ReadMap.h"
#include"SuffixTree.h"
#include"InputOutput.h"
#include"LocalAlignment.h"
#include<iostream>

using namespace std;

int nextIndex;
int* A;
unsigned long alignmentCounter = 0;
void ReadMap::PrepareST(Node* root)
{
	createArray(geneLen);
	DFS_PrepareST(root, A);
}

void ReadMap::createArray(int genomeLength)
{
	A = (int*)malloc(sizeof(int)*(genomeLength));

	if (A)
		for (int i = 0; i < genomeLength; i++)
			A[i] = -1;
	else
	{
		cout << "Array cannot be initialized";
		exit(EXIT_FAILURE);
	}
}

void ReadMap::DFS_PrepareST(Node *T, int A[])
{
	if (T == NULL)
		return;

	if (T->numChild == 0) {
		A[nextIndex] = T->id;
		if (T->stringDepth >= 5) {
			T->start_leaf_index = nextIndex;
			T->end_leaf_index = nextIndex;
		}
		nextIndex++;
		return;
	}
	else {
		for (int i = 0; i < T->numChild; i++)
		{
			DFS_PrepareST(T->child[i], A);
		}
	}

	if (T->stringDepth >= 5 && T->numChild != 0) {
		Node* u_left = T->child[0];
		Node* u_right = T->child[T->numChild - 1];
		T->start_leaf_index = u_left->start_leaf_index;
		T->end_leaf_index = u_right->end_leaf_index;
	}
}


Node *ReadMap::FindLoc(Node *root, string read, int *maxDepth) {

	Node *T = root, *deepestNode = T;
	int read_ptr = 0, charMatchesBelowEdge = 0, counter = 0;
	bool childExists = false, flag = false;
	*maxDepth = 0;

	while (read_ptr < read.length()) {
		childExists = flag = false;
		charMatchesBelowEdge = counter = 0;

		for (int i = 0; i < T->numChild; i++) {
			if (geneBuffer[T->child[i]->beginLabel] == read[read_ptr]) {
				T = T->child[i];
				childExists = true;
				break;
			}
		}
		if (!childExists) {
			T = T->suffixLink;
		}

		else {
			while (T->beginLabel + charMatchesBelowEdge <= T->endLabel && read_ptr < read.length()) {
				if (geneBuffer[T->beginLabel + charMatchesBelowEdge] == read[read_ptr]) {
					read_ptr++;
					charMatchesBelowEdge++;
				}
				else {
					flag = true;
					read_ptr = read_ptr - charMatchesBelowEdge;
					break;
				}
			}
			if (T->parent->stringDepth + charMatchesBelowEdge > *maxDepth && flag) {
				*maxDepth = T->parent->stringDepth + charMatchesBelowEdge;
				deepestNode = T->parent;
			}
			else if (T->parent->stringDepth + charMatchesBelowEdge > *maxDepth && !flag) {
				*maxDepth = T->parent->stringDepth + charMatchesBelowEdge;
				deepestNode = T;
			}
			if (flag) 
				T = T->parent->suffixLink;
			else {
				for (int i = 0; i < T->numChild; i++) {
					if (geneBuffer[T->child[i]->beginLabel] != read[read_ptr]) {
						counter++;
					}
					else break;
				}
				if (counter == T->numChild)
					T = T->suffixLink;
			}
		}
	}
	return deepestNode;
}

string ReadMap::getSubString(int readLen, int leafIDfromA, int *startIndex, int *endIndex) {
	string subString = "";

	if (leafIDfromA - readLen < 0) {
		*startIndex = 0;
	}
	else {
		*startIndex = leafIDfromA - readLen;
	}
	if (leafIDfromA + (readLen) > geneLen) {
		*endIndex = geneLen-2;
	}
	else {
		*endIndex = leafIDfromA + (readLen);
	}

	for (int i = *startIndex; i <= *endIndex; i++) {
		subString += geneBuffer[i];
	}

 	return subString;
}

//Step 3
void ReadMap::MapReads(Node *root, string inputPath) {

	string parameters[4] = { "1", "-2", "-5", "-1" };
	int readLen = 0;
	float alignmentLen = 0.0;
	int startIndex = 0, endIndex = 0, maxDepth = 0;
	float percentIdentity = 0.0, coverageLength = 0.0, maxCoverageLength = 0.0;
	int bestHitStart = 0, bestHitEnd = 0;
	Node *deepestNode = NULL;
	string subStringFromGene = "";
	fstream bestMatch;
	LocalAlignment local;
	bool flag = false;
	std::ifstream input;
	string readIdentifier, readSequence, line;
	unsigned long alignmentCounter=0, readcount=0;
	input.open(inputPath);
	if (input.is_open())
	{
		while (getline(input, line))
		{
			readcount++;
			//cout << readcount;
			readIdentifier = line;
			getline(input, readSequence);
			maxCoverageLength = 0.0;
			flag = false;
			char chars[] = "BDEFHIJKLMNOPQRSUVWXYZ";

			for (unsigned int i = 0; i < strlen(chars); ++i)
				readSequence.erase(std::remove(readSequence.begin(), readSequence.end(), chars[i]), readSequence.end());

			deepestNode = FindLoc(root, readSequence, &maxDepth);
			//Step 3(c) - for each j in array A
			if (deepestNode->start_leaf_index > -1) {
				for (int j = deepestNode->start_leaf_index; j <= deepestNode->end_leaf_index && maxDepth >= 25; j++) {
					alignmentCounter++;
					subStringFromGene = getSubString(readSequence.length(), A[j] - 1, &startIndex, &endIndex);
					local.localMain(readSequence, subStringFromGene, parameters);
					subStringFromGene = "";
					alignmentLen = countMatch + countMismatch + countGap;
					percentIdentity = (float)countMatch / alignmentLen;
					coverageLength = alignmentLen / float(readSequence.length());

					countMatch = countMismatch = countOpeningGap = countGap = 0;

					if (percentIdentity >= 0.9 && coverageLength >= 0.8) {
						if (coverageLength > maxCoverageLength) {
							maxCoverageLength = coverageLength;
							bestHitStart = startIndex;
							bestHitEnd = endIndex;
							flag = true;
						}
					}
				}
			}

			//Step 4 - output the best hit
			bestMatch.open("MappingResults_Cherry_reads_Real.txt", fstream::in | fstream::out | fstream::app);
			if (bestMatch.is_open() && flag == true) {
				bestMatch << readIdentifier << " G[" << bestHitStart << ".." << bestHitEnd << "]\n";
				bestMatch.close();
			}
			else if (bestMatch.is_open() && flag == false)
			{
				bestMatch << readIdentifier << " No hit found\n";
				bestMatch.close();
			}
			else
			{
				cout << "Error in opening the file";
				exit(EXIT_FAILURE);
			}
		}
	}
	cout << "\nTotal number of alignments: " << alignmentCounter;
	cout << "\nTotal number of reads: " << readcount;
	cout << "\nAverage number of alignments performed per read: " << alignmentCounter / (readcount);
}