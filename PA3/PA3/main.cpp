// ReadMapping.cpp : Defines the entry point for the console application.

#include"ReadMap.h"
#include"InputOutput.h"
#include"SuffixTree.h"
#include"Memory.h"
#include"Node.h"
#include<iostream>
#include<string>
#include<cstdlib>
#include<algorithm>
#include<cstdio>
#include<ctime>

using namespace std;


int main(int argc, char *argv[])
{
	InputOutput io;
	SuffixTree st;
	ReadMap rm;
	Node *constructedTree;
	Memory mem;
	string gene = "";
	string alphabet = "";
	int numAlphabets = 0;
	clock_t start, startProgram;
	double duration;
	startProgram = clock();
	try
	{
		if (argc != 4)
		{
			cout << "\nNumber of command line argumets are wrong.\nCommands should be\n";
			cout << "1. FASTA file containing reference genome sequence G\n2. FASTA file containing reads";
			cout << "\n3. Input alphabet file\n\n";
			exit(EXIT_FAILURE);
		}
		else
		{
			mem.detectFreeMemory();
			gene = io.inputGenome(argv[1]);
			//io.readReads(argv[2]);
			numAlphabets = io.alphabetParse(argv[3]);
			start = clock();
			constructedTree = st.suffixTree(gene, numAlphabets);
			duration = (clock() - start) / (double)CLOCKS_PER_SEC;
			cout << "\nTime in seconds for construction of Suffix tree: " << duration;

			start = clock();
			rm.PrepareST(constructedTree);
			duration = (clock() - start) / (double)CLOCKS_PER_SEC;
			cout << "\nTime in seconds for Prepare ST: " << duration;

			start = clock();
			rm.MapReads(constructedTree, argv[2]);
			duration = (clock() - start) / (double)CLOCKS_PER_SEC;
			cout << "\nTime in seconds for Map Read: " << duration;

			mem.freeMemory(constructedTree);
			duration = (clock() - startProgram) / (double)CLOCKS_PER_SEC;
			cout << "\nTime in seconds for whole program: " << duration;
			getchar();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception: " << e.what();
	}
	getchar();
	return 0;
}