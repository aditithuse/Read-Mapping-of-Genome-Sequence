#pragma once

struct Node
{
public:
	int id;
	int beginLabel;
	int endLabel;
	int stringDepth;
	int numChild;
	Node *parent;
	Node **child;
	Node *suffixLink;
	bool visited;
	int start_leaf_index; 
	int end_leaf_index;
};