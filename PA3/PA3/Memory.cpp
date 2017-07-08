#include"Memory.h"
#include<Windows.h>
#include<iostream>

using namespace std;

void Memory::freeMemory(Node* node)
{
	for (int i = 0; i < node->numChild; i++)
	{
		if (node->child[i]->visited == false)
		{
			freeMemory(node->child[i]);
		}
	}
	free(node);
}

void Memory::detectFreeMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	cout << "\nThe amount of physical memory currently available, in MB: " << (status.ullAvailPhys / (1024 * 1024));

	cout << "\nThe amount of actual physical memory, in MB: " << (float)status.ullTotalPhys / (1024 * 1024) << endl;

	return;
}