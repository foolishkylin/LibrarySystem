#ifndef B_TREE_H
#define B_TREE_H
#include <stdio.h>
#include <stdlib.h>
#define M 3
#define FALSE 0
#define TRUE 1
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
	BTree ptr;
	int pos;
	int tag;
} RESNode, *RES;

enum RESULT_TAG {
	SEARCH_SUCCESS = 0, 
	SEARCH_FAIL = 1
};


int Search(BTree search_node, int search_value);
// fun: searching in a no-left node 
// ret: the postion of node which value <= search_value
// key: none


void SearchBTree(BTree tree, int search_value, RES& search_result);
// fun: searching in a btree 
// ret: a result ptr including (pt, i, tag)
// key: if searching successfully, tag = 1, [*pt][i]=k
//      else tag = 0, return the postion which can be inserted.


void Split(BTree& current_node, int split_postion, BTree& splited_node);
// fun: split the current_node
// ret: none
// key: for the position split_postio, cut the current_node->key, the front ones are belong to
//		current_node and the behind are belong to splited_node 


void NewRoot(BTree& new_root, BTree left_son, int root_key, BTree right_son);
// fun: create a new root for the nodes left_son and right_son
// ret: none
// key: if sons == 0, then this func is to create a true root


void Insert(BTree& current_node, int inserted_postion, int inserted_value, BTree inserted_node);
// fun: insert a new node
// ret: none
// key: none


void InsertBTree(BTree& tree, int inserted_value, BTree inserted_node, int insert_position);
// fun: insert a new key to tree
// ret: none
// key: none


void DeleteBTree(BTree& deleted_node, int deleted_postion, BTree &t);

void Successor(BTree& deleted_node, int deleted_postion);

void Remove(BTree& deleted_node, int deleted_postion);

void Restore(BTree& deleted_node, int deleted_postion, BTree &t);


void ShowBTree(BTree root, int layer);
// fun: show the whole btree
// ret: none
// key: print the tree according the layer of tree
#endif