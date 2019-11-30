#ifndef B_TREE_H
#define B_TREE_H
#include <stdio.h>
#include <stdlib.h>
#define M 3
typedef int KeyType;

typedef struct BTNode
{
	int keynum;
	KeyType key[M + 1];
	struct BTNode* parent;
	struct BTNode* ptr[M + 1];
	// Record* recptr[M+1];

} BTNode, * BTree;

typedef struct
{
	BTree pt;
	int i;
	int tag;
} RESNode, *RES;

int Search(BTree p, int k);
// fun: searching in a no-left node 
// ret: the postion of node which value <= k
// key: none


void SearchBTree(BTree t, int k, RES &result);
// fun: searching in a btree 
// ret: a result ptr including (pt, i, tag)
// key: if searching successfully, tag = 1, [*pt][i]=k
//      else tag = 0, return the postion which can be inserted.


void split(BTree &q, int s, BTree &ap);


void newRoot(BTree &t, BTree p, int x, BTree ap);


void Insert(BTree &q, int i, int x, BTree ap);


void InsertBTree(BTree &t, int k, BTree q, int i);


void DeleteBTree(BTree &p, int i);


void ShowBTree(BTree root, int layer);
//
#endif